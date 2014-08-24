#ifndef __FRAGMENTPROCESSOR_H__
#define __FRAGMENTPROCESSOR_H__

class Fragment;
class FrameBuffer;

class FragmentProcessor {
public:
    void fragment(Fragment *frag, FrameBuffer *fb);
};

#endif
