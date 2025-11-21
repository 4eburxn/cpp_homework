#include "degrad.h"
#include <cmath>

/***********radians*************/
radians
radians::operator+(const radians& r) const {
    return radians(val + r.val);
}
radians
radians::operator-(const radians& r) const {
    return radians(val - r.val);
}
radians
radians::operator+(double f) const {
    return radians(val + f);
}
radians
radians::operator-(double f) const {
    return radians(val - f);
}
radians
radians::operator*(double f) const {
    return radians(val * f);
}
radians
radians::operator/(double f) const {
    return radians(val / f);
}

bool
radians::operator==(const radians& r) const {
    return val == r.val;
}
bool
radians::operator!=(const radians& r) const {
    return val != r.val;
}
bool
radians::operator<(const radians& r) const {
    return val < r.val;
}
bool
radians::operator>(const radians& r) const {
    return val > r.val;
}
degrees
radians::to_degrees() const {
    return degrees(val * 180.0 / M_PI);
}

/***********degrees*************/
degrees
degrees::operator+(const degrees& d) const {
    return degrees(val + d.val);
}
degrees
degrees::operator-(const degrees& d) const {
    return degrees(val - d.val);
}
degrees
degrees::operator+(double f) const {
    return degrees(val + f);
}
degrees
degrees::operator-(double f) const {
    return degrees(val - f);
}
degrees
degrees::operator*(double f) const {
    return degrees(val * f);
}
degrees
degrees::operator/(double f) const {
    return degrees(val / f);
}

bool
degrees::operator==(const degrees& d) const {
    return val == d.val;
}
bool
degrees::operator!=(const degrees& d) const {
    return val != d.val;
}
bool
degrees::operator<(const degrees& d) const {
    return val < d.val;
}
bool
degrees::operator>(const degrees& d) const {
    return val > d.val;
}

radians
degrees::to_radians() const {
    return radians(val * M_PI / 180.0);
}
