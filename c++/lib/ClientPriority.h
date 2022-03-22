#include <unordered_map>        // for Hashmap 
#include <vector>               // for Vector
#include <algorithm>            // for sorting

#include "Socket.h" 

// SideNote : Wasn't able to name the class ClientPriority because it already exists in g++ resolution scope.

/** 
* 
*/

/**
* @file ClientPriority.h
* @brief
*   This file contains the 'ClientPriorityClass' class.
* @class ClientPriorityClass
* @brief
*    Class responsible for maintaining details regarding all the clients.
* @details
*   This class is the implementation of how the server maintains Priority of the client with regard to it's 
*   network strength and all the associated functions required for it.
*/
class ClientPriorityClass
{
  private:
    /*
    * The Structure of m_clientPriorityMap is as : 
    * Key   (Integer)       : Client FileDescriptor
    * Value (Integer Array) : {NetworkStrength , Priority , FlagRequiredInSortingHashmap}
    */
    std::unordered_map<int, int *> *m_clientPriorityMap; 

  public:
    /**
    * @brief
    *   Constructor of ClientPriority Class
    * @details
    *   Creates a dynamic storage for holding client details.
    */
    ClientPriorityClass();

    /**
    * @brief
    *   Destructor of ClientPriority Class
    * @details
    *   Free's the dynamic storage allocated by the constructor for holding client details.
    */
    // For free'ing the resources once the object is asked to be deleted.
    ~ClientPriorityClass();

    /**
    * @brief
    *   Getter function to get the Map, where the details are stored.
    * @return
    *    'std::unordered_map' : Return by reference the map.
    */
    std::unordered_map<int , int*> & getMap();


    /**
    * @brief
    *   Remove a client map. 
    * @details
    *   Remove from map the key-value pair where key= 'key'.
    * @params 'int key' : File-descriptor of client, whose deatils are to be removed.
    * @return
    *   'void'
    */
    void eraseFromMap(int key);

    /**
    * @brief
    *   Simply Prints the map.
    */
    void printMap();

    /**
    * @brief
    *   Sorts the Map.
    * @details
    *    Sorts the Map as per network strength of the clients, and sets their priority.
    * @param 'void'
    * @return
    *   'void'
    */
    void SortMap(); 
  
    /**
    * @brief
    *   Insert a new key-value pair of client in Map.
    * @details
    *   Create the 'Value' as per the structure which the map accepts for the client and inserts it in the map.
    * @return
    *   'void'
    */
    void InsertInHashmap(int key, int NetworkStrength);
};

