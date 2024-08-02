#pragma once

constexpr auto PX_SIZE = sizeof(unsigned char[3]);

struct bmp {
    int width;
    int height;
    unsigned char* content;
};
bmp* load_bmp(std::string path);