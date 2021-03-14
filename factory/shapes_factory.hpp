/***********************   PREPROCESSOR DIRECTIVES   *************************/
#pragma once
#include "factory.hpp"

/*****************************   ilrd::rd90   ********************************/
namespace ilrd {
namespace rd90 {
/*****************************   Class Shape   *******************************/

class Shape
{
public:
    Shape(int x = 0, int y = 0);
    int getX();
    int getY();
    void setX(int new_x);
    void setY(int new_y);
    void moveTo(int new_x, int new_y);
    void rMoveTo(int x_delta, int y_delta);
    virtual void draw();

private:
    int m_x;
    int m_y;
};

/*****************************  Class Rectangle  *****************************/

class Rectangle : public Shape
{
public:
   Rectangle(int x = 0, int y = 0, int width = 2, int height = 2);
   int getWidth();
   int getHeight();
   void setWidth(int new_width);
   void setHeight(int new_height);
   void draw();

private:
   int m_width;
   int m_height;
};

/******************************  Class Circle  *******************************/

class Circle: public Shape 
{
public:
   Circle(int x = 0, int y = 0, int radius  = 5);
   int getRadius();
   void setRadius(int new_radius);
   void draw();

private:
   int m_radius;
};

/*******************************  ilrd::rd90  ********************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/