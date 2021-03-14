// ###################################################################
// #file Name   :    shapesFactory.hpp
// #Description :    shapes class decleration
// #Create Date :	 5.1.2020
// #last update :    5.1.2020
// #Developer   :    Gilad Barak  
// ###################################################################

#pragma once

namespace ilrd {
namespace rd90 {

/*---------------------------------------------------------------------------*/

class Shape 
{

public:
   Shape(int newx = 0, int newy = 0);
   int getX();
   int getY();
   void setX(int newx);
   void setY(int newy);
   void moveTo(int newx, int newy);
   void rMoveTo(int deltax, int deltay);
   virtual void draw();

private:
   int x;
   int y;
};

/*---------------------------------------------------------------------------*/

class Rectangle: public Shape 
{
public:
   Rectangle(int newx = 0, int newy = 0, int newwidth = 5, int newheight = 5);
   int getWidth();
   int getHeight();
   void setWidth(int newwidth);
   void setHeight(int newheight);
   void draw();

private:
   int width;
   int height;
};

/*---------------------------------------------------------------------------*/

class Circle: public Shape 
{
public:
   Circle(int newx = 0, int newy = 0, int newradius  = 5);
   int getRadius();
   void setRadius(int newradius);
   void draw();

private:
   int radius;
};


} // namespace rd90
} // namespace ilrd
/*****************************************************************************/