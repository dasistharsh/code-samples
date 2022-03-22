
#include "ClientPriority.h"    

    /*
    * The Structure of m_clientPriorityMap is as : 
    * itr->first (Points to integer)  : Key   (Integer)       : Client FileDescriptor
    * itr->first (Points to *integer) : Value (Integer Array) : {NetworkStrength , Priority , FlagRequiredInSortingHashmap}
    */

  ClientPriorityClass::ClientPriorityClass()
  {
    m_clientPriorityMap = new std::unordered_map<int, int *>; 
    cout << "[*] ClientPriorityClass Object Created"<<endl;
  }

  ClientPriorityClass::~ClientPriorityClass()
  {
    for (auto itr = m_clientPriorityMap->begin(); itr != m_clientPriorityMap->end(); itr ++)
      eraseFromMap(itr->first);   // Pass the FD i.e the key of the key-value pair.

    delete m_clientPriorityMap; 
    cout << "[*] ClientPriorityClass Object destructed"<<endl;
  }


  // Return by reference the m_clientPriorityMap.
  std::unordered_map<int , int*> & ClientPriorityClass::getMap() 
  {
    return *m_clientPriorityMap;   // Return the map-object, which is at (*m_clientPriorityMap) and not at(m_clientPriorityMap).
  }

  
  void ClientPriorityClass::eraseFromMap(int key)    
  { 
   if (m_clientPriorityMap->count(key))
    { 
      // Could have also deleted the int* which is the value corresponding to every key inthe map, deleting the entire pair here , so no worries.
      m_clientPriorityMap->erase(key); 
    }
    // Doesnt matter
    // else  cout<<"[*] Erase Map " << key << " element not found"<<endl;
    return;
  }


  void ClientPriorityClass::printMap()
  {
    cout << "[*] Device  \t\tNetworkStrength\t\t"<<"Priority"<<endl;
    for(auto itr = m_clientPriorityMap->begin(); itr != m_clientPriorityMap->end(); itr++)    
    {
  
      // +1 in 'itr->second[1]' is done in order to start the priority from 1 instead of 0 & keep 0 for manual override functionality.
      // here it is done so as to maintain syncronicity with what is sent and what is printed
      int socketFd = itr->first;
      string clientName = Socket::getNameFromFD(socketFd);
      string clientDetailsPrint = "[*] " ;
      if (clientName.length()==0)
        clientDetailsPrint = clientDetailsPrint + "FD : " + std::to_string(socketFd) ;
      else
        clientDetailsPrint += "" + clientName; 
      cout<<clientDetailsPrint<< "\t\t"<< itr->second[0]<<"\t\t\t"<<itr->second[1]+1<<"\t\t\t"<<endl;  
    }
    cout<<endl;
    return;
  }


  void ClientPriorityClass::SortMap()    
  {
    std::vector<int> *sortingVector = new std::vector<int>;

    // Push network strength of all clients in std::vector.
    for (auto itr = m_clientPriorityMap->begin(); itr != m_clientPriorityMap->end(); itr++)
    {
      // cout <<"Pushed into std::vector ->"<<itr->second[0]<<endl;
      sortingVector->push_back(itr->second[0]);
    }

    // sort std::vector in descending order
    std :: sort(sortingVector->begin() , sortingVector->end() , std::greater<int>());

    cout<<endl;

    for (  unsigned int i = 0 ; i < sortingVector->size(); i++)    
    {
      for (auto itr = m_clientPriorityMap->begin() ; itr != m_clientPriorityMap->end(); itr++)    
      {
        if ( (itr->second[0] == sortingVector->at(i) ) && ( itr->second[2] == 0) )    
        {
          // Priority of that client is equal to the index of it's NetworkStrength in the Sorted Array.
          itr->second[1] = i;
          itr->second[2] = 1;
          // cout << itr->first<<"(Key) : " <<itr->second[0]<<","<<itr->second[1]<<","<<itr->second[2]<<endl;
          break;
        }
      }
    }
    delete sortingVector;
    return;
  }

  void ClientPriorityClass::InsertInHashmap(int key, int NetworkStrength)   
  {
    // Array elements are ->  NetworkStrength , DefaultPriority , FlagRequiredInSortingHashmap
    int *array = new int[3]{NetworkStrength , -1 , 0 };
    (*m_clientPriorityMap)[key] = array;
  }
