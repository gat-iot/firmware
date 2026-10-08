#pragma once
#include <algorithm>
#include <cstdint>
#define HAS_SCREEN 1
#define OLED_CJK_SIZE 10
#define OLEDDISPLAY_UTF8_TOP_PADDING 2
#define LOG_INFO(...) ((void)0)
inline uint32_t testMillis = 1000;
inline uint32_t millis() { return testMillis; }
