// ###################################################################
// #file Name   :    shapesFactory.cpp
// #Description :    shapes class decleration
// #Create Date :	 5.1.2020
// #last update :    5.1.2020
// #Developer   :    Gilad Barak  
// ###################################################################

#include "shapesFactorty.hpp"
#include <iostream>

namespace ilrd {
namespace rd90 {


/********************************** Shape *************************************/

// constructor
Shape::Shape(int newx, int newy)
{
   moveTo(newx, newy);
}

/*----------------------------------------------------------------------------*/

// accessors for x & y
int Shape::getX() 
{ 
    return x; 
}

/*----------------------------------------------------------------------------*/

int Shape::getY() 
{ 
    return y; 
}

/*----------------------------------------------------------------------------*/

void Shape::setX(int newx) 
{ 
    x = newx; 
}

/*----------------------------------------------------------------------------*/

void Shape::setY(int newy) 
{ 
    y = newy;
}

/*----------------------------------------------------------------------------*/

// move the shape position
void Shape::moveTo(int newx, int newy) 
{
   setX(newx);
   setY(newy);
}

/*----------------------------------------------------------------------------*/

void Shape::rMoveTo(int deltax, int deltay) 
{
   moveTo(getX() + deltax, getY() + deltay);
}

/*----------------------------------------------------------------------------*/

// abstract draw method
void Shape::draw() 
{}

/******************************** Rectangle ***********************************/


// constructor
Rectangle::Rectangle(int newx, int newy, int newwidth, int newheight): 
                    Shape(newx, newy) 
{
   setWidth(newwidth);
   setHeight(newheight);
}

/*----------------------------------------------------------------------------*/

// accessors for width and height
int Rectangle::getWidth() 
{ 
    return width; 
}

/*----------------------------------------------------------------------------*/

int Rectangle::getHeight() 
{ 
    return height; 
}

/*----------------------------------------------------------------------------*/
void Rectangle::setWidth(int newwidth) 
{ 
    width = newwidth; 
}

/*----------------------------------------------------------------------------*/

void Rectangle::setHeight(int newheight) 
{ 
    height = newheight; 
}

/*----------------------------------------------------------------------------*/

// draw the rectangle
void Rectangle::draw() 
{
   std::cout << "Drawing a Rectangle at:(" << getX() << "," << getY() <<
      "), width " << getWidth() << ", height " << getHeight() << std::endl;
}

/*****************************************************************************/

// constructor
Circle::Circle(int newx, int newy, int newradius): Shape(newx, newy)
{
   setRadius(newradius);
}

/*----------------------------------------------------------------------------*/

// accessors for the radius
int Circle::getRadius() 
{ 
    return radius; 
}

/*----------------------------------------------------------------------------*/

void Circle::setRadius(int newradius) 
{ 
    radius = newradius; 
}

/*----------------------------------------------------------------------------*/

// draw the circle
void Circle::draw() 
{
  std::cout << "Drawing a Circle at:(" << getX() << "," << getY() <<
      "), radius " << getRadius() << std::endl;
}

/*----------------------------------------------------------------------------*/

} // namespace rd90
} // namespace ilrd