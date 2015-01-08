#pragma once

class Fragment;
class FrameBuffer;

class FragmentProcessor {
public:
    virtual void fragment(Fragment &frag, FrameBuffer &framebuffer) = 0;
};