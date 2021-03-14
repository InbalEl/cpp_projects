// ###################################################################
// #file Name   :    function.hpp
// #Description :    function class decleration and definition
// #Create Date :	 5.1.2020
// #last update :    6.1.2020   (add ARG)
// #Developer   :    Gilad Barak  
// ###################################################################

#include <functional>
#include <unordered_map>

namespace ilrd{
namespace rd90{

/*----------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
class Factory
{
public:
    explicit Factory() = default;
    template<typename R>
    void Register(KEY key);
    T *Create(KEY key, ARG arg);

private:
    template<typename R>
    static T* CreateInstance(ARG arg);
    std::unordered_map<KEY, std::function<T*(ARG)>> m_creators;
};

/*----------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
template<typename R>
void Factory<T,KEY,ARG>::Register(KEY key)
{
    m_creators.insert(std::make_pair(key,&Factory<T,KEY,ARG>::CreateInstance<R>));
}

/*----------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
template<typename R>
 T * Factory<T,KEY,ARG>::CreateInstance(ARG arg)
 {
    return (new R(arg));
 }
/*----------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
T* Factory<T,KEY,ARG>::Create(KEY key,ARG arg)
{
    return (m_creators[key](arg));
}

/*----------------------------------------------------------------------------*/

}
}