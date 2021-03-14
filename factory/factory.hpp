/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <unordered_map>
#include <memory>
#pragma once

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {

/*****************************   Class Factory   *****************************/
template <typename T, typename KEY, typename ARG>
class Factory
{
public:
    explicit Factory() = default;

    template<typename R>
    void Register(KEY key);

    std::shared_ptr<T> Create(KEY key, const ARG& arg);

private:
    typedef std::shared_ptr<T>(*func_ptr)(const ARG& arg);

    template<typename R>
    static std::shared_ptr<T> CreateNew(const ARG& arg);

    std::unordered_map<KEY, func_ptr> m_creators;
    // std::unordered_map<KEY, std::function<T*(ARG)>> m_creators;
};

/***********************  Factory Implementation   ***************************/

template <typename T, typename KEY, typename ARG>
template<typename R>
void Factory<T, KEY, ARG>::Register(KEY key)
{
    m_creators.insert(std::make_pair(key,&Factory<T,KEY,ARG>::CreateNew<R>));
}

/*---------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
std::shared_ptr<T> Factory<T, KEY, ARG>::Create(KEY key, const ARG& arg)
{
    return (m_creators[key](arg));
}

/*---------------------------------------------------------------------------*/

template <typename T, typename KEY, typename ARG>
template<typename R>
std::shared_ptr<T> Factory<T, KEY, ARG>::CreateNew(const ARG& arg)
{
    std::shared_ptr<T> ptr(new R(arg));
    return (ptr);
}

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/