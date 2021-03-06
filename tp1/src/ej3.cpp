#include <interfaz.h>

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

typedef struct mkp_object_t {
    uint64_t id;
    uint64_t weight;
    int64_t value;
} mkp_object;

vector<uint64_t> bins(3,0);
vector<mkp_object> objects;

uint64_t mkp(vector<uint64_t> bins, vector<mkp_object> const &objetos,
             vector<vector<uint64_t>> &resultIds) {
    // O(π(bins) * count(objects) )

    // Si hay menos de 3 mochilas, consideramos las siguientes como
    // de tamaño 0

    // Para cada combinación de k_i pesos de las mochilas, con 0 <= k_i <=
    // capacidad(i)
    // calculamos el mayor valor conseguible ocupando exactamente ese espacio
    //
    // En cada combinación, para cada 0 <= j <= cantidad de objetos,
    // calculamos el optimo usando los primeros j objetos.

    vector<vector<vector<vector<int64_t>>>> mejorValor(
        bins[0] + 1,
        vector<vector<vector<int64_t>>>(
            bins[1] + 1,
            vector<vector<int64_t>>(bins[2] + 1,
                                    vector<int64_t>(objetos.size() + 1, -1))));

    vector<vector<vector<vector<int64_t>>>> binObjeto(
        bins[0] + 1,
        vector<vector<vector<int64_t>>>(
            bins[1] + 1,
            vector<vector<int64_t>>(bins[2] + 1,
                                    vector<int64_t>(objetos.size() + 1, -1))));

    // 0 objetos en mochilas de tamaño 0 tienen valor 0
    mejorValor[0][0][0][0] = 0;

    int64_t valorMax = 0;
    vector<uint64_t> sizesMax(3, 0);

    for (uint64_t w0 = 0; w0 <= bins[0]; w0++) {
        for (uint64_t w1 = 0; w1 <= bins[1]; w1++) {
            for (uint64_t w2 = 0; w2 <= bins[2]; w2++) {
                // Para cada primeros j objetos buscamos el máximo
                for (uint64_t j = 1; j <= objetos.size(); j++) {
                    const mkp_object &obj = objetos[j - 1];

                    // Decidimos si conviene poner el objeto en una mochila o no
                    // usarlo
                    int64_t valCandidato, valor = mejorValor[w0][w1][w2][j - 1];

                    valCandidato = obj.weight > w0 ? -1 :
                        mejorValor[w0 - obj.weight][w1][w2][j - 1] + obj.value;
                    if (valor < valCandidato) {
                        binObjeto[w0][w1][w2][j] = 0;
                        valor = valCandidato;
                    }

                    valCandidato = obj.weight > w1 ? -1 :
                        mejorValor[w0][w1 - obj.weight][w2][j - 1] + obj.value;
                    if (valor < valCandidato) {
                        binObjeto[w0][w1][w2][j] = 1;
                        valor = valCandidato;
                    }

                    valCandidato = obj.weight > w2 ? -1 :
                        mejorValor[w0][w1][w2 - obj.weight][j - 1] + obj.value;
                    if (valor < valCandidato) {
                        binObjeto[w0][w1][w2][j] = 2;
                        valor = valCandidato;
                    }

                    mejorValor[w0][w1][w2][j] = valor;
                }

                // Mantenemos el máximo valor visto
                if (valorMax < mejorValor[w0][w1][w2][objetos.size()]) {
                    valorMax = mejorValor[w0][w1][w2][objetos.size()];
                    sizesMax = {w0, w1, w2};
                }
            }
        }
    }

    // Escribimos los ids del resultado
    resultIds.resize(bins.size());

    for(uint64_t j=objetos.size(); j > 0; j--) {
        int64_t bin = binObjeto[sizesMax[0]][sizesMax[1]][sizesMax[2]][j];
        if (bin != -1) {
            resultIds[bin].push_back(objetos[j-1].id);
            sizesMax[bin] -= objetos[j-1].weight;
        }
    }

    // Como recorrimos los objetos desde el último hay que invertir el array
    // (se pide ordenado)
    for (auto &v : resultIds) {
        reverse(v.begin(), v.end());
    }

    return valorMax;
}

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream& is) {
    uint64_t m,n;

    is >> m >> n;

    bins = {0,0,0};
    objects.resize(0);

    for (uint64_t i = 0; i < m; i++) {
        is >> bins[i];
    }

    for (uint64_t i = 0; i < n; i++) {
        uint64_t count;
        mkp_object obj;

        is >> count >> obj.weight >> obj.value;
        obj.id = i+1;
        for (uint64_t j = 0; j < count; j++) objects.push_back(obj);
    }
}

int prob_solve(std::ostream &os) {
    // Solve
    vector<vector<uint64_t>> resultIds;
    uint64_t finalValue;

    // Checkeamos que haya al menos un objeto y al menos una mochila no vacía
    if(bins[0] == 0 || objects.empty()) {
        return 1;
    }

    finalValue = mkp(bins, objects, resultIds);

    // Print
    os << finalValue << endl;

    for (uint64_t b=0; b < bins.size() && bins[b] > 0; b++) {
        auto &bin = resultIds[b];
        os << bin.size();

        for (auto id : bin) {
            os << " " << id;
        }

        os << endl;
    }

    return 0;
}

void prob_reload() {
}

vector<uint64_t> prob_vars() {
    vector<uint64_t> res;

    // k0, k1, k2
    for(auto b : bins) res.push_back(b);

    // Sum Ci
    res.push_back(objects.size());

    return res;
}

void prob_print_input(std::ostream& os) {
    for(auto v : prob_vars()) os << v << " ";
    os << endl;
}

vector<Option> prob_custom_options() {
    return {};
}

void generator_random(const std::vector<uint64_t>& v) {
    prob_reload();

    bins[0] = v[0];
    bins[1] = v[1];
    bins[2] = v[2];
    uint64_t sumC = v[3];

    uint64_t kmax = max(v[0],max(v[1],v[2]));

    objects.resize(0);
    for(uint64_t i=0; i<sumC; i++) {
        objects.push_back({i, rnd(1,kmax), (int64_t)rnd(1,1000000)});
    }
}

vector<Generator> prob_generators() {
    return {{"random", generator_random}};
}

