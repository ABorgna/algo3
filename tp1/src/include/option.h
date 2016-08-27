#pragma once

#include <stdint.h>
#include <string>
#include <vector>

typedef int (*OptionCall) (const std::vector<std::string>&);

typedef struct Option {
    Option(char s, std::string b, int32_t p, bool a, OptionCall c, 
           std::string ha, std::string hd);

    std::string small;
    std::string big;
    int32_t param_count;
    bool is_action;
    OptionCall call;
    std::string help_args;
    std::string help_desc;
} Option;

