#pragma once

#include <librealsense2/rs.hpp>
#include <vector>
#include <string>

struct CameraInfo {
    std::string serial;
    std::string physical_port;
    std::string firmware_version;
    rs2::device device; 
};

std::vector<CameraInfo> enumerate_cameras(const rs2::context& ctx);