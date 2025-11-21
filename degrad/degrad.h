#ifndef __DEGRAD_H__
#define __DEGRAD_H__

class radians;

class degrees {
  private:
    double val;

  public:
    degrees(double v = 0)
      : val(v) {}
    operator double() const {
        return val;
    }

    degrees operator+(const degrees& d) const;
    degrees operator-(const degrees& d) const;
    degrees operator*(double f) const;
    degrees operator/(double f) const;

    bool operator==(const degrees& d) const;
    bool operator!=(const degrees& d) const;
    bool operator<(const degrees& d) const;
    bool operator>(const degrees& d) const;

    radians to_radians() const;
};

class radians {
  private:
    double val;

  public:
    radians(double v = 0)
      : val(v) {}
    operator double() const {
        return val;
    }

    radians operator+(const radians& r) const;
    radians operator-(const radians& r) const;
    radians operator*(double f) const;
    radians operator/(double f) const;

    bool operator==(const radians& r) const;
    bool operator!=(const radians& r) const;
    bool operator<(const radians& r) const;
    bool operator>(const radians& r) const;

    degrees to_degrees() const;
};

#endif // !__DEGRAD_H__
