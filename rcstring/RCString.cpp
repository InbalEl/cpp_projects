/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <cstring>
#include <cstddef>
#include "RCString.hpp"

namespace ilrd
{
namespace rd90
{

/******************************   Class Proxy   ******************************/

RCString::Proxy::Proxy(RCString& str, std::size_t i): m_str(&str), m_i(i)
{}

/*---------------------------------------------------------------------------*/

RCString::Proxy::operator char() const
{
    return (m_str->GetChar(m_i));
}

/*---------------------------------------------------------------------------*/

char RCString::Proxy::operator=(char c)
{
    m_str->SetChar(c, m_i);
    return (m_str->GetChar(m_i));
}

/*---------------------------------------------------------------------------*/

char RCString::Proxy::operator=(const Proxy& other)
{
    char c = other;
    return (*this = c);
}

/****************************   Class StdData   ******************************/

class RCString::StringData
{
public:
    static StringData *Create(const char *str1, const char* str2 = "");
    bool IsShared() const;
    void Attach();
    void Detach();
    size_t GetStrLen() const;
    char& operator[](size_t i);

private:
    explicit StringData(const char* str, const char *str2, 
                        size_t len_str1, size_t len_str2);  
    size_t m_counter;
    char m_str[1]; // flexible array
};

/*---------------------------------------------------------------------------*/

bool RCString::StringData::IsShared() const
{
    return (m_counter != 1);
}

/*---------------------------------------------------------------------------*/

RCString::StringData *RCString::StringData::Create(const char *str1,
                                                   const char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2) + 1;
    
    void *buffer = ::operator new
                (offsetof(StringData, m_str) + len1 + len2);
    // using placement new
    return new (buffer) StringData(str1, str2, len1, len2);
}

/*---------------------------------------------------------------------------*/

RCString::StringData::StringData(const char* str, const char *str2, 
                                 size_t len_str1, size_t len_str2) 
                                 : m_counter(1)
{
    memcpy(m_str, str, len_str1);
    memcpy(m_str + len_str1, str2, len_str2);
}

/*---------------------------------------------------------------------------*/

void RCString::StringData::Detach()
{   // decrementing counter and deleting if necessary
    if (--m_counter == 0)
    {
        delete this;
    }
}

/*---------------------------------------------------------------------------*/

void RCString::StringData::Attach()
{   // incerementing counter
    ++m_counter;
}

/*---------------------------------------------------------------------------*/

size_t RCString::StringData::GetStrLen() const
{
    return (strlen(m_str));
}

/*---------------------------------------------------------------------------*/

char& RCString::StringData::operator[](size_t i)
{
    return (m_str[i]);
}
/***************************   Class RCString   ******************************/
//ctor
RCString::RCString(const char* str1, const char* str2)
    : m_str_data(RCString::StringData::Create(str1, str2))
{}

/*---------------------------------------------------------------------------*/
//cctor
RCString::RCString(const RCString& other): m_str_data(other.m_str_data)
{
    m_str_data->Attach();
}

/*---------------------------------------------------------------------------*/
//assignment
RCString& RCString::operator=(const RCString& other)
{
    /* The 2 challenges an assignment operator must deal with:
    * 1. self-assignment (solution: if ())
    * 2. Exception Safety (new can throw)
    */
    if (this != &other) // protecting against self-assignment
    {
        m_str_data->Detach();
        m_str_data = other.m_str_data;
        m_str_data->Attach();
    }

    return (*this);
}

/*---------------------------------------------------------------------------*/

RCString::~RCString()
{
    m_str_data->Detach();
}

/*---------------------------------------------------------------------------*/

size_t RCString::Length() const
{
    return ((*m_str_data).GetStrLen());
}

/*---------------------------------------------------------------------------*/

std::ostream& operator<<(std::ostream& os, const RCString& s)
{
    return (os << (&((*s.m_str_data)[0])));
}

/*---------------------------------------------------------------------------*/

const char& RCString::operator[](size_t i) const
{
    return ((*m_str_data)[i]);
}

/*---------------------------------------------------------------------------*/

RCString::Proxy RCString::operator[](size_t index)
{
    return Proxy(*this, index);
}

/*---------------------------------------------------------------------------*/

char RCString::GetChar(size_t i) const
{
    return ((*m_str_data)[i]);
}

/*---------------------------------------------------------------------------*/

void RCString::SetChar(char ch, size_t i)
{
    char *str = &((*m_str_data)[0]);
    
    if (m_str_data->IsShared())
    {

        RCString::StringData *new_str_data =
                    RCString::StringData::Create(str);
        m_str_data->Detach();
        m_str_data = new_str_data;
    }

    *(str + i) = ch;
}

/*---------------------------------------------------------------------------*/

RCString& RCString::operator+=(const RCString& other)
{
    *this = RCString(&((*m_str_data)[0]), &other[0]);
    return (*this);
}

/*---------------------------------------------------------------------------*/

const RCString operator+(RCString s1,const RCString& s2)
{
    return (s1 += s2);
}

/*---------------------------------------------------------------------------*/

bool operator==(const RCString& s1, const RCString& s2)
{
    return (0 == strcmp(&s1[0], &s2[0]));
}

/*---------------------------------------------------------------------------*/

bool operator<(const RCString& s1, const RCString& s2)
{
    return (0 > strcmp(&s1[0], &s2[0]));
}

/*---------------------------------------------------------------------------*/

bool operator>(const RCString& s1, const RCString& s2)
{
    return (0 < strcmp(&s1[0], &s2[0]));
}

/*---------------------------------------------------------------------------*/
} // namespace ilrd
} // namespace rd90
/*****************************************************************************/
