/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include "RCString.hpp"

using namespace ilrd::rd90;

/****************************   DECLARATIONS   *******************************/
/********************** Uitilty Function Declaration *************************/

static void TestRCString();

/**********************************  MAIN  ***********************************/
int main()
{
    TestRCString();
  
    return(0);
}

/*****************************************************************************/

static void TestRCString()
{
    RCString s1, s2("my name is");
    std::cout << "s1: \"" << s1 << "\"" << std::endl;
    std::cout << "s2: \"" << s2 << "\"" << std::endl;
    std::cout << "s2 len = " << s2.Length() << std::endl;
    
    RCString s3(s2), s4;
    s1 = "Inigo Montoya.";
    s2 += " ";
    s2 += s1;
    std::cout << "s2: \"" << s2 << "\"" << std::endl;
    s1 = "You killed my father.";
    s4 = " Prepare to die.";
    RCString s5 = s1 + s4;
    std::cout << "s5 = (s2 + s4): \"" << s5 << "\"" << std::endl;
    std::cout << "s5[8] = " << s5[8] << std::endl;
 
    std::cout << "Is s1 == s1? " << (s1 == s1) << std::endl;
    std::cout << "Is s1 == s2? " << (s1 == s2) << std::endl;
    std::cout << "Is s1 < s1? " << (s1 < s1) << std::endl;
    std::cout << "Is s1 > s1? " << (s1 > s1) << std::endl;
    std::cout << "Is s1 < s2? " << (s1 < s2) << std::endl;
    std::cout << "Is s1 > s2? " << (s1 > s2) << std::endl;

    const RCString s6("Life is pain, Highness! Anyone who says otherwise is selling something. a a");
    std::cout << "s6: \"" << s6 << "\"" << std::endl;
    std::cout << "s6[8] = " << s6[8] << std::endl;
}

/*****************************************************************************/
