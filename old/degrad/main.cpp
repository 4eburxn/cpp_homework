#include "degrad.h"
#include <cmath>
#include <iostream>

int
main(int argc, char* argv[]) {
    degrees deg1(90.0);
    radians rad1(M_PI);

    degrees deg2 = deg1 + degrees(45.0);
    degrees deg3 = deg1 - 45.;
    radians rad2 = rad1 + radians(M_PI / 2);
    radians rad3 = rad1 - M_PI / 2;
    bool check   = ((double)deg2 == 135.) && ((double)deg3 == 45.) &&
                 ((double)rad2 == M_PI + M_PI / 2) &&
                 ((double)rad3 == M_PI - M_PI / 2);

    radians converted_rad = deg1.to_radians();
    degrees converted_deg = rad1.to_degrees();

    bool equal_deg     = (deg1 == degrees(90.0));
    bool not_equal_rad = (rad1 != radians(2.0));

    degrees scaled_deg = deg1 * 2.0;
    radians scaled_rad = rad1 / 2.0;

    double deg_value = (double)deg1;
    double rad_value = (double)rad1;

    std::cout << "check with double is " << (check ? "correct" : "wrong")
              << "\n";
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
