#include "Texture.h"

#include <cstdio>
#include <cassert>
#include <cerrno>
#include <cstring>

#include <sys/stat.h>
#include <unistd.h>

#pragma pack(1)
struct TgaHeader {
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short int colourmaporigin;
    short int colourmaplength;
    char  colourmapdepth;
    short int originx;
    short int originy;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack()

static_assert(sizeof(struct TgaHeader) == 18, "sizeof TGA file header must be 18");

Vec3f Texture::getColor(float u, float v){
    int x = floorf(u * _width);
    int y = floorf(v * _height);
    int idx = y * _width + x;
    uint8_t *p = _data + idx * 3;
    return Vec3f(*(p + 2)/255.0f,
                 *(p + 1)/255.0f,
                 *(p + 0)/255.0f);
}

bool Texture::loadTga(const char &file) {
    FILE *f = fopen(&file, "rb");
    if (f == NULL) {
        printf("open tga file failed, error %s\n", strerror(errno));
        return false;
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    assert(sz > sizeof(struct TgaHeader));

    struct TgaHeader hdr;
    fread((void*)&hdr, sizeof(hdr), 1, f);
    // only support Uncompressed, RGB images
    assert(hdr.datatypecode == 2);

    _width = hdr.width;
    _height = hdr.height;

    _size = sz - sizeof(struct TgaHeader);
    _data = (unsigned char*)malloc(_size);
    size_t used = fread(_data, 1, _size, f);
    if (used != _size) {
        free(_data);
        _data = NULL;
        _size = 0;
        _width = 0;
        _height = 0;
        fclose(f);
        return false;
    }
    else {
        fclose(f);
        return true;
    }
}

bool Texture::dumpTga(const char &file, bool overwrite) {
    bool fileExist = (stat(&file, NULL) == 0);
    if (fileExist && overwrite) {
        if ( unlink(&file) != 0 ) {
            printf("delete file %s failed! error : %s\n", &file, strerror(errno));
            return false;
        }
    }

    FILE *f = fopen(&file, "wb");
    if (f == NULL)
        return false;

    struct TgaHeader hdr;
    bzero(&hdr, sizeof(hdr));
    hdr.datatypecode = 2;
    hdr.width = _width;
    hdr.height = _height;
    hdr.bitsperpixel = 24;
    fwrite(&hdr, 1, sizeof(struct TgaHeader), f);
    for (int y = 0; y < _height; y = y + 1) {
        for (int x = 0; x < _width; x = x + 1) {
            int idx = y * _width + x;
            uint32_t clr = _data[idx];
            uint8_t* p = (uint8_t*)&clr;
            // write BGR
            fwrite(p + 2, sizeof(uint8_t), 1, f);
            fwrite(p + 1, sizeof(uint8_t), 1, f);
            fwrite(p + 0, sizeof(uint8_t), 1, f);
        }
    }

    fwrite(_data, 1, _size, f);
    fclose(f);

    return true;
}