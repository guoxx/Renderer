#ifndef __FRAGMENTPROCESSOR_H__
#define __FRAGMENTPROCESSOR_H__

struct Fragment;
class FrameBuffer;

class FragmentProcessor {
public:
    virtual void fragment(Fragment &frag, FrameBuffer &framebuffer) = 0;
};

#endif
