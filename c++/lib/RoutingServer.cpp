
#include "RoutingServer.h"

/*
* Toggle back the value of FlagRequiredInSortingHashmap as well as send the priority to the clients.
* The last argument(default) in the function i.e. 'configUpdate', tells that a client has newer info which it sent to the server and now the server is propogating that 
* info to all other clients , and in that case the priority of client doesn't matter, as it's going to restart anyway.
*/
void sendPriorityToClient( std::unordered_map<int , int*> &clientPriorityMap , long long timestamp_g_server, string mode_g_server,string controllerId_g_server , int configUpdate = 0)
{
  if (configUpdate)
  {
    cout << "[*] SendPriorityToClient() : A Client had newer info than server, propogating it to rest of the clients before updating my(server's) config"<<endl;
  }
  for (auto itr = clientPriorityMap.begin() ; itr != clientPriorityMap.end(); itr++)    
  { 
    itr->second[2] = 0;
    // +1 is done in order to start the priority from 1 instead of 0 & keep 0 for manual override functionality(although not reqd in Routing.service V2).
    string clientPriority = std::to_string(itr->second[1]+1);
    string sendBack       = helper::general_helper::formPacket(clientPriority, timestamp_g_server, mode_g_server, controllerId_g_server);
    
    // Sending back the priorities to the clients  in the proper format.
    if ( !Socket::send_data(itr->first , sendBack) )
    {
      perror("[#] Error in sending back priority to client(ignore me) ");
      cout<<endl;
    }
  }
  return;
}

