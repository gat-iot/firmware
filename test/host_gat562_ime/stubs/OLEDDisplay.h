#pragma once
#include <cstring>
#include <string>
#include <vector>
enum { BLACK, WHITE };
class OLEDDisplay {
  public:
    struct Line { int x, y, w; };
    std::vector<Line> lines;
    int screenWidth = 128;
    int getWidth() const { return screenWidth; }
    int getHeight() const { return 64; }
    int width() const { return screenWidth; }
    void setColor(int) {}
    void setFont(const uint8_t *) {}
    int getStringWidth(const char *s, size_t len, bool = false) const {
        int w = 0;
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = s[i];
            if ((c & 0xc0) != 0x80)
                w += c < 128 ? 6 : 10;
        }
        return w;
    }
    int getStringWidth(const char *s) const { return getStringWidth(s, strlen(s)); }
    void drawString(int, int, const char *) {}
    void drawHorizontalLine(int x, int y, int w) { lines.push_back({x, y, w}); }
    void drawVerticalLine(int, int, int) {}
    void fillRect(int, int, int, int) {}
};
