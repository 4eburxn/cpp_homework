#include "degrad.h"

/*****AngleDeg*****/
AngleDeg::AngleDeg(double degrees)
  : deg(degrees) {}

AngleDeg::AngleDeg(const AngleRad& rad_angle) {
    deg = rad_angle.getRad() * 180.0 / M_PI;
}

AngleDeg
AngleDeg::operator+(const AngleDeg& other) const {
    return AngleDeg(deg + other.deg);
}

AngleDeg
AngleDeg::operator-(const AngleDeg& other) const {
    return AngleDeg(deg - other.deg);
}

AngleDeg
AngleDeg::operator-() const {
    return AngleDeg(-deg);
}

/*****AngleRad*****/
AngleRad::AngleRad(double radians)
  : rad(radians) {}

AngleRad::AngleRad(const AngleDeg& deg_angle) {
    rad = deg_angle.getDeg() * M_PI / 180.0;
}

AngleRad
AngleRad::operator+(const AngleRad& other) const {
    return AngleRad(rad + other.rad);
}

AngleRad
AngleRad::operator-(const AngleRad& other) const {
    return AngleRad(rad - other.rad);
}

AngleRad
AngleRad::operator-() const {
    return AngleRad(-rad);
}
