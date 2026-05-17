#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>
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
        std::cerr << "No cameras found, exiting\n";
        return 1;
    }

    std::vector<RealSenseStream> streams;
    streams.reserve(cameras.size());
    for (auto& cam : cameras)
        streams.emplace_back(cam);

    for (auto& s : streams)
        s.start();

    std::cout << "Streaming " << streams.size() << " cameras, Ctrl+C to stop\n";

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\nShutting down...\n";
    for (auto& s : streams)
        s.stop();

    return 0;
}