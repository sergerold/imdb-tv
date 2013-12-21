#include "include/Series.h"
#include "include/curlandclean.h"
#include "pugixml/src/pugixml.hpp"


 #define NUM_THREADS 20

Series::Series (std::string inputURL)
{

	//set URL
	URL = inputURL;

	//get episode URLs

	std::string seasonURL = "?season=1";
	std::vector <std::string> episodeURLs;	


	while (true)
	{
		std::string listcontent = getCleanedHTML (baseURL + getURL() + "episodes" + seasonURL);
		pugi::xml_document doc;
 		doc.load_buffer(listcontent.c_str(), listcontent.size());

		pugi::xpath_node_set link_nodes = doc.select_nodes("//a[@itemprop='name']");

		for (pugi::xpath_node_set::const_iterator it = link_nodes.begin();it != link_nodes.end();++it)
   		{
			      		  pugi::xpath_node single_link_node = *it;
					  addEpisode(Episode (single_link_node.node().attribute("href").value()));
		}

		//check if next season exists. IF so then get new content
	
		pugi::xml_node next_link_node = doc.select_single_node("//a[@id='load_next_episodes']").node();

		if (next_link_node)
			seasonURL = next_link_node.attribute("href").value();	

		else
			break;
	
	}



	//get series information

	std::string homecontent = getCleanedHTML (baseURL + getURL());
	pugi::xml_document doc;
 	doc.load_buffer(homecontent.c_str(), homecontent.size());

	pugi::xml_node series_value;

	//set episode count
	
	if (!episode_list.size())
		throw "ERROR: NO EPISODES FOUND";

	setEpisode_count (std::to_string(getEpisode_list().size()));

	//set name
	series_value = doc.select_single_node("//span[@itemprop='name']").node();
	setName(series_value.child_value());

	if (!series_value)
		std::cerr<< "WARNING: Name not found";



	//set rating
	series_value = doc.select_single_node("//span[@itemprop='ratingValue']").node();
	setRating(series_value.child_value());

	if (!series_value)
		std::cerr<< "WARNING: Rating not found";

	//set vote_count
	series_value = doc.select_single_node("//span[@itemprop='ratingCount']").node();
	setVote_count(series_value.child_value());

	if (!series_value)
		std::cerr<< "WARNING: Total votes not found";

	//set airdate
	series_value = doc.select_single_node("//div/h4[text()='Release Date:']/parent::div").node();
	setAirdate(series_value.child_value());


	if (!series_value)
		std::cerr<< "WARNING: Airdate not found";

	


	
}

std::string Series::getXML()
{
	//create series node
	pugi::xml_document series_doc;
	pugi::xml_node series_node = series_doc.append_child();
	series_node.set_name("series");
	series_node.append_attribute("name") = getName().c_str();
	series_node.append_attribute("URL") = getURL().c_str();
	series_node.append_attribute("Total-episode") = getEpisode_count().c_str();
	series_node.append_attribute("Airdate") = getAirdate().c_str();
	series_node.append_attribute("Rating") = getRating().c_str();
	series_node.append_attribute("Total-votes") = getVote_count().c_str();
	

	//loop through seasons/episodes
		
	std::string current_season = episode_list[0].getSeason_number();
	pugi::xml_node season_node = series_node.append_child();
	season_node.set_name("Season");
	season_node.append_attribute("number") = episode_list[0].getSeason_number().c_str();

	for (int epIt = 0; epIt <episode_list.size();epIt++)
	{
		if (current_season != episode_list[epIt].getSeason_number())		//new season
		{
			season_node = series_node.append_child();
			season_node.set_name("Season");
			season_node.append_attribute("number") = episode_list[epIt].getSeason_number().c_str();
			current_season = episode_list[epIt].getSeason_number();

		}

		//add each episode
			
		std::string episode_xml = episode_list[epIt].getXML();
		pugi::xml_document episode_doc;
		episode_doc.load(episode_xml.c_str());
		season_node.append_copy(episode_doc.child("episode"));
		
		
		
		
		
	}

		


	return node_to_string (series_node);	

}

std::string Series::getURL ()
{

	return URL;
}

void Series::getEpisodes()
{
	//check URL set


     //threaded	 
   	#pragma omp parallel for num_threads(NUM_THREADS)
	for ( int i=0; i < episode_list.size();i++)
		episode_list[i].getEpisode();

}



//getters and setters

void Series::setName(std::string inputName)
{
	name = inputName;
}

std::string Series::getName ()
{
	return name;
}



void Series::setEpisode_count (std::string inputEpisode_count)
{
	episode_count = inputEpisode_count;
}

std::string Series::getEpisode_count()
{	
	return episode_count;
}



void Series::setAirdate (std::string inputAirdate)
{
	inputAirdate.erase(inputAirdate.size()-1,1);
	airdate = inputAirdate;
}

std::string Series::getAirdate ()
{
	return airdate;
}


void Series::setRating (std::string inputRating)
{
	rating = inputRating;
}

std::string Series::getRating ()
{
	return rating;
}




void Series::setVote_count (std::string inputVote_count)
{
	vote_count = removecommas(inputVote_count);
}

std::string  Series::getVote_count ()
{
	return vote_count;
}




void Series::addEpisode (Episode inputEpisode)
{
	episode_list.push_back (inputEpisode);
}


std::vector <Episode> Series:: getEpisode_list()
{
	return episode_list;
}



