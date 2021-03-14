/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <memory>
#include "factory.hpp"
#include "shapes_factory.hpp"

/*************************  namespace ilrd::rd90  ****************************/
using namespace ilrd::rd90;
/****************************   DECLARATIONS   *******************************/

/**********************************  MAIN  ***********************************/

int main()
{
    Factory<Shape, int, int> shape_factory;

    shape_factory.Register<Circle>(1);
    shape_factory.Register<Rectangle>(2);

    std::cout << "creating shapes" << std::endl;
    std::shared_ptr<Shape> circle1 = shape_factory.Create(1, 10);
    std::shared_ptr<Shape> circle2(shape_factory.Create(1, 5));

    std::shared_ptr<Shape> rec1(shape_factory.Create(2, 3));

    std::cout << "shape2(circle) x,y = " << circle2->getX() << ", " << circle2->getY() << std::endl;
    std::cout << "rec1 Y = " << rec1->getY() << std::endl;
    return(0);
}

/*****************************************************************************/

/*****************************************************************************/
