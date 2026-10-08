#pragma once
namespace graphics {
class Screen {
  public:
    enum { FOCUS_PRESERVE };
    void setFrames(int) {}
};
}
inline graphics::Screen *screen = nullptr;
