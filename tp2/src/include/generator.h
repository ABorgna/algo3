#pragma once

#include <string>
#include <vector>
#include <stdint.h>

typedef void (*GeneratorCall) (const std::vector<uint64_t>&);

typedef struct Generator {
    Generator(std::string n, GeneratorCall gen) : name(n), call(gen) {}

    std::string name;
    GeneratorCall call;
} Generator;
