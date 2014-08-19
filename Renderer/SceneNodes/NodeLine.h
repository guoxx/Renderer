#ifndef __RENDERER__NODELINE__
#define __RENDERER__NODELINE__

#include "RenderableProtocol.h"
#include "utMath.h"

class NodeLine : public RenderableProtocol {
public:
    void setup(const Vec3f& s, const Vec3f& e);
private:
    Vec3f m_start;
    Vec3f m_end;
};

#endif /* defined(__RENDERER__NODELINE__) */
