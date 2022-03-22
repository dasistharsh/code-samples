#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using std::endl;
using std::cout;
using std::string;

/**
* @file configParser.h
* @brief
*   This file contains the KeyValue class.
* @class KeyValue 
* @brief
*   Class for holding the key-value pair.
* @details
*   The configuration Parameter will be the key and the corresponding value mentioned will be the 'value' in the pair.
*   This class is used for storage purposed required for parsing the 'routing.conf' i.e the configuration file and thus the name 'configParser'.
*/

class KeyValue  {
	string m_key;    /** string m_key : 'key' to hold the name of the configuration parameter */
	string m_value;  /** string m_value: 'Value' to hold the value of the corresponding configuration parameter */
  public:		

    /** 
    * @brief 
    *   Constructor of KeyValue Class.
    * @details
    *   Initialises members of the object.
    * @param 'string key' : The 'key' of the 'key-value' pair.
    * @param 'string value' : The 'value' of the 'key-value' pair.
    */
		KeyValue(string key , string value)  // Direct initalisation
        : m_key(key) , m_value(value)  
    {}

    /** 
    * @brief 
    *   Constructor of KeyValue Class.
    * @details
    *   Initialises members of the object(seperate constructor so that no implicit type-casting is required).
    * @param 'string key' : The 'key' of the 'key-value' pair.
    * @param 'string value' : The 'value' of the 'key-value' pair.
    */
		KeyValue(char *key, char *value)     // Direct initalisation
			: m_key (string(key)) , m_value (string(value))
    {}

    /**
    * @brief
    *   Getter function to fetch 'key' i.e name of the configuration parameter from the object.
    * @param
    *   void
    * @return
    *    'string key' 
    */
    string getKey()
    {
      return this->m_key;
    }

    /**
    * @brief
    *   Getter function to fetch 'value' i.e value corresponding to the name of the configuration parameter from the object.
    * @param
    *   void
    * @return
    *    'string value' 
    */
    string getValue()
    {
      return this->m_value;
    }
};


/**
* @brief
*   Parses a line at a time from the configuration file.
* @details
*   This function takes 1 single line of input read from the configuration file and parse it's value based on the delimiters 
*   and then returns a pointer to the object pointing to the key-value pair.
* @param 'string singleLine' : One single line of the configuration file(routing.conf).
* @return
*   'KeyValue *' : Dynamic keyValue pair object.
*/

KeyValue * Parse(string singleLine) {
  string nullValue("Null");
  if (singleLine[0] == '#' || singleLine[0] == '\n' || singleLine[0] == '\0' )
    return  new KeyValue(nullValue, nullValue);

  int length=singleLine.length();
  char key[length] , value[length];
  int delimiterEncountered=0 , index1=0,index2=0;
  
  for ( int i = 0 ; i < length ; i ++){
    if (singleLine[i] == '=') {
      delimiterEncountered =1;
      i++;      
    }
    if (!delimiterEncountered)  {
      key[index1] = singleLine[i];
      index1++;
    }
    else  {
      value[index2]=singleLine[i];
      index2++;
    }
  }
  
  key[index1]='\0';
  value[index2]='\0';
  if (!delimiterEncountered)  {
    // cout << "[#] No delimiter found - error"<<endl;
    return  new KeyValue(string("corrupt"), nullValue);
  }
  // cout << "[*] Returning "<<key<<":"<<value<<endl;

  return new KeyValue(key,value);
}
  
