# pragma once

#include <iostream>  // For C++ I/O
#include <string.h>  // for strtok
#include <string>    // for stoll 

using std::string;


/**
* @file Parser.h
* @brief
*   This file contains the Parser class.
* @class Parser 
* @brief
*    Class responsible for parsing the data being shared b/w client-server over sockets.
*/
class Parser
{

  private:
    string m_mode;            /* string m_mode                         : 'automatic' or 'manual'                            */
    string m_priorityOrNs;    /* string m_priorityOrNs                 : priorityOrNetworkStrength based on what is passed. */
    string m_controllerId;    /* string m_controllerId                 : valid controllerId or "0"(when mode is automatic)  */
    long long m_timestamp;    /* long long m_timestamp possible values : Non-decimal integer value (epoch time)             */
    bool m_formatIsCorrect;   /* Will be True if all the values are as per format. Just for this purpose                    */
  
  public:

    /** 
    * @brief 
    *   Constructor of Parser Class.
    * @details
    *   Parse the string and fill the members of the object.
    * @param 
    *   'string entireString' : The string to parse using ',' as delimiter.
    */
    Parser(string);             


    /** 
    * @brief 
    *   Constructor of Parser Class.
    * @details
    *   Parse the string and fill the members of the object(seperate constructor so that no implicit type-casting is required).
    * @param 
    *   'char* entireString' : The string to parse using ',' as delimiter.
    */
    Parser(char *);             // Just for support character pointer


    /**
    * @brief
    *   Parse the string.
    * @details
    *   Function which is actually responsible for parsing the string, called from the constructor itself.
    * @param
    *   'string entireString' : The string to parse using ',' as delimiter.
    * @return
    *   void
    */
    void parsedString(string);  


    /**
    * @brief
    *   Getter function to fetch 'timestamp' from the object.
    * @param
    *   void
    * @return
    *    long long Timestamp
    */
    long long getTimestamp();
    

    /**
    * @brief
    *   Function which tells whether or not the string was successfully parsed. 
    * @param
    *   void
    * @return
    *    bool
    */
    bool getFormatIsCorrect() ;
    

    /**
    * @brief
    *   Getter function to fetch 'Mode' from the object.
    * @param
    *   void
    * @return 
    *   string Mode 
    */
    string getMode();
    

    /**
    * @brief
    *   Getter function to fetch 'controllerId' from the object. 
    * @param
    *   void
    * @return
    *   string ControllerId
    */
    string getControllerId();
        

    /**
    * @brief
    *   Getter function to fetch 'priority or Network Strength' from the object.(depending on whoc created the object Server/Client). 
    * @param
    *   void
    * @return 
    *   string PriorityOrNs
    */
    string getPriorityOrNs();

};
