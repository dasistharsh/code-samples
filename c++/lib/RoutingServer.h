#pragma once 

#include <sys/epoll.h>        // For epoll
#include <time.h>             // For epoch time time(NULL)

#include "ClientPriority.h"   // For Storing Client info like NetworkStrength , Priority etc.
#include "SocketServer.h"     // For Socket functionality

/**
* @file RoutingServer.h
* @brief
*   The actual Event loop which is responsible for managing the data flow and connection with the client. 
* @details
*   RoutingServer also handles assigning the clients their priorities , connecting with them keeping them updated regarding config 
*   and everything that the server has to do.
*/



/*
* @brief
*   Send the priority to the clients.
*
* @param 'unordered_map' : Hashmap Used to Store the clients and their details, priorities etc.
* @return
*   'void'
*/
void sendPriorityToClient( std::unordered_map<int , int*> &clientPriorityMap);

/*
* @brief
*   The actual Event loop which is responsible for managing the data flow and connections with client and assigning priorities. 
*
* @param 'long long timestamp' Timestamp as per the configuration file.
* @param 'string mode Mode' as per the configuration file.
* @param 'string controllerId' ControllerId as per the configuration file.
* @return 
*   'int' : Returns integer(from Macros) to the caller. 
*/
int StartServer( long long timestamp , string mode , string controllerId);
