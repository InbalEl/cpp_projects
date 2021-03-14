#include <iostream>  // cout
#include <cstdlib> /* EXIT_SUCCESS, EXIT_FAILURE */
// #include "RCString.hpp"
#include "SmartPointers.hpp"

using namespace ilrd::rd90;

class Student
{
public:
    Student(int age = 0);
    ~Student();
    int m_age;
private:
};

Student::Student(int age) : m_age(age) {}

Student::~Student() {}


/*********************************  MAIN  ************************************/
int main()
{
    SharedPtr<size_t> num1(new size_t);
    SharedPtr<size_t> num2(new size_t);
    SharedPtr<size_t> num3(num1);
    SharedPtr<size_t> num4(num1);

    *num1 = 42;
    *num2 = 21;

    std::cout << "num1 = " << *num1 << std::endl;
    std::cout << "num2 = " << *num2 << std::endl;
    std::cout << "num3 = " << *num3 << std::endl;
    std::cout << "num4 = " << *num4 << std::endl;
    std::cout << "num4.UseCount() = (should be 3) " << num4.UseCount() << std::endl;

    if (!num1 == 0)
    {
        std::cout << "!num == 0? yes it is! " << std::endl;
    }
    
    SharedPtr<Student> me(new Student(32));

    std::cout << "me->m_age = " << me->m_age << std::endl;

    return (EXIT_SUCCESS);
}

/*****************************************************************************/