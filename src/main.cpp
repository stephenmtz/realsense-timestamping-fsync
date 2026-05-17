#include "utils/realsense_populating.h"
#include <iostream>

int main() {
    rs2::context ctx;

    auto cameras = enumerate_cameras(ctx);
    if (cameras.empty()) {
        std::cerr << "No cameras found, exiting\n";
        return 1;
    }

    std::cout << cameras.size() << " camera(s) ready\n";
    return 0;
}