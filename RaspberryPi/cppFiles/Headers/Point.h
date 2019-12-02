#include <string>
#include <iostream>

#ifndef POINT
#define POINT

using namespace std;

//we can expand on this class later
class Point {
public:
    Point();
    Point(float x, float y, float z);
    void set(float x, float y, float z);
    
    float X();
    float Y();
    float Z();

    operator auto() const {
        std::string res = "{\"x\":" + std::to_string(x_)
               +", \"y\":"+std::to_string(y_)
               +", \"z\":"+std::to_string(z_)+"}";
        return res;
    }

    Point& operator=(const Point& r);
    Point operator*(float factor) const;
    friend Point operator*(float factor, const Point& other);
    Point operator/(float factor) const;
    Point& operator*=(float amount);
    Point& operator/=(float amount);
    friend ostream& operator<<(ostream& os,const Point& m);
    friend istream& operator>>(istream& is, Point& p);
protected:
    float x_;
    float y_;
    float z_;
};

#endif