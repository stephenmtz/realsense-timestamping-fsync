#pragma once

#include <librealsense2/rs.hpp>
#include <atomic>
#include <thread>
#include "realsense_populating.h"

class RealSenseStream {
public:
    explicit RealSenseStream(const CameraInfo& info);
    ~RealSenseStream();

    void start();
    void stop();

private:
    void stream_loop();
    void configure_sync();

    CameraInfo        info_;
    rs2::pipeline     pipe_;
    rs2::config       cfg_;
    std::thread       thread_;
    std::atomic<bool> running_{false};
};