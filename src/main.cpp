#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>
#include <memory>
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

    std::vector<std::unique_ptr<RealSenseStream>> streams;
    for (auto& cam : cameras)
        streams.push_back(std::make_unique<RealSenseStream>(cam));

    for (auto& s : streams)
        s->start();

    while (running)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (auto& s : streams)
        s->stop();

    return 0;
}