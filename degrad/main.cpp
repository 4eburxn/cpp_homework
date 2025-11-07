#include "degrad.h"
#include <cmath>
#include <iostream>

int
main(int argc, char* argv[]) {
    AngleRad rad1(M_PI);
    AngleDeg deg1(180.0);

    // Явное преобразование между системами
    AngleRad rad2 = AngleRad(deg1);
    AngleDeg deg2 = AngleDeg(rad1);

    std::cout << "rad1: " << rad1.getRad() << " rad" << std::endl;
    std::cout << "deg1: " << deg1.getDeg() << " deg" << std::endl;
    std::cout << "rad2 from deg1: " << rad2.getRad() << " rad" << std::endl;
    std::cout << "deg2 from rad1: " << deg2.getDeg() << " deg" << std::endl;

    // Арифметические операции
    AngleRad rad_sum  = rad1 + rad2;
    AngleDeg deg_diff = deg1 - deg2;

    std::cout << "rad1 + rad2: " << rad_sum.getRad() << " rad" << std::endl;
    std::cout << "deg1 - deg2: " << deg_diff.getDeg() << " deg" << std::endl;

    // Умножение и деление на числа разных типов
    AngleRad rad_mult  = rad1 * 2.0f;  // float
    AngleDeg deg_div   = deg1 / 3;     // int
    AngleRad rad_mult2 = 2.5 * rad1;   // double
    AngleDeg deg_div2  = 360.0 / deg1; // double

    std::cout << "rad1 * 2.0f: " << rad_mult.getRad() << " rad" << std::endl;
    std::cout << "deg1 / 3: " << deg_div.getDeg() << " deg" << std::endl;
    std::cout << "2.5 * rad1: " << rad_mult2.getRad() << " rad" << std::endl;
    std::cout << "360.0 / deg1: " << deg_div2.getDeg() << " deg" << std::endl;
    return 0;
}
