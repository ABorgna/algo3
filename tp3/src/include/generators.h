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

// All the gyms are on x=0 and all the stops on x=16383
// bagsize=3 and gymPower=3
void zigzagGenerator(const vector<uint64_t>&);
