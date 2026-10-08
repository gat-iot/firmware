#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#define private public
#include "../../src/graphics/VirtualKeyboard.h"
#undef private
#include "../../src/graphics/VirtualKeyboard.cpp"

int main()
{
    graphics::VirtualKeyboard kb;
    OLEDDisplay display;
#if defined(CJK_IME_ZHUYIN)
    kb.toggleIME();
#endif
    for (char c : std::string("abcABC2")) {
        testMillis += 100;
        kb.handleT9Character(c);
        assert(kb.getInputText() == std::string(1, c));
    }
    kb.handlePress();
    assert(kb.getInputText() == "2");
#if defined(VK_HAS_CJK_IME)
    kb.toggleIME();
    const std::string seed = "\xe4\xb8\xad";
    kb.setInputText(seed);
    kb.draw(&display, 0, 0);
    assert(kb.selectableChars > 1);
    const std::string first = kb.selectList.substr(0, kb.selectListLayout[0]);
#if defined(CJK_IME_ZHUYIN)
    assert(first == kb.bpmfEngine.candidates()[0].substr(seed.size()));
#else
    assert(first == kb.pinyinCandidates[0].substr(seed.size()));
#endif
    kb.moveCursorRight();
    display.lines.clear();
    kb.draw(&display, 0, 0);
    assert(kb.candidateCursor == 1);
    assert(!display.lines.empty());
    assert(display.lines[0].x == display.getStringWidth(first.c_str()));
    kb.moveCursorLeft();
    assert(kb.candidateCursor == 0);
    kb.moveCursorDown();
    assert(kb.candidateCursor == 1);
    kb.moveCursorUp();
    assert(kb.candidateCursor == 0);
    const std::string expected = seed + kb.selectList.substr(0, kb.selectListLayout[0]);
    assert(expected.size() > seed.size() && expected.compare(0, seed.size(), seed) == 0);
    kb.handlePress();
    assert(kb.getInputText() == expected);
    assert(kb.processedWords == expected.size());
    testMillis += 100;
    kb.handleT9Character('a');
    assert(kb.getInputText().compare(0, expected.size(), expected) == 0);
    kb.handleBackspace();
    assert(kb.getInputText() == expected);
    kb.handleBackspace();
    assert(kb.getInputText().size() == expected.size() - 3);

#if defined(VK_HAS_PINYIN_PREDICTION)
    kb.setInputText("");
    kb.handleCharacter('z');
    kb.draw(&display, 0, 0);
    assert(!kb.pinyinPrediction && kb.selectableChars == 3);
    kb.handlePress();
    assert(kb.getInputText() == seed);
    kb.draw(&display, 0, 0);
    assert(kb.pinyinPrediction && kb.selectableChars > 0);
#endif

    kb.setInputText(seed);
    kb.candidateQuery = "p" + seed;
    std::vector<std::string> candidates = {
        seed + seed, seed + seed + seed, seed + seed + seed + seed,
        seed + seed, seed + seed + seed, seed + seed};
#if defined(CJK_IME_ZHUYIN)
    kb.bpmfEngine.queryKey_ = kb.candidateQuery;
    kb.bpmfEngine.prediction_ = true;
    kb.bpmfEngine.candidates_ = candidates;
#else
    kb.pinyinPrediction = true;
    kb.pinyinCandidates = candidates;
#endif
    display.screenWidth = 70;
    kb.selectListOffset = 0;
    kb.candidateCursor = 0;
    kb.candidatePageStarts.clear();
    kb.draw(&display, 0, 0);
    const int firstCount = kb.selectableChars;
    assert(firstCount == 3);
    kb.candidateCursor = firstCount - 1;
    kb.moveCursorRight();
    kb.draw(&display, 0, 0);
    assert(kb.selectListOffset == firstCount && kb.candidateCursor == 0);
    kb.moveCursorLeft();
    display.lines.clear();
    kb.draw(&display, 0, 0);
    assert(kb.selectListOffset == 0 && kb.candidateCursor == firstCount - 1);
    assert(display.lines[0].x == 30);
    kb.handlePress();
    assert(kb.getInputText() == candidates[firstCount - 1]);
    kb.toggleIME();
    assert(kb.selectableChars == 0);
#endif
    std::string sent;
    kb.setCallback([&](const std::string &text) { sent = text; });
    kb.handleLongPress();
    assert(sent == kb.getInputText());
    std::cout << "PASS: case, selection, highlight, word paging, prediction commit, delete, submit\n";
}
