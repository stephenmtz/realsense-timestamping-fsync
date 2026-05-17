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

    std::vector<std::unique_ptr<RealSenseStream>> streams;

    for (auto& cam : cameras) {
        try {
            auto s = std::make_unique<RealSenseStream>(cam);
            s->start();
            streams.push_back(std::move(s));
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } catch (const rs2::error& e) {
            std::cerr << "[" << cam.serial << "] failed to start: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "[" << cam.serial << "] error: " << e.what() << "\n";
        }
    }

    std::cout << streams.size() << "/" << cameras.size() << " cameras streaming\n";

    while (running)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (auto& s : streams)
        s->stop();

    return 0;
}