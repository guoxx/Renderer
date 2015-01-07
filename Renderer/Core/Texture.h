#pragma once

#include <cstdlib>
#include <cstdint>
#include <sys/types.h>
#include "utMath.h"

class Texture {
public:
    Texture()
    : _width(0)
    , _height(0)
    , _size(0)
    , _data(NULL) {
    }

    ~Texture() {
        if (_data != NULL) {
            free(_data);
            _data = NULL;
        }
    }

    Vec3f getColor(float u, float v);
    bool loadTga(const char &file);
    bool dumpTga(const char &file, bool overwrite);

private:
    int _width;
    int _height;
    long _size;
    uint8_t *_data;
};