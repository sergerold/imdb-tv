#ifndef EPISODE_H
#define EPISODE_H

#include <iostream>

class Episode
{

	public:
		
		Episode(std::string URL);		
		std::string getURL ();

		void getEpisode();                      //GET EPISODE DETAILS
		std::string getXML();			//GET XML FOR EPISODE

		void setName (std::string);
		std::string getName ();

		void setAirdate (std::string);
		std::string getAirdate ();

		void setRating (std::string);
		std::string getRating ();

		void setEpisode_season (std::string);
		std::string getSeason_episode_number ();
		std::string getSeason_number();
	
		void setVote_count (std::string);
		std::string getVote_count();

		void setSeries_episode_number(std::string);
		std::string getSeries_episode_number ();

		void setSeason (std::string);
		std::string getSeason();

	private:
		std::string name;
		std::string airdate;
		std::string season_number;
		std::string season_episode_number;
		std::string series_episode_number;

		std::string rating;
		std::string vote_count;
		std::string URL;



};
#endif /* EPISODE_H */