int StartServer( long long timestamp , string mode , string controllerId)   
{
  long long timestamp_g_server   = timestamp;
  string mode_g_server           = mode;
  string controllerId_g_server   = controllerId;
  

  // creates the server socket and initialises it.
  SocketServer server_obj;

  // Set up socket options  
  if (server_obj.setSocketOptions() == -1)    
  {
    return(RETURN_FAIL);
  }

  // Bind the socket to the port
  if (server_obj.socket_bind(PORT) == -1)     
  {
    return 1;
  }

  // make the Server Socket nonblocking.
  SocketServer::set_nonblocking(server_obj.getFileDescriptor());

  if (server_obj.socket_listen() == -1)    
  {
    return(RETURN_FAIL);
  }

  cout<<"[*] Listening on : "<<PORT<<endl;

  // ::helper::general_helper::checkIAmTheOnlyServer(CHECK_I_AM_THE_ONLY_SERVER_SCRIPT,EntireDomainNameToConnectTo,NUMBER_OF_TIMES_BEFORE_RETURNING_FALSE) returns 0 if ServerIsRunningInThisDevice , else it returns non-zero value
  if ( helper::general_helper::checkIAmTheOnlyServer( CHECK_I_AM_THE_ONLY_SERVER_SCRIPT ,EntireDomainNameToConnectTo , NUMBER_OF_TIMES_BEFORE_RETURNING_FALSE) )   
  {
    //cout << "[#] Seems as if server is already running on some other device." <<endl;
    cerr << "[#] Either someone is already running or I am not configured properly." <<endl;
    return (SERVER_ALREADY_PRESENT);
  }
  else
  {
    cout << "[*] I am the only one running the server" << endl;
  }

  // create the epoll socket , argument passed is the flag values
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1)   
  {
    perror("[#] epoll_create1() ");
    return(RETURN_FAIL);
  }

  // mark the server socket for reading, and become edge-triggered
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = server_obj.getFileDescriptor();

  // Monitor for events which are either Edge Triggered or Input to socket 
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_obj.getFileDescriptor(), &event) == -1)     
  {
    perror("[#] epoll_ctl() ");
    return 1;
  }

  // Assign memory for the events.
  struct epoll_event *events = (epoll_event *) calloc(MAXEVENTS, sizeof(event));
  int t = (int) time(NULL);
  bool jouleboxRunning = false;
  
  // Object of ClientPriority class.
  ClientPriorityClass clientMap;

  //Event Loop
  while(1)    
  {
    // timeout = 0 means that it wont block.
    int nevents = epoll_wait(epoll_fd, events, MAXEVENTS, 0);

    // Since timeout is 0, hence must add some sleep so that cpu can rest ==> THIS SLEEP HIGHLY EFFECTS CPU USAGE (inversely proportional). 
    // Keep in mind that sleep is blocking hence, while sleeping if any request or anything comes, you wont be able to access it
    // sleep of 1 is same as setting the value of timeout to 1.         
    sleep(1);

    if (( (int)time(NULL) - t ) >= TIME_TO_SEND_DATA_TO_CLIENTS)   
    {
      
      // Just a check to ensure that this device is the only server (don't remove).
      if (helper::general_helper::checkIAmTheOnlyServer(CHECK_I_AM_THE_ONLY_SERVER_SCRIPT , EntireDomainNameToConnectTo , NUMBER_OF_TIMES_BEFORE_RETURNING_FALSE) == 0)    
      {

        cout<<"[*] \tSending Priorities to resp. clients";        
        clientMap.SortMap();
        clientMap.printMap();

        // After the client map has been sorted with their respected Priorty as per their NetworkStrength.
        sendPriorityToClient(clientMap.getMap() , timestamp , mode , controllerId , 0);
        t = (int)time(NULL);
      }

      else
      {
        cerr <<"[#] Seems like someone else has started running the server"<<endl;	
        return (SERVER_ALREADY_PRESENT);
      }

      // Start joulebox after 1 minute.
      if (!jouleboxRunning)   
      {

        // Time to Start Joulebox
        cout <<"[*] Starting JouleboxContainer"<<endl;
        string jbStart = START_JOULEBOX_SCRIPT + EntireDomainNameToConnectTo;
        while(system(jbStart.c_str()));
        cout <<"[*] Started JouleboxContainer"<<endl;
        jouleboxRunning = true;
      }
    }

    // Epoll fucked up
    if (nevents == -1)    
    {
      perror("[#] Error in epoll_wait(), returned -1 ");
      return 1;
    }

    // Iterate through all the events that occured 
    for (int i = 0; i < nevents; i++)    
    {
      // EPOLLERR: error on socket , EPOLLHUP: remote end disconnected , EPOLLIN: socket is available for reading
      if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))    
      {
        // Error in socket
        perror("[#] Error  in client ");
        //cout << "[#] on event : "<<events[i].events<<endl;

        // Close the connection and remove the FDs from Hashmap.
        Socket::close_socket(events[i].data.fd);
        clientMap.eraseFromMap(events[i].data.fd);
        continue;
      }
      // Event occured on the server socket FD, therefore must be a new connection. 
      else if (events[i].data.fd == server_obj.getFileDescriptor())     
      {
        // Accept connection from clients trying to connect.
        while(1)    
        {
          int client = server_obj.socket_accept();

          if (client == -1) 
          {
            
            // These 2 errors has to be checked bcoz of Non-Blocking nature
            if (errno == EAGAIN || errno == EWOULDBLOCK)    
            {
              //Processed all the connections
              break;
            }
            else      
            {
              perror("[#] accept() ");
              return 1;
            }
          } 

          else
          {
            cout<<"[*] Accepted new connection on fd : "<< client <<endl;
            SocketServer::set_nonblocking(client);
            // fill the event structure with client FDs and events to monitor 
            event.data.fd = client;
            event.events = EPOLLIN | EPOLLET;
            // Add to epoll
            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1)       
            {
              perror("[#] epoll_ctl() ");
              return 1;
            }
          }
        }
      }
      else
      {
        // Read all the data from client -> As we can only access it once.
        while(1)
        {
          string Data;
          ssize_t sizeOfDataFromSocket = Socket::recv_data(events[i].data.fd , Data);
          if (sizeOfDataFromSocket == -1)
          {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
              // cout<<"Done reading data from client"<<endl;
              break;
            } 
            else 
            {
              perror("[#] read() ");
              return 1;
            }
          } 
        // As we know that an event has occured on this FD, then the client must be sending something & if lenght(DataByClient) is 0, it means client died.
          else if (sizeOfDataFromSocket == 0) 
          {
            cout<<"[*] Client on FD:"<< events[i].data.fd<<" disconnected" <<endl;
            Socket::close_socket(events[i].data.fd);
            clientMap.eraseFromMap(events[i].data.fd);
            break;
          } 
          else 
          {
            // Print on console what client send would be as : 99,123,automatic,0 
            cout<<"[*] Data recvd from client "<<events[i].data.fd <<"  : "<<Data<<endl;
            Parser parserObj(Data);
            // Check to ensure that client sent in proper format, if status is False then error occured.
            if (parserObj.getFormatIsCorrect())    
            {
              long long ts = parserObj.getTimestamp();
              cout << "[*] NetworkStrength recieved  : "         << parserObj.getPriorityOrNs()<<endl;
              cout << "[*] timestamp recieved from client   : "  << parserObj.getTimestamp() <<endl;
              cout << "[*] mode recieved from client : "         << parserObj.getMode() <<endl;
              cout << "[*] controllerId recieved from client : " << parserObj.getControllerId() <<endl;            

              // Before inserting in hashmap just verify that the timestamp recieved from data packet is same or less than what is in server config file.  
              if (helper::general_helper::verifyEverythingIsGood(ts , timestamp_g_server) == 0)   
              {
                // Need to update my config
                cout << "[*] This client was running on config newer than mine, changing my config file and then will restart"<<endl;
                // UpdateConfig&Restart -> simply call updateConfig.sh with necessary arguments.
                string callthis = CONFIG_UPDATE_SCRIPT + std::to_string(parserObj.getTimestamp()) + " " + parserObj.getMode() + " " + parserObj.getControllerId();
                
                // Before updating the server's configuration , server should propogate this newer configuration to all it's client , so that entire network can be updated at once
                // and not one controller at a time , which earlier used to happen.
                sendPriorityToClient(clientMap.getMap() , parserObj.getTimestamp(), parserObj.getMode(), parserObj.getControllerId() , 1);

                Socket::close_socket(server_obj.getFileDescriptor());
                if (system(callthis.c_str()) == 0 ) 
                {
                  return WAS_RUNNING_OLDER_CONFIG;
                }
              }

              clientMap.InsertInHashmap(events[i].data.fd , stoi(parserObj.getPriorityOrNs()) );
            }
            else
            {
              cerr << "[#] Data recieved from client not in proper format,sending it DefaultPriority of 100(in proper format,right now)"<<endl;
              string sendBack = helper::general_helper::formPacket(string("100"), timestamp_g_server, mode_g_server, controllerId_g_server);
              // Sending back the priorities to the clients  in the proper format.
              if ( !Socket::send_data(events[i].data.fd , sendBack) )   
              {
                perror("[#] Error in sending back priority to client(ignore me) ");
                cout<<endl;
              }

              // Also ensure , that this client's FD is not present in the Map to sort.
              clientMap.eraseFromMap(events[i].data.fd);
              cerr << "[#] Removed client FD from map" << endl;
            }
  
          }
  
        }
      }
    }
  }
  return SERVER_DEAD;
}
