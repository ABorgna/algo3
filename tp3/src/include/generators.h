#pragma once

#include <generator.h>
#include <pokegraph.h>

#include <vector>

void setupGraph(PokeGraph*);
vector<Generator> defaultGenerators();

// Random position for each node
void randomGenerator(const vector<uint64_t>&);

// Generate a cluster with all the gyms and a separated cluster with the stops
void separatedGenerator(const vector<uint64_t>&);
