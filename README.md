INTRO

imdb-tv is a library which can be used to retrieve data about tv shows and episodes.

It can get:
--Show/episode names
--Airdates
--Ratings
--Total votes cast
--Season/episode numbers

*This program is multithreaded to ensure that all bandwidth (which will probably be the bottleneck) is used.

It can also write xml for this data

INSTALLATION

simple run 'make'

USAGE

--If you want to use the library then simply link against imdb.a
--YOU MUST LINK AGAINST -fopenmp -lpthread due to multithreading requirements
-- You must include:
- "src/include/Series.h" --includes Episode.h by default
OR
- "src/include/Episode.h" 

OBJECTS AND FUNCTIONS

--There are two fundamental types:
-Series 
-Episode

-These are initialised by the constructor parameter of the IMDB ID e.g. '/title/tt1475582/' i.e. NOT the absolute URL

-If create a Series object then calling the constructor will automatically retrieve the ID of every episode of that Series

-To retrieve the actual data for each episode then call Series::getEpisodes()

-To return a std::string of XML for that Series the call Series::getXML()

-You can access data for that episode by the methods listed in Series.h and Episode.h

WARNING: ANY CHANGE TO IMDB HTML DESIGN MAY CAUSE THIS PROGRAM TO FAIL (PARTIALLY OR ENTIRELY)





