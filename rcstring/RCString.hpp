#include <iostream>     //ostream
#pragma once

/*---------------------------------------------------------------------------*/

namespace ilrd
{
namespace rd90
{

/*****************************   Class String   ******************************/

class RCString;

bool operator==(const RCString& s1, const RCString& s2); // "hello" == s1 OR s1 == "hello"
bool operator<(const RCString& s1, const RCString& s2); // "hello" == s1 OR 
bool operator>(const RCString& s1, const RCString& s2); // "hello" == s1 OR 
const RCString operator+(RCString s1,const RCString& s2);
std::ostream& operator<<(std::ostream& os, const RCString& s);

class RCString
{
private:
    class Proxy; // forward declaration

public:
    /* non-explicit */RCString(const char* str1 = "",const char* str2 = ""); // def. ctor
    RCString(const RCString& other); // cctor
    RCString& operator=(const RCString& other); // assignment
    ~RCString(); // dtor
    const char& operator[](size_t i) const;
    Proxy operator[](size_t i);
    std::size_t Length() const;
    RCString& operator+=(const RCString& other);

private:
    class StringData;
    StringData *m_str_data;

    char GetChar(size_t i) const;
    void SetChar(char ch, size_t i);

    friend std::ostream& operator<<(std::ostream& os, const RCString& s);
    char *RCBuildRCString(const char *str1, const char* str2 = "");
    inline void cleanUp();
    friend bool operator==(const RCString& s1, const RCString& s2); // "hello" == s1 OR 
    friend bool operator<(const RCString& s1, const RCString& s2); // "hello" == s1 OR 
    friend bool operator>(const RCString& s1, const RCString& s2); // "hello" == s1 OR 
    friend const RCString operator+(RCString s1,const RCString& s2);
};

class RCString::Proxy
{
public:
    Proxy(RCString& str, std::size_t i);
    // ~Proxy();
    
    char operator=(char c);
    char operator=(const Proxy& other);
    operator char() const;

private:
    //Proxy(const Proxy& other);
    RCString* m_str;
    std::size_t m_i;
};

} // namespace rd90
} // namespace ilrd




