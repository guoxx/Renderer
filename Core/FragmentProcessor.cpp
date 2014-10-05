#include "FragmentProcessor.h"
#include "Fragment.h"
#include "FrameBuffer.h"

void FragmentProcessor::fragment(Fragment &frag, FrameBuffer &framebuffer){
    framebuffer.setColorBuffer(frag.x, frag.y, frag.color);
}
