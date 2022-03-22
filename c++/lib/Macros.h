
/**
* @file Macros.h
* @brief   
*   This file contains all the Macros being used in the RoutingService code.
*/

/*
 * Common Macros
 */

/** Port number of server                        */
#define PORT                                                9000      

/** Socket Buffer size                           */
#define BUFF_SIZE                                           100       

/** When something failed                        */
#define RETURN_FAIL                                         -1        

/** Server died due to some reason.              */
#define SERVER_DEAD                                         -3        

/** When this node is running on older config    */
#define WAS_RUNNING_OLDER_CONFIG                            -5        

/** Script to update config file                  */
#define CONFIG_UPDATE_SCRIPT                                "bash /home/pi/routingService/updateConfig.sh " 


/*
 * For RoutingServer.h
 */

/** Time interval in which server will send data to all the connected clients(in seconds)   */
#define TIME_TO_SEND_DATA_TO_CLIENTS                         60       

/** Maximum number of events the server can listen/keep in backlog                          */
#define MAXEVENTS                                            1500     

/** Minimum times to check whether the server is alive or not                               */
#define NUMBER_OF_TIMES_BEFORE_RETURNING_FALSE               5        

/** When some other device has already started the server.                                  */
#define SERVER_ALREADY_PRESENT                               -2       

/** Script to check whether or not this controller is the running the server.               */
#define CHECK_I_AM_THE_ONLY_SERVER_SCRIPT                    "bash /home/pi/routingService/deps/checkIAmTheOnlyServer.sh "

/** Script to change hostname of the controller.                                            */
#define CHANGE_HOSTNAME_COMMAND                              "sudo bash /home/pi/routingService/deps/ChangeHostname.sh "

/** Script to start jouleboxContainer.                                                      */
#define START_JOULEBOX_SCRIPT                                "sudo bash /home/pi/routingService/deps/startJoulebox.sh "


/*
 * For RoutingClient.h
 */


/** Time interval in which clients will send data to the server(in seconds)   */
#define TIME_TO_SEND_DATA_TO_SERVER                           25      

/** It means that if the server doesn't even respond once in ( {TIME_TO_SEND_DATA_TO_SERVER}25 * 6 = 150)seconds then assume it's dead. */
#define MESSAGE_COUNT_IN_WHICH_SERVER_SHOULD_RESOND           6       

/** Unable to connect to server or server not found.                           */
#define COULD_NOT_CONNECT_TO_SERVER                           -4

/** Unable to connect to server or server not found.                           */
#define SERVER_NOT_FOUND                                      COULD_NOT_CONNECT_TO_SERVER 

/** Script to get networkStrength of this controller.                          */
#define GET_NETWORK_STRENGTH                                  "bash /home/pi/routingService/deps/networkStrength.sh "

/** Script to stop the server and reset the environment.                       */
#define SERVER_STOP_COMMAND                                   "sudo bash /home/pi/routingService/deps/ServerSetupStop.sh "


/**
 * NOTE : The Client sends it's networkStrength every 25seconds whereas the server responds every 60seconds, 
 * becuase server needs to assign priorities to the client.
 */
