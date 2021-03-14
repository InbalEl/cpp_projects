#include <memory>       /* shared_ptr */
#pragma once

namespace ilrd {
namespace rd90 {

/****************************  Class Function  *******************************/
template <typename>
class Function;

/*****************  Class Function R(ARG) Specialization  ********************/

template <typename R, typename ARG>
class Function<R(ARG)>                              /* invocation signature */
{
public:
    explicit Function(); // generated default ctor is fine since the shared_ptr member is copyable
    template <typename T>
    explicit Function(R(T::*func)(ARG), T* obj);    // ctor template for member function
    explicit Function(R(*func)(ARG));               // ctor for free function

    Function(const Function&) = default;
    Function& operator=(const Function&) = default;

    R operator()(ARG) const;

/*-----------------------  Helper Class IFunction  --------------------------*/

private:
    class IFunction
    {
    public:
        virtual ~IFunction() {}
        virtual R operator()(ARG) const = 0;
    };

/***********************  Subclass MembFuncProxy  ****************************/

    template<typename T>
    class MembFuncProxy : public IFunction
    {
    public:
        explicit MembFuncProxy(R(T::*func)(ARG), T* obj);
        virtual R operator()(ARG data) const;

    private:
        T* m_obj;
        R(T::*m_func)(ARG);
    }; // class MembFuncProxy

/***********************  Subclass FreeFuncProxy  ****************************/

    class FreeFuncProxy : public IFunction
    {
    public:
        explicit FreeFuncProxy(R(*func_ptr)(ARG));
        virtual R operator()(ARG data) const;

    private:
        R(*m_func)(ARG);
    }; // class FreeFuncProxy

/*---------------------------------------------------------------------------*/

    std::shared_ptr<IFunction> m_func;
};

/*****************  Class Function R(ARG) Implementation  ********************/
/****************************  Class Function  *******************************/

template <typename R, typename ARG>
Function<R(ARG)>::Function() {}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
template<typename T>
Function<R(ARG)>::Function(R(T::*func)(ARG), T* obj)
 : m_func(new MembFuncProxy<T>(func, obj)) {}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
Function<R(ARG)>::Function(R(*func)(ARG))
 : m_func(new FreeFuncProxy(func)){}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
R Function<R(ARG)>::operator()(ARG data) const
{
    return ((*m_func)(data));
}

/***********************  Subclass MembFuncProxy  ****************************/

template <typename R, typename ARG>
template<typename T>
Function<R(ARG)>::MembFuncProxy<T>::MembFuncProxy(R(T::*func_ptr)(ARG), T* obj)
 : m_obj(obj), m_func(func_ptr) {}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
template<typename T>
R Function<R(ARG)>::MembFuncProxy<T>::operator()(ARG data) const
{
    return ((m_obj->*m_func)(data));
}

/****************  Subclass FreeFuncProxy Implementation  ********************/

template <typename R, typename ARG>
Function<R(ARG)>::FreeFuncProxy::FreeFuncProxy(R(*func_ptr)(ARG))
 : m_func(func_ptr) {}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
R Function<R(ARG)>::FreeFuncProxy::operator()(ARG data) const
{
    return ((m_func)(data));
}

/********************************   Bind   ***********************************/

template <typename T, typename R, typename ARG>
Function<R(ARG)> Bind(R(T::*func)(ARG), T* obj)
{
    return (Function<R(ARG)>(func, obj));
}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
Function<R(ARG)> Bind(R(*func)(ARG))
{
    return (Function<R(ARG)>(func));
}

/*****************************************************************************/
/*****************************************************************************/
/*****************  Class Function R(void) Specialization  *******************/
/*
Adapting template specialization to remember arguments given by bind,
and to use them when function is invoced.

- Arg arg is added as data member to both inheriting classed of IFunction

*/

template <typename R>
class Function<R(void)>
{
public:
    explicit Function(){}
    template <typename T, typename ARG> // ctor template for member function
    explicit Function(R(T::*func)(ARG), T* obj, const ARG& arg);
    template <typename ARG> // ctor for free function
    explicit Function(R(*func)(ARG), const ARG& arg);

    template <typename T, typename ARG>
    explicit Function(R(*func)(ARG), T* obj);

    template <typename ARG>
    explicit Function(R(*func)(ARG));

/*     template <typename T>
    explicit Function(R(*func)(T* obj));
    
    explicit Function(R(*func)()); */

