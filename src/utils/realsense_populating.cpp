#include "realsense_populating.h"
#include <iostream>

std::vector<CameraInfo> enumerate_cameras(const rs2::context& ctx) {
    std::vector<CameraInfo> cameras;

    auto devices = ctx.query_devices();

    if (devices.size() == 0) {
        std::cerr << "[enumerate] no RealSense devices found\n";
        return cameras;
    }

    for (auto dev : devices) {
        CameraInfo info;
        info.serial           = dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
        info.physical_port    = dev.get_info(RS2_CAMERA_INFO_PHYSICAL_PORT);
        info.firmware_version = dev.get_info(RS2_CAMERA_INFO_FIRMWARE_VERSION);

        std::cout << "[enumerate] found camera"
                  << "  serial="   << info.serial
                  << "  port="     << info.physical_port
                  << "  fw="       << info.firmware_version
                  << "\n";

        cameras.push_back(info);
    }

    return cameras;
}