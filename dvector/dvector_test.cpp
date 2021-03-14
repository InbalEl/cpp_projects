#include <iostream>  // cout
#include <cstdlib> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <fstream> // ifstream
#include <sstream>
#include <string>
#include "RCString.hpp"
#include "dvector.hpp"

using namespace ilrd::rd90;

static void TestDVectorInts();
static void TestDVectorStrings();
static void LoadDict();

/*********************************  MAIN  ************************************/
int main()
{
    TestDVectorInts();
    TestDVectorStrings();
    
    DVector<std::string> vec1;

    DVector<String> vec2(vec1.Size());
    std::copy(vec1.Begin(), vec1.End(), vec2.Begin());
    
    LoadDict();

    return (EXIT_SUCCESS);
}

/*****************************************************************************/

static void TestDVectorInts()
{
    std::cout << "/***  Tesing vector with ints  ***/ " << std::endl;
    std::cout << "creating IntVec1(10, 5)" << std::endl;    
    DVector<int> IntVec1(10, 5);
    
    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[3] = " << IntVec1[3] << std::endl;

    std::cout << "----  PushBask 30 elements  ----" << std::endl;

    for (int i = 0; i < 30; ++i)
    {
        IntVec1.PushBack(i);
    }
    
    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[31] = " << IntVec1[31] << std::endl;

    std::cout << "----  PopBask 35 elements  ----" << std::endl;
    
    for (int i = 0; i < 35; ++i)
    {
        IntVec1.PopBack();
    }

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[4] = " << IntVec1[4] << std::endl;

    std::cout << "----  PushBask 10 elements  ----" << std::endl;
    
    for (int i = 0; i < 10; ++i)
    {
        IntVec1.PushBack(i);
    }

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[4] = " << IntVec1[4] << std::endl;

    std::cout << "----  Reserve 50  ----" << std::endl;

    IntVec1.Reserve(50);

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[10] = " << IntVec1[10] << std::endl;

    std::cout << "----  Reserve 20  ----" << std::endl;

    IntVec1.Reserve(20);

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[12] = " << IntVec1[12] << std::endl;

    std::cout << "----  Resize 20  ----" << std::endl;

    IntVec1.Resize(10, 42);

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[7] = " << IntVec1[7] << std::endl;

    std::cout << "----  Resize 30  ----" << std::endl;

    IntVec1.Resize(30, 42);

    std::cout << "capacity = " << IntVec1.Capacity() << std::endl;
    std::cout << "size = " << IntVec1.Size() << std::endl;
    std::cout << "IntVec1[20] = " << IntVec1[20] << std::endl;

    // DVector<int> IntVec2();
}

/*---------------------------------------------------------------------------*/

static void TestDVectorStrings()
{
    std::cout << "\n\n/***  Tesing vector with Strings  ***/ " << std::endl;
    std::cout << "creating StrVec1(10, \"Hello\")" << std::endl;    
    DVector<RCString> StrVec1(10, "Hello");
    
    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[3] = " << StrVec1[3] << std::endl;

    std::cout << "----  PushBask 30 elements  ----" << std::endl;

    for (int i = 0; i < 30; ++i)
    {
        StrVec1.PushBack("new element " + i);
    }
    
    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[31] = " << StrVec1[31] << std::endl;

    std::cout << "----  PopBask 35 elements  ----" << std::endl;
    
    for (int i = 0; i < 35; ++i)
    {
        StrVec1.PopBack();
    }

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[4] = " << StrVec1[4] << std::endl;

    std::cout << "----  PushBask 10 elements  ----" << std::endl;
    
    for (int i = 0; i < 10; ++i)
    {
        StrVec1.PushBack("another new element " + i);
    }

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[4] = " << StrVec1[4] << std::endl;

    std::cout << "----  Reserve 50  ----" << std::endl;

    StrVec1.Reserve(50);

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[10] = " << StrVec1[10] << std::endl;

    std::cout << "----  Reserve 20  ----" << std::endl;

    StrVec1.Reserve(20);

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[12] = " << StrVec1[12] << std::endl;

    std::cout << "----  Resize 20  ----" << std::endl;

    StrVec1.Resize(10, "42");

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[7] = " << StrVec1[7] << std::endl;

    std::cout << "----  Resize 30  ----" << std::endl;

    StrVec1.Resize(30, "31");

    std::cout << "capacity = " << StrVec1.Capacity() << std::endl;
    std::cout << "size = " << StrVec1.Size() << std::endl;
    std::cout << "StrVec1[20] = " << StrVec1[20] << std::endl;
}


/*---------------------------------------------------------------------------*/

static void LoadDict()
{
    static const size_t size_of_word = 50; 
    std::cout << "\n\n/***  Tesing vector with Dictionary  ***/ " << std::endl;
    std::ifstream ifs("/usr/share/dict/words");
    DVector<RCString> dictionary(1000);

    char string_buffer[size_of_word];
    while (ifs.getline(string_buffer, size_of_word)) 
    {
        dictionary.PushBack(string_buffer);
    }

    size_t dict_size = dictionary.Size();
    for (size_t i = 0; i < dict_size; i++)
    {
        std::cout << dictionary[i] << std::endl;
    }
    
    ifs.close(); //close the file object.
}

/*****************************************************************************/