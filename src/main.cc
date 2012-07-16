#include <iostream>
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

int main()
{
  payload_scraper my_scraper("test.txt");
  my_scraper.scrape();

  return 0;

}
