#include "../Headers/Point.h"

/*
Point() {}
Point(float x, float y, float z) {}
void set(int x, int y, float z) {}

float X() {}
float Y() {}
float Z() {}
*/

Point::Point() {
    x_ = 0.0f;
    y_ = 0.0f;
    z_ = 0.0f;
}

Point::Point(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

void Point::set(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

float Point::X() { return x_; }
float Point::Y() { return y_; }
float Point::Z() { return z_; }

Point& Point::operator=(const Point& r) {
    x_ = r.x_; y_ = r.y_; z_ = r.z_;    
    return *this;
}

Point& Point::operator*=(float amount) {
    x_ *= amount; y_ *= amount; z_ *= amount;    
    return *this;
}

Point& Point::operator/=(float amount) {
    x_ /= amount; y_ /= amount; z_ /= amount;    
    return *this;
}

Point Point::operator*(float factor) const {
    return Point(x_ * factor, y_ * factor, z_ * factor);
}

Point operator*(float factor, const Point& other) {
    return Point(other.x_ * factor, other.y_ * factor, other.z_ * factor);
}

Point Point::operator/(float factor) const {
    return Point(x_ / factor, y_ / factor, z_ / factor);
}

ostream& operator << (ostream& os,const Point& m) {
    os << "[ " << m.x_ << " " << m.y_ << " " << m.z_ << " ]\n";
    return os;
}

istream& operator >> (istream& is, Point& p) {
    is >> p.x_ >> p.y_ >> p.z_;
    return is;
}