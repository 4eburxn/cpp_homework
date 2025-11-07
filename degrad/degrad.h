#ifndef __DEGRAD_H__
#define __DEGRAD_H__

#include <cmath>
#include <cmath>

class AngleRad; // Предварительное объявление

class AngleDeg
{
  private:
    double deg;

  public:
    explicit AngleDeg(double degrees = 0.0);
    explicit AngleDeg(const AngleRad& rad_angle);

    double getDeg() const { return deg; }

    AngleDeg operator+(const AngleDeg& other) const;
    AngleDeg operator-(const AngleDeg& other) const;
    AngleDeg operator-() const;

    template<typename T>
    AngleDeg operator*(const T& value) const
    {
        return AngleDeg(deg * static_cast<double>(value));
    }

    template<typename T>
    AngleDeg operator/(const T& value) const
    {
        return AngleDeg(deg / static_cast<double>(value));
    }
};

class AngleRad
{
  private:
    double rad;

  public:
    explicit AngleRad(double radians = 0.0);
    explicit AngleRad(const AngleDeg& deg_angle);

    double getRad() const { return rad; }

    AngleRad operator+(const AngleRad& other) const;
    AngleRad operator-(const AngleRad& other) const;
    AngleRad operator-() const;

    template<typename T>
    AngleRad operator*(const T& value) const
    {
        return AngleRad(rad * static_cast<double>(value));
    }

    template<typename T>
    AngleRad operator/(const T& value) const
    {
        return AngleRad(rad / static_cast<double>(value));
    }
};

template<typename T>
AngleRad
operator*(const T& value, const AngleRad& angle)
{
    return AngleRad(static_cast<double>(value) * angle.getRad());
}

template<typename T>
AngleDeg
operator*(const T& value, const AngleDeg& angle)
{
    return AngleDeg(static_cast<double>(value) * angle.getDeg());
}

template<typename T>
AngleRad
operator/(const T& value, const AngleRad& angle)
{
    return AngleRad(static_cast<double>(value) / angle.getRad());
}

template<typename T>
AngleDeg
operator/(const T& value, const AngleDeg& angle)
{
    return AngleDeg(static_cast<double>(value) / angle.getDeg());
}

#endif // !__DEGRAD_H__
