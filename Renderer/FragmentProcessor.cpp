#include "FragmentProcessor.h"
#include "Fragment.h"
#include "FrameBuffer.h"

void FragmentProcessor::fragment(Fragment *frag, FrameBuffer *fb){
    fb->setColorBuffer(frag->x, frag->y, frag->color);
}