    R operator()() const;

private:
    class IFunction;
    template<typename ARG>
    class FreeFuncProxy;
    template<typename T, typename ARG>
    class MembFuncProxy;

    std::shared_ptr<IFunction> m_func;
};

/*****************  Class Function R(void) Implementation  *******************/

template <typename R>
template<typename T, typename ARG>
Function<R(void)>::Function(R(T::*func)(ARG), T* obj, const ARG& arg)
 : m_func(new MembFuncProxy<T, ARG>(func, obj, arg)) {}

/*---------------------------------------------------------------------------*/

template <typename R>
template <typename ARG>
Function<R(void)>::Function(R(*func)(ARG), const ARG& arg)
 : m_func(new FreeFuncProxy<ARG>(func, arg)){}

/*---------------------------------------------------------------------------*/

template <typename R>
template<typename T, typename ARG>
Function<R(void)>::Function(R(*func)(ARG), T* obj)
 : m_func(new MembFuncProxy<T, ARG>(func, obj)) {}

/*---------------------------------------------------------------------------*/

template <typename R>
template <typename ARG>
Function<R(void)>::Function(R(*func)(ARG))
 : m_func(new FreeFuncProxy<ARG>(func)){}

/*---------------------------------------------------------------------------*/

template <typename R>
R Function<R(void)>::operator()() const
{
    return ((*m_func)());
}

/*********************  Abstract Subclass IFunction  *************************/

template <typename R>
class Function<R(void)>::IFunction
{
public:
    virtual ~IFunction() {}
    virtual R operator()() const = 0;
};

/****************  Subclass MembFuncProxy Implementation  ********************/

template <typename R>
template<typename T, typename ARG>
class Function<R(void)>::MembFuncProxy : public IFunction
{
public:
    explicit MembFuncProxy(R(T::*func)(ARG), T* obj, const ARG& arg);
    virtual R operator()() const;

private:
    T* m_obj;
    R(T::*m_func)(ARG);
    ARG m_arg;
};

/*****************************************************************************/

template <typename R>
template<typename T, typename ARG>
Function<R(void)>::MembFuncProxy<T, ARG>::MembFuncProxy(R(T::*func_ptr)(ARG), T* obj, const ARG& arg)
 : m_obj(obj), m_func(func_ptr), m_arg(arg){}

/*---------------------------------------------------------------------------*/

template <typename R>
template<typename T, typename ARG>
R Function<R(void)>::MembFuncProxy<T, ARG>::operator()() const
{
    return ((m_obj->*m_func)(m_arg));
}

/****************  Subclass FreeFuncProxy Implementation  ********************/

template <typename R>
template<typename ARG>
class Function<R(void)>::FreeFuncProxy : public IFunction
{
public:
    explicit FreeFuncProxy(R(*func_ptr)(ARG), const ARG& arg);
    virtual R operator()() const;

private:
    R(*m_func)(ARG);
    ARG m_arg;
};

/*****************************************************************************/

template <typename R>
template<typename ARG>
Function<R(void)>::FreeFuncProxy<ARG>::FreeFuncProxy(R(*func_ptr)(ARG), const ARG& arg)
 : m_func(func_ptr), m_arg(arg){}

/*---------------------------------------------------------------------------*/

template <typename R>
template<typename ARG>
R Function<R(void)>::FreeFuncProxy<ARG>::operator()() const
{
    return (m_func)(m_arg);
}

/********************************   Bind   ***********************************/

template <typename T, typename R, typename ARG>
Function<R(void)> Bind(R(T::*func)(ARG), T* obj, const ARG& arg)
{
    return (Function<R(void)>(func, obj, arg));
}

/*---------------------------------------------------------------------------*/

template <typename R, typename ARG>
Function<R(void)> Bind(R(*func)(ARG), const ARG& arg)
{
    return (Function<R(void)>(func, arg));
}

/*---------------------------------------------------------------------------*/

template <typename T, typename R>
Function<R(void)> Bind(R(T::*func)(), T* obj)
{
    return (Function<R(void)>(func, obj));
}

/*----------------------------------------------------------------------------*/

template <typename R>
Function<R(void)> Bind(R(*func)(void))
{
    return (Function<R(void)>(func));
}

/**************************  End of NameSpace  *******************************/
} // namespace ilrd
} // namespace rd90
/*****************************************************************************/