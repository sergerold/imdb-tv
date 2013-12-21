


#include "../include/curlandclean.h"
#include "../libcurl/include/curl/curl.h"
#include "../tidy/tidy-html5-master/include/tidy.h"
#include "../tidy/tidy-html5-master/include/buffio.h"



std::string cleanHTML (std::string);
std::string get_html_content (std::string);


std::string getCleanedHTML (std::string URL)
{
	std::string HTML = get_html_content (URL);
	return cleanHTML (HTML);

}


std::string cleanHTML (std::string html)
{


  TidyDoc tidyDoc = tidyCreate();
    TidyBuffer tidyOutputBuffer = {0};



// Configure Tidy
    // The flags tell Tidy to output XML and disable showing warnings
    bool configSuccess = tidyOptSetBool(tidyDoc, TidyXmlOut, yes)
        && tidyOptSetBool(tidyDoc, TidyQuiet, yes)
        && tidyOptSetBool(tidyDoc, TidyNumEntities, yes)
        && tidyOptSetBool(tidyDoc, TidyShowWarnings, no);

	tidyOptSetValue(tidyDoc,TidyForceOutput,"true");

int tidyResponseCode = -1;

    // Parse input
    if (configSuccess)
        tidyResponseCode = tidyParseString(tidyDoc, html.c_str());


 
    // Process HTML
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidyCleanAndRepair(tidyDoc);
 
    // Output the HTML to our buffer
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidySaveBuffer(tidyDoc, &tidyOutputBuffer);

// Any errors from Tidy?
    if (tidyResponseCode < 0)
        throw ("Tidy encountered an error while parsing an HTML response. Tidy response code: " + tidyResponseCode);

	
 
    // Grab the result from the buffer and then free Tidy's memory

    std::string tidyResult = (char*)tidyOutputBuffer.bp;
	   
    tidyBufFree(&tidyOutputBuffer);
    tidyRelease(tidyDoc);
 
    return tidyResult;


}




//GET HTML FUNCTIONS

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);

  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}




std::string get_html_content (std::string url)
{



  std::string htmlcontent;
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = (char*) malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

 
  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */ 
 
    htmlcontent = chunk.memory;
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  if(chunk.memory)
    free(chunk.memory);
 
  /* we're done with libcurl, so clean it up */ 
  //curl_global_cleanup();

  return htmlcontent;

}


//remove new lines

std::string removenewlines(std::string unformattedstr)
{
//remove new break

	for (int a=0;a <unformattedstr.length();a++)
	{
		if (unformattedstr[a] == '\n')
		{
			unformattedstr.erase(a,1);
			unformattedstr.insert (a," ");
		}
	}


return unformattedstr;
}

std::string removecommas(std::string unformattedstr)
{
	for (int a=0; a < unformattedstr.length();a++)
	{
		if (unformattedstr[a] == ',')
		{		
			unformattedstr.erase(a,1);
			a--;
		}
	}
return unformattedstr;
}

std::string trim_whitespace (std::string input)
{
	 
   for (int i=0; i <input.size();i++)
   {
       if (input[i] != ' ')
            break;
            
        input.erase(i,1);
        i--;
        
        
   }
   
   for (int i=input.size()-1;i>=0;i--)
   {
       if (input[i]!= ' ')
            break;
    
        input.erase(i,1);
   }

   return input;
}

std::string removeStr(std::string str, std::string toRemove)
{
    size_t findval = str.find(toRemove);
    
    if (findval == std::string::npos)
        return str;
        
    str.erase(findval,toRemove.length());
    
    return str;
}



//PUGI WRITE

struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
};

std::string node_to_string(pugi::xml_node node)
{
    xml_string_writer writer;
    node.print(writer);

    return writer.result;
}

