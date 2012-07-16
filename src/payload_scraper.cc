/* Copyright 2012 vmon
   See LICENSE for other credits and copying information
*/

#include <iostream>
#include <algorithm> //removing quotes from path
#include <fstream> 
#include <string>
#include <stdio.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

#define DELETE_METHOD = delete

#include "util.h"
#include "crypt.h"
#include "payloads.h"
#include "payload_scraper.h"
#include "base64.h"

/** We read the /etc/httpd/conf/httpd.conf (this need to be more dynamic)
    but I'm testing it on my system which is running arch) find
    the DocumentRoot. Then it will check the directory recursively and
    gather the name of all files of pdf, swf and js type and store them
    in a database file.
*/

/**
   Scrapes current directory, recursively. it uses a boost library.
   returns number of payload if successful -1 if it fails.

   @param cur_dir the name of the dir to be scraped
*/
int payload_scraper::scrape_dir(const path dir_path)
{
  long int total_file_count = 0;
  char url_hash[20];
  char url_hash64[40];

  if ( !exists( dir_path ) ) 
      return -1;

  recursive_directory_iterator end_itr; // default construction yields past-the-end
  for ( recursive_directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr, total_file_count++)
    {
      for(steg_type* cur_steg = _available_stegs; cur_steg->type!= 0; cur_steg++)
        if (cur_steg->extension == itr->path().extension().string())
          {
            string cur_filename(itr->path().generic_string());
            string cur_url(cur_filename.substr(_apache_doc_root.length(), cur_filename.length() -  _apache_doc_root.length()));
            sha256((const unsigned char *)(cur_url.c_str()), cur_url.length(), (unsigned char*)url_hash);
            base64::encoder url_hash_encoder;
            url_hash_encoder.encode(url_hash, 20, url_hash64);
            
            ifstream cur_file;
            cur_file.open(cur_filename.c_str()); //, ios::binary | ios::in);
            
            if (!cur_file.is_open())
              {
                fprintf(stderr, "Error opening payload for capacity analyze.");
                continue;
              }
            
            cur_file.seekg (0, ios::end);
            unsigned long cur_filelength = cur_file.tellg();
            cur_file.seekg (0, ios::beg);
            
            char* payload_buf = new char[cur_filelength];
            cur_file.read(payload_buf, cur_filelength);
            
            unsigned long capacity = cur_steg->capacity_function(payload_buf, cur_filelength);
            
            cur_file.close();
            delete payload_buf;
            
            _payload_db << total_file_count << ", " << cur_steg->type << "," << url_hash64 << "," << capacity << "," << cur_filelength << ", " << cur_url <<"\n";
          }
    }

  return total_file_count; 

}

/** 
    The constructor, calls the scraper by default
    
    @param database_filename the name of the file to store the payload list   
*/
payload_scraper::payload_scraper(string  database_filename, string apache_conf)
  :_available_stegs(NULL)
{
  _database_filename = database_filename;
  _apache_conf_filename  = apache_conf;

  /** This is hard coded */
  _available_stegs = new steg_type[_c_no_of_steg_protocol];

  _available_stegs[0].type = HTTP_CONTENT_JAVASCRIPT; _available_stegs[0].extension = ".js";  _available_stegs[0].capacity_function = capacityJS;
  _available_stegs[1].type = HTTP_CONTENT_PDF; _available_stegs[1].extension = ".pdf"; _available_stegs[1].capacity_function = capacityPDF;
  _available_stegs[2].type = HTTP_CONTENT_SWF; _available_stegs[2].extension = ".swf";  _available_stegs[2].capacity_function = capacitySWF;
  _available_stegs[3].type = HTTP_CONTENT_HTML; _available_stegs[3].extension = ".html";  _available_stegs[3].capacity_function = capacityJS;
  _available_stegs[4].type = HTTP_CONTENT_HTML; _available_stegs[4].extension = ".htm";  _available_stegs[5].capacity_function = capacityJS;

 _available_stegs[5].type = 0;
}

/** 
    reads all the files in the Doc root and classifies them. return the number of payload file founds. -1 if it fails
*/
int payload_scraper::scrape()
{
  /* open the database file for write this will delete the
     current content */
  _payload_db.open(_database_filename.c_str());

  if (!_payload_db.is_open())
    {
      fprintf(stderr, "Error opening the payload database file.");
      return -1;
    }

  /* look for doc root dir */
  if (apache_conf_parser())
    {
      fprintf(stderr, "Error in retrieving apache doc root.\n");
      _payload_db.close();
      return -1;
    }
  
  /* now all we need to do is to call scrape */
  path dir_path(_apache_doc_root);
  if (scrape_dir(dir_path) < 0)
    {
      fprintf(stderr, "Error in retrieving payload dir.\n");
      _payload_db.close();
      return -1;
    }
    
  _payload_db.close();
  return 0;
  
}

/** 
    open the apache configuration file, search for DocumentRoot
    and set the 

*/
int payload_scraper::apache_conf_parser()
{
  /* open the apache config file to find the doc root dir*/
  FILE* apache_conf;

  apache_conf = fopen(_apache_conf_filename.c_str(), "rb");
  if (apache_conf == NULL)
    {
      fprintf(stderr, "Error opening apache config file.");
      return 0;
    }

  char* cur_line = NULL;
  size_t line_length;
  while(~feof(apache_conf))
    {
      xgetline(&cur_line, &line_length, apache_conf);
      /*pass the comment*/
      cout << cur_line;
      if ((line_length > 0) && ( cur_line[0] == '#')) continue;

      if (!strncmp(cur_line,"DocumentRoot", strlen("DocumentRoot")))
        {
           _apache_doc_root =  cur_line + strlen("DocumentRoot ");
          _apache_doc_root.erase(remove( _apache_doc_root.begin(), _apache_doc_root.end(), '\"' ), _apache_doc_root.end());
          _apache_doc_root.erase(std::remove( _apache_doc_root.begin(), _apache_doc_root.end(), '\n' ), _apache_doc_root.end());
          if (_apache_doc_root[_apache_doc_root.length()-1] != '/')
            _apache_doc_root.push_back('/');


          return 0;
        }
    }

  /* no suitable tag in apache config file
     I should probably return a defult dir in this case
     but we return error for now
  */
  fprintf(stderr, "DocumentRoot isn't specified in apache config file");
  return -1;

}
