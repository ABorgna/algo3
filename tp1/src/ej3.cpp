#include <interfaz.h>

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef struct mkp_object_t {
    uint32_t id;
    uint32_t weight;
    int32_t value;
} mkp_object;

uint32_t m, n;
vector<uint32_t> bins;
vector<mkp_object> objects;

uint32_t mkp(vector<uint32_t> bins, vector<mkp_object> const &objetos,
             vector<vector<uint32_t>> &resultIds) {
    // O(π(bins) * count(objects) )

    // Si hay menos de 3 mochilas, consideramos las siguientes como
    // de tamaño 0
    bins.resize(3, 0);

    // Para cada combinación de k_i pesos de las mochilas, con 0 <= k_i <=
    // capacidad(i)
    // calculamos el mayor valor conseguible ocupando exactamente ese espacio
    //
    // En cada combinación, para cada 0 <= j <= cantidad de objetos,
    // calculamos el optimo usando los primeros j objetos.

    vector<vector<vector<vector<int32_t>>>> mejorValor(
        bins[0] + 1,
        vector<vector<vector<int32_t>>>(
            bins[1] + 1,
            vector<vector<int32_t>>(bins[2] + 1,
                                    vector<int32_t>(objetos.size() + 1, -1))));

    vector<vector<vector<vector<int32_t>>>> binObjeto(
        bins[0] + 1,
        vector<vector<vector<int32_t>>>(
            bins[1] + 1,
            vector<vector<int32_t>>(bins[2] + 1,
                                    vector<int32_t>(objetos.size() + 1, -1))));

    // 0 objetos en mochilas de tamaño 0 tienen valor 0
    mejorValor[0][0][0][0] = 0;

    int32_t valorMax = 0;
    vector<uint32_t> sizesMax(3, 0);

    for (uint32_t w0 = 0; w0 <= bins[0]; w0++) {
        for (uint32_t w1 = 0; w1 <= bins[1]; w1++) {
            for (uint32_t w2 = 0; w2 <= bins[2]; w2++) {
                // Para cada primeros j objetos buscamos el máximo
                for (uint32_t j = 1; j <= objetos.size(); j++) {
                    const mkp_object &obj = objetos[j - 1];

                    // Decidimos si conviene poner el objeto en una mochila o no
                    // usarlo
                    int32_t valCandidato, valor = mejorValor[w0][w1][w2][j - 1];

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

    for(uint32_t j=objetos.size(); j > 0; j--) {
        int32_t bin = binObjeto[sizesMax[0]][sizesMax[1]][sizesMax[2]][j];
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

void prob_load(std::istream& is) {
    is >> m >> n;
    bins.reserve(m);
    objects.reserve(n);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t budget;
        is >> budget;
        bins.push_back(budget);
    }
    for (uint32_t i = 0; i < n; i++) {
        uint32_t count;
        mkp_object obj;

        is >> count >> obj.weight >> obj.value;
        obj.id = i;
        for (uint32_t j = 0; j < count; j++) objects.push_back(obj);
    }
}

int prob_solve(std::ostream &os) {
    // Solve
    vector<vector<uint32_t>> resultIds;
    uint32_t finalValue;

    finalValue = mkp(bins, objects, resultIds);

    // Print
    os << finalValue << endl;

    for (uint32_t b=0; b < m; b++) {
        auto &bin = resultIds[b];
        os << bin.size();

        for (auto id : bin) {
            os << " " << id;
        }

        os << endl;
    }

    return 0;
}

