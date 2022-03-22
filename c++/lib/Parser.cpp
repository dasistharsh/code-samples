#include "Parser.h"
// Added bhaskar
    /* Expected Format :
           - Ns Or Priority , timsestamp , mode , controllerId
           - 1,1234567890,automatic,0  
    */
    Parser::Parser(char* pointerString) : Parser(string(pointerString)){} // C++11

    Parser::Parser(string entireString) {
      
      entireString += ",";   // appended ',' in end : 1,1234567890,automatic,0,
      
      m_priorityOrNs = "";
      m_timestamp    = -1;   // As this wont be the timestamp ever.
      m_mode         = "";
      m_controllerId = "";

      this->Parser::parsedString(entireString);
      if (this->m_priorityOrNs == "" || this->m_timestamp == -1 || this->m_mode == "" || this->m_controllerId == "")
        this->m_formatIsCorrect = false;
      else
        this->m_formatIsCorrect = true;
    }

    /*
    * The function actually responsible for parsing the string and extracting the values of all the parameters.
    * Ex : 1,1234567890,automatic,0, : Note a 'comma' at end.
    */
    void Parser::parsedString( string str1) {
      int i    {0};
      size_t x {0};
      int len   = str1.length();

      while(i<4) {
        x = str1.find(",");
        if ( x!= string::npos)
        {
          string token = str1.substr(0,x);
          if (i == 0)
            m_priorityOrNs = token;
          else if (i == 1)
            m_timestamp = std::stoll(token);
          else if (i==2)
            m_mode = token;
          else if (i==3)
            m_controllerId = token;
        }
        str1 = str1.substr(x+1,len);
        i++;
      }
      return;
    }

    bool Parser::getFormatIsCorrect() {
      return this->m_formatIsCorrect;
    }

    long long Parser::getTimestamp() {
      return this->m_timestamp;
    }

    string Parser::getMode()  {
      return this->m_mode;
    }

    string Parser::getControllerId()  {
      return this->m_controllerId;
    }

    string Parser::getPriorityOrNs()  {
      return this->m_priorityOrNs;
    }
