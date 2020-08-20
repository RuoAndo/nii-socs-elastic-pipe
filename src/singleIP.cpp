#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <bitset>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <alloca.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <sys/resource.h>

#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/concurrent_vector.h"
#include "utility.h"
#include <boost/algorithm/string.hpp>

#include "csv.hpp"
#include "timer.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace tbb;

std::vector<std::string> split_string_2(std::string str, char del) {
  int first = 0;
  int last = str.find_first_of(del);

  std::vector<std::string> result;

  while (first < str.size()) {
    std::string subStr(str, first, last - first);

    result.push_back(subStr);

    first = last + 1;
    last = str.find_first_of(del, first);

    if (last == std::string::npos) {
      last = str.size();
    }
  }

  return result;
}

/*
std::string now_str()
{

  const boost::posix_time::ptime now =  boost::posix_time::microsec_clock::local_time();

  const boost::posix_time::time_duration td = now.time_of_day();

  const long hours        = td.hours();
  const long minutes      = td.minutes();
  const long seconds      = td.seconds();
  const long milliseconds = td.total_milliseconds() -
    ((hours * 3600 + minutes * 60 + seconds) * 1000);

  char buf[40];
  sprintf(buf, "%02ld:%02ld:%02ld.%03ld",
	  hours, minutes, seconds, milliseconds);

  return buf;
}
*/


int traverse_file(char* IPAddr, char* filelist_name) {


  int counter = 0;
  int addr_counter = 0;
  
  int netmask;
  
  try {

    const string list_file = string(filelist_name); 
    vector<vector<string>> list_data; 
   
    try {
      Csv objCsv(list_file);
      if (!objCsv.getCsv(list_data)) {
	cout << "read ERROR" << endl;
	return 1;
      }
    }
    catch (...) {
      cout << "EXCEPTION (session)" << endl;
      return 1;
    }
  
  
    counter = 0;
    for (unsigned int row = 0; row < list_data.size(); row++) {
      
      vector<string> rec = list_data[row];
      const string argIP = rec[0]; 
      std::string argIPstring;
      
      netmask = atoi(rec[1].c_str());
      // cout << "netmask:" << netmask << endl;
      
      /*
      std::cout << "[" << now_str() << "]" << "threadID:" << thread_id << ":" << list_file << ":" << addr_counter << "(" << list_data.size() << "):"
		<< argIP << "/" << netmask << ":" << filename << ":" << ingress_counter_global << ":" << egress_counter_global
		<< ":" << miss_counter << std::endl;
      */
	    
      char del2 = '.';
	    
      for (const auto subStr : split_string_2(argIP, del2)) {
	unsigned long ipaddr_src;
	ipaddr_src = atol(subStr.c_str());
	std::bitset<8> trans =  std::bitset<8>(ipaddr_src);
	std::string trans_string = trans.to_string();
	      argIPstring = argIPstring + trans_string;
      }
	    
      std::string srcIP = string(IPAddr);
      // cout << srcIP << endl;
      
      for(size_t c = srcIP.find_first_of("\""); c != string::npos; c = c = srcIP.find_first_of("\"")){
	srcIP.erase(c,1);
      }
	
      std::string sessionIPstring;
      for (const auto subStr : split_string_2(srcIP, del2)) {
	unsigned long ipaddr_src;
	ipaddr_src = atoi(subStr.c_str());
	// cout << ipaddr_src << endl;
	std::bitset<8> trans =  std::bitset<8>(ipaddr_src);
	// cout << trans << endl;
	std::string trans_string = trans.to_string();
	sessionIPstring = sessionIPstring + trans_string;
	// cout << sessionIPstring << endl;
      }
		
      std::bitset<32> bit_argIP(argIPstring);
      std::bitset<32> bit_sessionIP(sessionIPstring);
	
      std::bitset<32> trans2(0xFFFFFFFF);
      trans2 <<= (32 - netmask);
      bit_sessionIP &= trans2;

      /*
      cout << trans2 << endl;
      cout << bit_sessionIP << endl;
      cout << bit_argIP << endl;
      */
      
      if(bit_sessionIP == bit_argIP)
	{
	  cout << "HIT: " << srcIP << " <- " << rec[0] << "/" << rec[1] << endl;
	}

    } // for (unsigned int row = 0; row < list_data.size(); row++) {     
  
    return 0;
  }
    
  catch(std::exception& e) {
    std::cerr<<"error occurred. error text is :\"" <<e.what()<<"\"\n";
  }

}

int main(int argc, char* argv[]) {


    if (argc != 3) {
        printf("Usage: ./singleIP [IPAddr] [LIST_FILE_NAME] \n"); return 0;
    }
  
    traverse_file(argv[1], argv[2]); 
  
    return 0;
}
