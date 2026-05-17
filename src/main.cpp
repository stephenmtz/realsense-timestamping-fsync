#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>
#include <memory>
#include <thread>
#include <chrono>
#include "utils/realsense_populating.h"
#include "utils/realsense_streaming.h"

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main() {
    std::signal(SIGINT, signal_handler);

    rs2::context ctx;
    auto cameras = enumerate_cameras(ctx);

    if (cameras.empty()) {
        std::cerr << "No cameras found\n";
        return 1;
    }

    std::cout << "Testing single camera: " << cameras[0].serial << "\n";

    try {
        RealSenseStream s(cameras[0]);
        s.start();

        while (running)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        s.stop();
    } catch (const rs2::error& e) {
        std::cerr << "RealSense error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}