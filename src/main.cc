#include <iostream>
#include <fstream> 
#include <string>
#include <stdio.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

#define DELETE_METHOD = delete

#include "util.h"
#include "curl_util.h"
#include "crypt.h"
#include "payload_server.h"
#include "apache_payload_server.h"
#include "payload_scraper.h"

int main()
{
  PayloadScraper my_scraper("test.txt");
  my_scraper.scrape();

  cout << "done";

  return 0;

}
