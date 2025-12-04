#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "lib/Graph_lib/Graph.h"
#include "lib/Graph_lib/Simple_window.h"
#include "lib/poly/poly.h"

using namespace Graph_lib;

#define SIZE 200
#define MAXITER 5
#define WINW 800
#define WINH 600
#define SCALING 1.5

void
next_line(Simple_window& win,
          std::vector<std::shared_ptr<Graph_lib::Line>>& lines,
          Point p,
          int iter    = MAXITER,
          float scale = 1,
          int angle   = 0) {
    if (!iter)
        return;
    float angleD = (M_PI / 3.24) * angle;
    auto N       = lines.size();
    Point tmp_point{ p.x + (int)(SIZE * scale * std::sin(angleD)),
                     p.y - (int)(SIZE * scale * std::cos(angleD)) };
    std::shared_ptr<Graph_lib::Line> newline(new Graph_lib::Line(p, tmp_point));
    lines.emplace_back(newline);
    win.attach(*newline);
    next_line(win, lines, tmp_point, iter - 1, scale / SCALING, angle + 1);
    next_line(win, lines, tmp_point, iter - 1, scale / SCALING, angle - 1);
}

void
draw_funny(int win_w, int win_h) {
    Point win_lt{ 100, 100 }; // window left top corner
    Simple_window win{ win_lt, win_w, win_h, "Fibonacci spiral" };

    std::vector<std::shared_ptr<Graph_lib::Line>> lines;

    next_line(win, lines, { win_w / 2, win_h }, 14);
    win.wait_for_button();
}

int
main() try {
    draw_funny(WINW, WINH);
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
} catch (...) {
    std::cerr << "Oops, something went wrong..." << std::endl;
    return 2;
}
