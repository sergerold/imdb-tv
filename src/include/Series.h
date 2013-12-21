#include <string>
#include <vector>
#include "Episode.h"

class Series
{

	public:
	 	Series (std::string URL);
		std::string getURL ();
		std::string getXML();

		void getEpisodes(); //GET ALL INDIVIDUAL EPISODE DETAILS

		void setName (std::string);
		std::string getName ();

		void setEpisode_count (std::string);
		std::string getEpisode_count ();

		void setAirdate (std::string);
		std::string getAirdate ();

		void setRating (std::string);
		std::string getRating ();

		void setVote_count (std::string);
		std::string getVote_count ();

		void addEpisode(Episode);
		std::vector <Episode> getEpisode_list ();
		

	private:
		std::string name;
		std::string episode_count;
		std::string airdate;
		std::string rating;
		std::string vote_count;
		std::vector <Episode> episode_list;
		std::string URL;


};
