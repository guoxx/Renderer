#ifndef __RENDERER__TRIVIALCOLORFP__
#define __RENDERER__TRIVIALCOLORFP__

#include "FragmentProcessor.h"
#include "Fragment.h"
#include "FrameBuffer.h"

class TrivialColorFP : public FragmentProcessor {
    void fragment(Fragment &frag, FrameBuffer &framebuffer){
        framebuffer.setColorBuffer(frag.x, frag.y, frag.color);
    }
};

#endif /* defined(__RENDERER__TRIVIALCOLORFP__) */
