#pragma once 

#include <sys/select.h> // For select obv
#include <time.h>       // For random numbers -> testing only 
#include <utility>      // For pair class

#include "SocketClient.h"

using std::pair;


/**
* @file RoutingClient.h
* @brief
* The actual Event loop which is responsible for managing the data flow and connection with the server. 
*/

/* 
* @brief
*   The 'StartClient' function is responsible for handling the client connection.
* @details
*   This function handles the entire client side event loop , handling the things like connecting to server,
*    all the communication between client-server and everything in client side.
* @param long long timestamp Timestamp as per the configuration file.
* @param string mode Mode as per the configuration file.
* @param string controllerId ControllerId as per the configuration file.
* @return 
*   'Pair<return value , clientPriority>' : Returns the Pair object containing the return value as well as the clientPriority. 
*/
pair<int,int> StartClient( long long timestamp , string mode , string controllerId);
