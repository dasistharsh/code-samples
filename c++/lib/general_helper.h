#pragma once  // Only compile once

#include "helper.h"


/**
* @file general_helper.h
* @brief
*   This file contains the 'helper::general_helper' namespace.
* @brief 
*   This file contains the functions present in the nested 'helper::general_helper' namespace.
*/
namespace helper 
{
  namespace general_helper 
  {

    /**
    * @brief
    *   Generates the Packet.
    * @details
    *   This function forms a packet from the arguments in a format that could be sent over socket and the node at 
    *   other end could easily parse/understand it. 
    *   @param  'string myNsOrPriority' 
    *   @param  'long long timestamp'   
    *   @param  'string mode'           
    *   @param  'string controllerId'   
    * @return
    *   'string' : The generated packet.
    */
    string formPacket(string myNsOrPriority , long long timestamp , string mode ,string controllerId );

    /**
    * @brief
    *   Fetch NetworkStrength
    * @details  
    *   Execute the necessary command to get the networkStrength of this controller and returns it in stringified format.
    * @param  'string command_to_execute'
    * @return
    *   'string' : The Network Strength.  
    */
    string getNetworkStrength(string command_to_execute);

    /**
    * @brief
    *   Compare who has the latest configuration(in routing.conf).
    * @details
    *   This function checks if the timestamp(in routing.conf) of this node is greater than or smaller than equal to the timestamp(in routing.conf) 
    *   of it's partner (i.e it's client or server).
    * @param 'long long timestamp_of_partner' 
    * @param 'long long my_timestamp'
    * @return
    *   'int'  : 0 if timestamp(in routing.conf) of this controller is smaller, else 1.
    */
    int verifyEverythingIsGood( long long timestamp_of_partner , long long my_timestamp);

    /**
    * @brief
    *   Checks if there is any other server in the network.
    * @details
    *   This function checks whether or not there already exists a node running the server and that I am the only node in the network who is acting as the server. 
    * @param 'string scriptName' : Script which will is actually responsible. 
    * @aram  'string EntireDomainNameToConnectTo' : Domain name of the server
    * @param 'int minimum_tries' : Maximum retries applicable.
    * @return
    *   'int' : 0 if this controller is the only server in the network, else 1.
    */
    int checkIAmTheOnlyServer(string scriptName , string EntireDomainNameToConnectTo , int minimum_tries);


    /**
    * @brief
    *   Ping Server.
    * @details
    *   This function tries to pings the server in order to check the presence of server in the network.
    * @param 'string pingCheckRequest' : stringified command to execute in order to ping.
    * @param 'int NumberOfTimesBeforeReturningFalse' : Upper limit on number of times to try.
    * @return
    *     'int' : 0 if server exists, else 1.
    */
    int pingCheckFunction(string pingCheckRequest, int NumberOfTimesBeforeReturningFalse);   

  }

}
