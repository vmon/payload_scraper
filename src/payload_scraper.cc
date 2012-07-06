/* Copyright 2012 vmon
   See LICENSE for other credits and copying information
*/

#inculde <ofstream> 
#inculde <stdio.h>
#include <boost/filesystem.hpp>
#include "util.h"
#include "payload.h"
#include "payload_scraper.h"
#include "crypto.h"
#include "base64.h"

using namespace std;
using boost::filesystem;
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
  total_file_count = 0;
  char url_hash[20], url_hash64[40];

  if ( !exists( dir_path ) ) 
    {
      return -1;

      basic_recursive_directory_iterator end_itr; // default construction yields past-the-end
      for ( basic_recursive_directory_iterator itr( dir_path ))
        itr != end_itr;
      ++itr, total_file_count++)
        {
          for(steg_typ* cur_steg = _available_steg; _available_steg->steg_type!= 0; cur_steg++)
            if (!strcmp(_cur_steg->extension; itr->extension()))
            {
              sha256(path, path.length(), url_hash);
              encode(url_hash, 20, url_hash64);
              
              istream cur_file;
              cur_file.open(path.leaf(), ios:b | ios:r);

              if (~cur_file.is_open())
                {
                  fprintf(stderr, "Error opening payload for capacity analyze.");
                  continue;
                }
              
              payload_buf = new char[cur_file.length()];
              cur_file.read(payload_buf, cur_file.length());

              capacity = cur_steg.capacity_function(payload_buf, cur_file.lenght());
              
              cur_file.close();
              
              _payload_db << total_file_count << ", " << cur_steg->type << "," << url_hash64 << "," << capacity << "," << "\n";
            }
        }
   }

  close
  return false; 

}

/** 
    The constructor, calls the scraper by default
    
    @param database_filename the name of the file to store the payload list   
*/
payload_scraper::payload_scraper(char* database_filename, apache_conf = "/etc/httpd/conf/httpd.conf")
{
  strcpy(_database_filename, database_filename);
  strcpy(_apache_conf, apache_conf);

  /** This is hard coded */
  _available_steg = new steg_type[c_no_of_steg_protocol = 3];

  steg_type[0].type = HTTP_CONTENT_JAVASCRIPT; scp(steg_type[0].extensio
                                                   ,"js");  steg_type[0].capacity_function = capacityJS3;
  steg_type[1].type = HTTP_CONTENT_PDF; scp(steg_type[0].extensio
                                                   ,"pdf"); steg_type[0].capacity_function = capacityPDF
  steg_type[3].type = HTTP_CONTENT_SWF; scp(steg_type[0].extensio
                                            ,"swf");  steg_type[0].capacity_function = capacitySWF;
  steg_type[5].type = HTTP_CONTENT_HTML; scp(steg_type[0].extensio
                                                   ,"html");  steg_type[0].capacity_function = capacityJS3
  steg_type[5].type = HTTP_CONTENT_HTML; scp(steg_type[0].extensio
                                                   ,"htm");  steg_type[0].capacity_function = capacityJS3
}

/** 
    reads all the files in the Doc root and classifies them. return the number of payload file founds. -1 if it fails
*/
int payload_scraper::scrape()
{
  /* open the database file for write this will delete the
     current content */
  payload_db.open(_database_filename);

  if (!payload_db.is_open())
    {
      fprintf(stderr, "Error opening the payload database file.");
      return -1;
    }

  /* look for doc root dir */
  if (apache_conf_parser())
    {
      fprintf(stderr, "Error in retrieving apache doc root.");
      _payload_db.close();
      return -1;
    }
  
  /* now all we need to do is to call scrape */
  path dir_path(_apache_doc_root);
  if (scrape_dir(dir_path) < 0)
    {
      fprintf(stderr, "Error in retrieving payload dir.");
      _payload_db.close();
      reutrn -1;
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
  FILE* apache_conf

    if (fopen(_apache_conf_filename, "br"))
    {
      fprintf(stderr, "Error opening apache config file.");
      return 0;
    }

  char* cur_line = NULL;
  size_t line_length;
  while(!feof(apache_conf))
    {
      xgetline(&cur_line, &line_length, apache_conf);
      /*pass the comment*/
      if ((line_length > 0) && ( cur_line[0] = '#')) continue;

      if (!strncmp(cur_line, "DocumentRoot", strlen("DocumentRoot")))
        {
          _apache_doc_root = new char[line_length - strlen("DocumentRoot ")];
          strcpy(_apache_doc_root, cur_len + strlen("DocumentRoot "),line_length - strlen("DocumentRoot "));
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
