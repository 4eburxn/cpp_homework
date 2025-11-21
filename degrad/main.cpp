#include "degrad.h"
#include <cmath>
#include <iostream>

int
main(int argc, char* argv[]) {
    degrees deg1(90.0);
    radians rad1(3.14159);

    degrees deg2 = deg1 + degrees(45.0);
    radians rad2 = rad1 + radians(1.57);

    radians converted_rad = deg1.to_radians();
    degrees converted_deg = rad1.to_degrees();

    bool equal_deg     = (deg1 == degrees(90.0));
    bool not_equal_rad = (rad1 != radians(2.0));

    degrees scaled_deg = deg1 * 2.0;
    radians scaled_rad = rad1 / 2.0;

    double deg_value = (double)deg1;
    double rad_value = (double)rad1;

    std::cout << "deg1: " << deg_value << " degrees\n";
    std::cout << "rad1: " << rad_value << " radians\n";
    std::cout << "deg1 to radians: " << (double)converted_rad << "\n";
    std::cout << "rad1 to degrees: " << (double)converted_deg << "\n";
    std::cout << "deg1 == 90.0: " << equal_deg << "\n";
    std::cout << "rad1 != 2.0: " << not_equal_rad << "\n";
    std::cout << "deg1 * 2: " << (float)(scaled_deg) << "\n";
    std::cout << "rad1 / 2: " << (float)(scaled_rad) << "\n";

    return 0;
}
