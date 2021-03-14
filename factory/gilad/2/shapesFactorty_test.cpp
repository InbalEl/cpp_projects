// ###################################################################
// #file Name   :    shapesFactory_test.cpp
// #Description :    shapes class decleration
// #Create Date :	 5.1.2020
// #last update :    5.1.2020
// #Developer   :    Gilad Barak  
// ###################################################################

#include "factory.hpp"
#include "shapesFactorty.hpp"

using namespace ilrd::rd90;


int main()
{
    Factory<Shape,int,std::string> shape1;
    // Factory<Shape,int,std::string> shape1;

    shape1.Register<Circle>(1);
    shape1.Register<Rectangle>(2);

    std::shared_ptr<Shape> shape_ptr1 = shape1.Create(1,"Circle");
    std::shared_ptr<Shape> shape_ptr2 = shape1.Create(2,"Rectangle");

    shape_ptr1->draw();
    shape_ptr2->draw();

    return 0;
}


