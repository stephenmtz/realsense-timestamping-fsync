#include "realsense_streaming.h"
#include <iostream>
#include <iomanip>

RealSenseStream::RealSenseStream(const CameraInfo& info)
    : info_(info) {}

RealSenseStream::~RealSenseStream() {
    stop();
}

void RealSenseStream::configure_sync(const rs2::pipeline_profile& profile) {
    try {
        auto depth_sensor = profile.get_device().first<rs2::depth_sensor>();
        if (depth_sensor.supports(RS2_OPTION_INTER_CAM_SYNC_MODE)) {
            depth_sensor.set_option(RS2_OPTION_INTER_CAM_SYNC_MODE, 3);
            std::cout << "[" << info_.serial << "] sync mode set to 3\n";
        }
    } catch (const rs2::error& e) {
        std::cerr << "[" << info_.serial << "] sync mode error: " << e.what() << "\n";
    }
}

void RealSenseStream::start() {
    cfg_.enable_device(info_.serial);
    cfg_.enable_stream(RS2_STREAM_DEPTH, -1, -1, RS2_FORMAT_Z16,  -1);
    cfg_.enable_stream(RS2_STREAM_COLOR, -1, -1, RS2_FORMAT_YUYV, -1);

    auto profile = pipe_.start(cfg_);
    
    configure_sync(profile);
    
    running_ = true;
    thread_ = std::thread(&RealSenseStream::stream_loop, this);
    std::cout << "[" << info_.serial << "] streaming started\n";
}

void RealSenseStream::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
    pipe_.stop();
    std::cout << "[" << info_.serial << "] stopped\n";
}

void RealSenseStream::stream_loop() {
    while (running_) {
        rs2::frameset frames;
        if (!pipe_.try_wait_for_frames(&frames, 2000)) {
            std::cerr << "[" << info_.serial << "] timeout\n";
            continue;
        }

        auto depth = frames.get_depth_frame();
        auto color = frames.get_color_frame();

        std::cout << std::fixed << std::setprecision(3)
                  << "[" << info_.serial << "]"
                  << "  frame="     << depth.get_frame_number()
                  << "  depth_ts="  << depth.get_timestamp()
                  << "  color_ts="  << color.get_timestamp()
                  << "  domain="    << rs2_timestamp_domain_to_string(
                                         depth.get_frame_timestamp_domain())
                  << "\n";
    }
}