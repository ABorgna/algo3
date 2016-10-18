#pragma once

#include <pokegraph.h>

bool esCaminoValido(vector<int64_t>::iterator start,
                    vector<int64_t>::iterator end, int64_t bagSize,
                    const PokeGraph&);
bool esCaminoValido(vector<int64_t>::iterator start,
                    vector<int64_t>::iterator end, int64_t bagSize,
                    const PokeGraph&, int64_t& powerAcum);

double distanciaCamino(vector<int64_t>::iterator start,
                       vector<int64_t>::iterator end, const PokeGraph&);
