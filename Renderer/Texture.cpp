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

bool Texture::loadTga(const char &file) {
    FILE *f = fopen(&file, "r");
    if (f == NULL)
        return false;

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, sizeof(struct TgaHeader), SEEK_SET);

    struct TgaHeader hdr;
    fread((void*)&hdr, 1, sizeof(hdr), f);

    _width = hdr.width;
    _height = hdr.height;

    _size = sz - sizeof(struct TgaHeader);
    _data = (unsigned char*)malloc(_size);
    if (fread(_data, 1, _size, f) != _size) {
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
    fwrite(_data, 1, _size, f);
    fclose(f);

    return true;
}