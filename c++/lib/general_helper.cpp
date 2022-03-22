
#include "general_helper.h"

  string helper::general_helper::formPacket(string myNsOrPriority , long long timestamp , string mode ,string controllerId )
  {
    string ts = std::to_string(timestamp);
    string formattedString = myNsOrPriority + "," + ts + "," + mode + "," + controllerId;
    return formattedString; 
  }
  

  int helper::general_helper::verifyEverythingIsGood( long long timestamp_of_partner , long long my_timestamp)  
  {
    /*
    if the timestamp recived is same as mine then good.
    else if timestamp is greater than mine then update my config and restart Routing.Service.
    */
    //cout << "[*] Inside verifyEverythingIsGoodC (Client)"<<endl;
    if ( timestamp_of_partner > my_timestamp)     
      return 0; // Screwed up , time to update my config.
    else
      return 1; // Either my ts same as what client send or greater either way i am good.
  }


  string helper::general_helper::getNetworkStrength(string command_to_execute)
  {  
    // divide by 256 is necessary
    // cout << "calling system"<<endl;
    int ns = system(command_to_execute.c_str())/256;
    string ans = std::to_string(ns);
    return ans;
  } 


  int helper::general_helper::checkIAmTheOnlyServer(string scriptName , string EntireDomainNameToConnectTo , int minimum_tries)   
  {
  
    string command_to_execute = scriptName + EntireDomainNameToConnectTo;
  
    int  resp = -1; // check max of minimum_tries times as sometimes it returns ipv6 address too
    while(minimum_tries--)    {
      resp = system(command_to_execute.c_str())/256;
      if ( resp == 0)
        break;
    } 
    // cout <<"[*] Response recieved from checkIAmTheOnlyServer script : " << resp<<endl;
    // if resp == 0 means successfully executed i.e I am the only one running the server , otherwise it means either there is no server or someone else is running. 
    return resp;
  } 


  int helper::general_helper::pingCheckFunction(string pingCheckRequest, int NumberOfTimesBeforeReturningFalse)
  {
    int resp = 1;
    while (NumberOfTimesBeforeReturningFalse--)
    {
      resp = system(pingCheckRequest.c_str())/256;
      if ( resp == 0)   // ping passed.
        break;
      sleep(45); // Retry after 45 seconds;
    }
    return resp;
  }
