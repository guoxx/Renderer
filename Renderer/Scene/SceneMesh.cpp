#include "SceneMesh.h"
#include "Renderer.h"
#include "utMath.h"

#include <cstdio>
#include <cerrno>
#include <cassert>

#if defined __APPLE__
#include <unistd.h>
#elif defined _WIN32 || defined _WIN64
#include <direct.h>
#endif

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

        // \r\n
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

SceneMesh::SceneMesh(std::string& file)
{
    FILE *f = fopen(file.c_str(), "rb");
    if (f == nullptr)
	{
		char msg[2048];
		char cwd[1024];
		getcwd(cwd, 1024);
        sprintf(msg, "%s:%d, cwd: %s, open file error: %s", __FILE__, __LINE__, cwd, strerror(errno));
		throw std::ios_base::failure{msg};
    }

    fseek(f, 0, SEEK_END);
    int sz = ftell(f);
    fseek(f, 0, SEEK_SET);
	assert(sz > 0);

	char* buf = (char*)malloc(sz);
    assert(fread(buf, 1, sz, f) == sz);

    struct FReader ud {sz, 0};

    _readint(&ud, buf, &_pointCnt);
    _readint(&ud, buf, &_polyCnt);

	_verticesPtr = std::unique_ptr<float[]>(new float[_pointCnt * 3]);
    _texcoordsPtr = std::unique_ptr<float[]>(new float[_pointCnt * 2]);
    _trianglesPtr = std::unique_ptr<int[]>(new int[_polyCnt * 3]);

	float* _vertices = _verticesPtr.get();
	float* _texcoords = _texcoordsPtr.get();
	int* _triangles = _trianglesPtr.get();

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
	free(buf);
}

void SceneMesh::setup(Renderer &renderer) {
    Vec3f eye = Vec3f(3.0f, 4.0f, 5.0f);
    Vec3f target = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);
    renderer.lookat(eye, target, up);
    renderer.viewport(0, 0, renderer.getWidth(), renderer.getHeight());
    renderer.setupViewParams(90.0f, 1.0f, 1.0f, 10.0f);
}

void SceneMesh::update(Renderer &renderer) {

}

void SceneMesh::render(Renderer &renderer){
    renderer.clearDepthBuffer(10.0f);
    renderer.clearColorBuffer(Vec3f(0, 0, 0));

	float* _vertices = _verticesPtr.get();
	float* _texcoords = _texcoordsPtr.get();
	int* _triangles = _trianglesPtr.get();

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

        renderer.renderTriangle(3, v, c, n, t, nullptr);
    }
}