#pragma once

#include "gat562_pinyin_predictions.h"
#include <cstring>
#include <string>
#include <vector>

inline std::vector<std::string> gat562PredictAfter(const std::string &lastChar)
{
    std::vector<std::string> result;
    if (lastChar.size() != 3)
        return result;
    for (const char *word = gat562PinyinPredictions; *word; word += strlen(word) + 1) {
        const int order = strncmp(word, lastChar.c_str(), lastChar.size());
        if (order > 0)
            break;
        if (order == 0)
            result.emplace_back(word);
    }
    return result;
}
