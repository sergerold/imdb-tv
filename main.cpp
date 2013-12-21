#include "src/include/Series.h"
#include "src/include/Episode.h"
#include <iostream>
#include <vector>

int main()
{
	//threaded	 

	try
	{
		Series test ("/title/tt0436992/");
		test.getEpisodes();
		std::cout << test.getXML();
	}

	catch (const char* msg)
	{
    		 std::cerr << msg << std::endl;
   	}


}
