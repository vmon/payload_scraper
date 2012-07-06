/* Copyright 2012 vmon
   See LICENSE for other credits and copying information
*/

#ifndef PAYLOAD_SCRAPER_H
#define PAYLOAD_SCRAPER_H

#endif


struct steg_type
{
   int type;
   char extension[10];
};

/** 
    We read the /etc/httpd/conf/httpd.conf (this need to be more dynamic)
    but I'm testing it on my system which is running arch) find
    the DocumentRoot. Then it will check the directory recursively and
    gather the name of all files of pdf, swf and js type and store them
    in a database file.
*/

class payload_scraper
{
protected:

    static const int _c_no_of_steg_protocol = 5;
    char* _database_filename;
    fostream _payload_db = NULL;
    steg_type* _available_stegs = NULL;
p    
    char* _apache_conf_filename = NULL;
    char* _apache_doc_root = NULL; /* the directory that apache serve where
                               the html doc*/
    
    /* 
       Scrapes current directory, recursively calls itself for
       for subdirs, return number of payload if successful -1 
       if it fails.

       @param cur_dir the name of the dir to be scraped
     */
    int scrape_dir(char* cur_dir);

   /** 
       open the apache configuration file, search for DocumentRoot
       and set the 

   */
   int apache_conf_parser();

public:

   /** 
      The constructor, calls the scraper by default

      @param database_filename the name of the file to store the payload list   
    */
   payload_scraper(char* database_filename,  apache_conf = "/etc/httpd/conf/httpd.conf");

   /** reads all the files in the Doc root and classifies them. return the number of payload file founds. -1 if it fails
   */
   int scrape();

}
