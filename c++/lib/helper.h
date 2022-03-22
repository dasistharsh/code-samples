
#include <stdlib.h>     // For calling system commands.  
#include <unistd.h>     // For sleep function
#include "Parser.h"

# pragma once   // Only compile once

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/** Constant required by RoutingClient and RoutingServer */
const std::string HostnameOfBroker            = "smartjoules";

/** Constant required by RoutingClient and RoutingServer */
const std::string tld                         = ".local";

/** Constant required by RoutingClient and RoutingServer */
const std::string EntireDomainNameToConnectTo = HostnameOfBroker + tld;  

/**
* @file helper.h
* @brief
*   This file contains the 'helper' namespace.
* @brief 
*   This is simply the decleration file i.e all the nested namespaces must have themselves registered and a line explaining there usage here. 
*   All include's should be done here only in the parent namespace directory and not the nested levels.
*/

// Creating a namespace for all helper functions.
namespace helper {

    /** @brief helper::general_helper Namespace
    *   @details
    *    This namespace consists of some general functions pertaining to either the business logic or being used anywhere else.
    *    For Ex : pingCheckFunction() i.e checking if there exists or not a server in the network. 
    */
  namespace general_helper  
  {
  }
 
}
