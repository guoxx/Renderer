#include "SceneMesh.h"
#include "Renderer.h"
#include "../utMath.h"

#include <cstdio>
#include <cerrno>
#include <cassert>

struct FReader {
    int size;
    int cursor;
};

static char *_readline(struct FReader *reader, char *buf) {
    if (reader->cursor > reader->size)
        return nullptr;

    int sz = reader->size;
    int mark = reader->cursor;
    int idx = reader->cursor;
    while (true) {
        if (idx > sz) {
            mark = -1;
            break;
        }

        if (buf[idx] != '\n') {
            idx = idx + 1;
        }
        else {
            buf[idx - 1] = '\0';
            reader->cursor = idx + 1;
            break;
        }
    }

    return mark >= 0 ? buf + mark : nullptr;
}

static bool _readint(struct FReader *reader, char *buf, int *val) {
    char *data = _readline(reader, buf);
    if (data != nullptr) {
        sscanf(data, "%d", val);
    }
    return false;
}

static bool _readfloat(struct FReader *reader, char *buf, float *val) {
    char *data = _readline(reader, buf);
    if (data != nullptr) {
        sscanf(data, "%f", val);
    }
    return false;
}

SceneMesh::SceneMesh()
: _pointCnt(0)
, _polyCnt(0)
, _vertices(nullptr)
, _texcoords(nullptr)
, _triangles(nullptr){
}

SceneMesh::~SceneMesh() {
    if (_vertices != nullptr) delete [] _vertices;
    if (_texcoords != nullptr) delete [] _texcoords;
    if (_triangles != nullptr) delete [] _triangles;
}

bool SceneMesh::load(const char &file){
    FILE *f = fopen(&file, "rb");
    if (f == NULL) {
        printf("%s:%d, open file error: %s", __FILE__, __LINE__, strerror(errno));
        return false;
    }

    fseek(f, 0, SEEK_END);
    int sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[sz];
    assert(fread(buf, 1, sz, f) == sz);

    struct FReader ud {sz, 0};

    _readint(&ud, buf, &_pointCnt);
    _readint(&ud, buf, &_polyCnt);

    _vertices = new float[_pointCnt * 3];
    _texcoords = new float[_pointCnt * 2];
    _triangles = new int[_polyCnt * 3];

    char *line = _readline(&ud, buf);
    while (line != nullptr) {
        if (strcmp(line, "vertices") == 0) {
            for (int i = 0; i < _pointCnt * 3; ++i) {
                _readfloat(&ud, buf, _vertices + i);
            }
        }
        else if( strcmp(line, "texcoords") == 0) {
            for (int i = 0; i < _pointCnt * 2; ++i) {
                _readfloat(&ud, buf, _texcoords + i);
            }
        }
        else if( strcmp(line, "triangles") == 0) {
            for (int i = 0; i < _polyCnt * 3; ++i) {
                _readint(&ud, buf, _triangles + i);
            }
        }
        line = _readline(&ud, buf);
    }
    return true;
}

void SceneMesh::render(Renderer &renderer){
    for (int i = 0; i < _polyCnt * 3; i = i + 3) {
        int iv0 = 3 * _triangles[i + 0];
        int iv1 = 3 * _triangles[i + 1];
        int iv2 = 3 * _triangles[i + 2];

        Vec3f v[3] = {
            Vec3f(_vertices[iv0], _vertices[iv0 + 1], _vertices[iv0 + 2]),
            Vec3f(_vertices[iv1], _vertices[iv1 + 1], _vertices[iv1 + 2]),
            Vec3f(_vertices[iv2], _vertices[iv2 + 1], _vertices[iv2 + 2])
        };

        Vec3f c[3] = {
            Vec3f(1, 0, 0),
            Vec3f(0, 1, 0),
            Vec3f(0, 0, 1)
        };

        Vec3f n[3] = {
            Vec3f(0, 0, 1),
            Vec3f(0, 0, 1),
            Vec3f(0, 0, 1)
        };

        Vec3f t[3] = {
            Vec3f(_texcoords[_triangles[i + 0] * 2], _texcoords[_triangles[i + 0] * 2 + 1], 0),
            Vec3f(_texcoords[_triangles[i + 1] * 2], _texcoords[_triangles[i + 1] * 2 + 1], 0),
            Vec3f(_texcoords[_triangles[i + 2] * 2], _texcoords[_triangles[i + 2] * 2 + 1], 0)
        };

        renderer.renderTriangle(3, v, c, n, t, NULL);
    }
}