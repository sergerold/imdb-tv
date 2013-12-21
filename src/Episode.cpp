#include "include/Episode.h"
#include "include/curlandclean.h"
#include "pugixml/src/pugixml.hpp"

#include <string>


Episode::Episode (std::string inputURL)
{
	URL = inputURL;
}

std::string Episode::getURL()
{
	return URL;
}



void Episode::getEpisode()
{
	//get document
	std::string episodecontent = getCleanedHTML (baseURL + getURL());
	pugi::xml_document doc;
 	doc.load_buffer(episodecontent.c_str(), episodecontent.size());

	//parse and insert details

	
	pugi::xml_node episode_value;

	//set name
	episode_value = doc.select_single_node("//h1[@class='header']/span[@class='itemprop']").node();
	setName (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Not found. Terminating"<<std::endl;

	//set rating
	episode_value = doc.select_single_node("//span[@itemprop='ratingValue']").node();
	setRating (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Rating not found"<<std::endl;

	//set airdate
	episode_value = doc.select_single_node("//h1[@class='header']/span[@class='nobr']").node();
	setAirdate (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Airdate not found"<<std::endl;


	//set vote_count
	episode_value = doc.select_single_node("//span[@itemprop='ratingCount']").node();
	setVote_count (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Total votes not found"<<std::endl;


	//set episode_season
	episode_value = doc.select_single_node("//h2[@class='tv_header']/span[@class='nobr']").node();
	setEpisode_season (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Season/episode not found"<<std::endl;

	//set series_episode_number
	episode_value = doc.select_single_node("//div[@id='titleTVEpisodes']/strong").node();
	setSeries_episode_number (episode_value.child_value());

	 if (!episode_value)
		std::cerr<< "WARNING: Series episode number not found"<<std::endl;



}


std::string Episode::getXML()
{
		//setup episode XML
		pugi::xml_document episode_doc;
		pugi::xml_node episode_node = episode_doc.append_child();


		//name and attributes
		episode_node.set_name("episode");
		episode_node.append_attribute("URL") = getURL().c_str();
		episode_node.append_attribute("Series-number") = getSeries_episode_number().c_str();

		//child notes etc
		pugi::xml_node episode_details_node;
		//name

		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("name");
		episode_details_node.text().set(getName().c_str());
		
		//rating
		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("rating");
		episode_details_node.text().set(getRating().c_str());

		//airdate
		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("airdate");
		episode_details_node.text().set(getAirdate().c_str());

		//votes
		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("votes");
		episode_details_node.text().set(getVote_count().c_str());

		//season
		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("Season");
		episode_details_node.text().set(getSeason_number().c_str());
		
		//episode
		episode_details_node = episode_node.append_child();
		episode_details_node.set_name("Episode");
		episode_details_node.text().set(getSeason_episode_number().c_str());

		return node_to_string (episode_node);


}




void Episode::setName(std::string inputName)
{
	inputName = removenewlines (inputName);
	name = inputName;
}

std::string Episode::getName ()
{
	return name;
}




void Episode::setAirdate (std::string inputAirdate)
{
	airdate = inputAirdate;
}

std::string Episode::getAirdate ()
{
	return airdate;
}




void Episode::setRating (std::string inputRating)
{
	rating = inputRating;
}

std::string Episode::getRating ()
{
	return rating;
}




void Episode::setVote_count (std::string inputVote_count)
{
	inputVote_count = removecommas(inputVote_count);
	vote_count = inputVote_count;
	
}

std::string  Episode::getVote_count ()
{
	return vote_count;
}



void Episode::setEpisode_season (std::string inputEpisode_season)
{
	 
    bool cfound = false;
    
    for (int i=0;i < inputEpisode_season.size();i++)
    {
        if (inputEpisode_season[i]==',')
        {
            cfound = true;
            continue;
        }
        
        if (!cfound)
            season_number += inputEpisode_season[i];
        
        else
            season_episode_number += inputEpisode_season[i];
    }
     //remove prefixing 'season'/'episode'
	season_number = removeStr(season_number,"Season");
	season_episode_number = removeStr (season_episode_number,"Episode");

    //trim any white space
    season_number = trim_whitespace(season_number);
    season_episode_number = trim_whitespace(season_episode_number);
    

}

std::string Episode::getSeason_episode_number()
{
	return season_episode_number;
}

std::string Episode::getSeason_number()
{
	return season_number;
}





void Episode::setSeries_episode_number(std::string inputSeries_episode)
{
	series_episode_number = inputSeries_episode;
}
std::string Episode::getSeries_episode_number ()
{
	return series_episode_number;
}


