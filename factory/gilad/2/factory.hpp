// ###################################################################
// #file Name   :    function.hpp
// #Description :    function class decleration and definition
// #Create Date :	 5.1.2020
// #last update :    10.1.2020   (Change to smart pointers)
// #Developer   :    Gilad Barak  
// ###################################################################

#include <functional>
#include <unordered_map>
#include <memory>

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
    std::shared_ptr<T> Create(KEY key, const ARG& arg);

private:
    template<typename R>
    static std::shared_ptr<T> CreateInstance(const ARG& arg);
    typedef std::shared_ptr<T>(*func_ptr)(const ARG& arg);
    std::unordered_map<KEY,func_ptr> m_creators;
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
 std::shared_ptr<T> Factory<T,KEY,ARG>::CreateInstance(const ARG& arg)
 {
    std::shared_ptr<T> ret(new R(arg));
    return (ret);
 }
/*----------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
std::shared_ptr<T> Factory<T,KEY,ARG>::Create(KEY key,const ARG& arg)
{
    return (m_creators[key](arg));
}

/*----------------------------------------------------------------------------*/

}
}