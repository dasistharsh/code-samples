
#include "RoutingClient.h"

pair<int,int> StartClient( long long timestamp , string mode , string controllerId)
{
  int ClientPriority = 1;
	long long timestamp_g_client    = timestamp;
	string mode_g_client         = mode;
	string controllerId_g_client = controllerId;

	int CounterWhichEnsureServerIsAlive = 0; 

  // creates the client socket and initialises it.
  SocketClient client_obj;
	
	cout << "[*] Trying to connect to : "<<EntireDomainNameToConnectTo<<endl;
  // It connects to server and handles name resolution internally.
  if (client_obj.connectToServer(EntireDomainNameToConnectTo ,PORT) == -1)
  {
    // It could be that SERVER_NOT_FOUND means the same.
    return pair<int,int>(COULD_NOT_CONNECT_TO_SERVER , ClientPriority);
  }
	else
  {
		//    ConnectedToServer = true;
		cout <<"[*] Connected to server"<<endl;
	}

  signed int lengthOfDataRecvdFromServer;
  
  // Some parameters for select 
	fd_set readmask;   
  struct timeval timeout;
  int selectFinish = 0;
	while (!selectFinish)
	{
		/* Below 4 instructions must be in the loop as select forgets everything after 1 iteration.*/
		// Clear the FD set to monitor.
		FD_ZERO(&readmask);   
		// Add FD we want to monitor to the FD set.
		FD_SET(client_obj.getFileDescriptor(), &readmask);

		// Select blocking call would be interrupted after this timeout whether or not data is there on the socket to read.   
		// Timeout value in seconds and milliseconds.
		timeout.tv_sec=TIME_TO_SEND_DATA_TO_SERVER;
		timeout.tv_usec=0;

		// select(MaxFds to Monitor + 1,FdsToMonitorForRead,FdsToMonitorForWrite,FdsToMonitorForExceptions)
		int retValue = select(client_obj.getFileDescriptor()+1, &readmask, NULL, NULL, &timeout);
		if ( retValue==RETURN_FAIL)
		{
			std::perror("[#] Error in select : ");
			exit(RETURN_FAIL);
		}

		// If this FD is set that means something arrived on the FD.
		if (FD_ISSET(client_obj.getFileDescriptor(), &readmask))
		{
      string Data;
      lengthOfDataRecvdFromServer = Socket::recv_data(client_obj.getFileDescriptor(), Data);
      if (lengthOfDataRecvdFromServer == RETURN_FAIL)		
      {
		  	// Decrement of CounterWhichEnsureServerIsAlive should be done here too
		  	// CounterWhichEnsureServerIsAlive --;
		  	std::perror("[#] Error in data recvd : ");
		  }
		  else if (lengthOfDataRecvdFromServer == 0)		
      {
		  	// Server broke connection
		  	selectFinish = 1;
		  }
		  else
      {
  	  	// Set the counter(which signify that the server has responded) to 0 instead of decrementing as we are cool with server responding once in 6 client messages. 
	    	CounterWhichEnsureServerIsAlive = 0;
		  	cout << "[*] Data recieved from Server : "<<Data<<endl;
		  	Parser obj(Data);

		  	if ( !obj.getFormatIsCorrect() )		
        {
		  		cerr << "[#] As you can see it is not in correct format, thus I am ignoring it and I will continue to send data to server"<<endl;
		  		cerr << "[#] Setting my priority to 100 "<<endl;
		  		ClientPriority = 100;
		  	}

		  	else		
        {
		  		cout << "[*] My Priority :  "                           << obj.getPriorityOrNs() <<  endl;
		  		cout << "[*] Server(routing.conf) has timestamp : "     << obj.getTimestamp()    <<  endl;
		  		cout << "[*] Server(routing.conf) has mode : "          << obj.getMode()         <<  endl;
		  		cout << "[*] Server(routing.conf) has controllerId : "  << obj.getControllerId() <<  endl;
		  		
          // Setting the priority of client as recieved from the server.
		  		ClientPriority = stoi(obj.getPriorityOrNs());

		  		long long ts = obj.getTimestamp();
		  		if ( !::helper::general_helper::verifyEverythingIsGood(ts, timestamp_g_client) )  
          {
		  			cout << "[*] I am running on older config, updating my configuration and restarting."<<endl;
		  			// UpdateConfig&Restart -> simply call updateConfig.sh with necessary arguments.
		  			string callthis = CONFIG_UPDATE_SCRIPT + std::to_string(obj.getTimestamp()) + " " + obj.getMode() + " " + obj.getControllerId();
		  			if (system(callthis.c_str()) == 0 )
            {
              return pair<int,int>(WAS_RUNNING_OLDER_CONFIG , ClientPriority);            
            } 
		  		}

		  	}     
			}
		}

		// Timeout occured and now the client wants to send some data to the server.
		if ( retValue == 0)
		{
			// Before sending verify that whether or not the server is up, by checking whether or not the server respnded to the last 5 requests
			// if the server didnt responded that means server is dead
			if ( CounterWhichEnsureServerIsAlive > MESSAGE_COUNT_IN_WHICH_SERVER_SHOULD_RESOND )
      {
				// Server is dead
				cout <<"[*] Server didnt respond for last "<<CounterWhichEnsureServerIsAlive<<" messages which means it must be dead."<<endl;
				return pair<int,int>(SERVER_DEAD , ClientPriority);
			}
			string myNs             = ::helper::general_helper::getNetworkStrength(GET_NETWORK_STRENGTH);
			string sendThisToServer = ::helper::general_helper::formPacket(myNs , timestamp , string(mode) , string(controllerId));
			
			if ( !Socket::send_data(client_obj.getFileDescriptor() , sendThisToServer) ) 
      {
				std::perror("[#] Error in sending data ");
				exit(RETURN_FAIL);
			}
			
      // Incrementing the counter to tell that a message is sent to server.
			CounterWhichEnsureServerIsAlive++; 
			cout<<"[*] Sent to server : "<<sendThisToServer<<endl; 
		}

	}  // Loop end

	cout<<"[*] Server broke the connection"<<endl;
	return pair<int,int>(SERVER_DEAD , ClientPriority);
}
