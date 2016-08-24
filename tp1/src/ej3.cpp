
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef struct mkp_object_t {
    uint32_t count;
    uint32_t weight;
    int32_t value;
} mkp_object;

uint32_t mkp(vector<uint8_t> const &bins, vector<mkp_object> const &objetos,
             vector<vector<uint32_t>> &resultIds) {
    // O(π(bins) * count(objects) )

    // Para cada combinación de k_i pesos de las mochilas, con 0 <= k_i <=
    // capacidad(i)
    // calculamos el mayor valor conseguible ocupando exactamente ese espacio
    //
    // En cada combinación, para cada 0 <= j <= cantidad de objetos,
    // calculamos el optimo usando los primeros j objetos.

    // Cantidad total de objetos
    uint32_t totObjetos = 0;
    vector<uint32_t> objetosFlatten;
    for (uint32_t i = 0; i < objetos.size(); i++) {
        const mkp_object &obj = objetos[i];
        totObjetos += obj.count;

        objetosFlatten.reserve(objetosFlatten.size() + obj.count);
        for (uint32_t j = 0; j < obj.count; j++) objetosFlatten.push_back(i);
    }

    // m tiene como claves los pesos de las mochilas
    // y como valor la máxima ganancia y su combinación de objetos necesaria en
    // cada mochila
    // Una ganancia -1 significa que no hay una configuración posible
    typedef struct valor_combinacion_t {
        int32_t valor;
        bool usaObjetoJ;
        int32_t binObjetoJ;
        int32_t objetoAnterior;
    } valor_combinacion;

    map<vector<uint8_t>, vector<valor_combinacion>> m;

    // Recorremos cada combinacion
    bool done = false;
    vector<uint8_t> combinacion(bins.size(), 0);
    vector<uint8_t> combinacionMax = combinacion;
    int32_t valorMax = -1;

    // Todas las mochilas en 0 se optimizan con 0
    vector<valor_combinacion> vcs_tmp(totObjetos + 1, {0, false, -1, -1});
    m.insert({combinacion, vcs_tmp});
    combinacion[0]++;

    while (!done) {

        // Agregamos la combinación a la lista
        // Inicializamos la ganancia con 0 objetos a 0
        auto out =
            m.insert({combinacion, vector<valor_combinacion>(
                                       totObjetos + 1, {-1, false, -1, -1})});
        vector<valor_combinacion> &vcs = out.first->second;

        // Para cada primeros j objetos buscamos el máximo
        uint32_t j = 0;
        for (auto obj : objetos) {
            for (uint32_t cnt = 0; cnt < obj.count; cnt++) {
                j++;

                // Decidimos a qué bin conviene agregar el objeto,
                // o si conviene no usarlo
                int32_t mejorBin = -1;
                valor_combinacion *mejorVc = &vcs[j - 1];
                int32_t mejorValor = mejorVc->valor;

                for (uint8_t bin = 0; bin < bins.size(); bin++) {
                    // No podemos agregar el objeto al bin si no entra
                    if (combinacion[bin] < obj.weight)
                        continue;

                    // Veamos si combiene usar este bin
                    vector<uint8_t> comb(combinacion);
                    comb[bin] -= obj.weight;
                    valor_combinacion &vc_a_agregar = m[comb][j - 1];

                    if (vc_a_agregar.valor < 0) {
                        continue;  // No había una combinación de objetos válida
                                   // para esta cantidad
                    }

                    if (mejorValor <= vc_a_agregar.valor + obj.value) {
                        mejorBin = bin;
                        mejorVc = &vc_a_agregar;
                        mejorValor = vc_a_agregar.valor + obj.value;
                    }
                }

                vcs[j] = *mejorVc;

                vcs[j].objetoAnterior =
                    mejorVc->usaObjetoJ ? j - 1 : mejorVc->objetoAnterior;
                vcs[j].usaObjetoJ = false;

                if (mejorBin != -1) {
                    // Usamos el objeto, hay que marcarlo en el vc que copiamos
                    vcs[j].valor += obj.value;
                    vcs[j].usaObjetoJ = true;
                    vcs[j].binObjetoJ = mejorBin;
                }

                // Recalcular el máximo
                if (valorMax < 0 || valorMax < vcs[j].valor) {
                    valorMax = vcs[j].valor;
                    combinacionMax = combinacion;
                }
            }
        }

        // Calcular la siguiente combinación
        for (uint32_t i = 0; i < bins.size(); i++) {
            combinacion[i]++;
            if (combinacion[i] > bins[i]) {
                combinacion[i] = 0;
                if (i == bins.size() - 1)
                    done = true;
            } else {
                break;
            }
        }
    }

    // Escribimos los ids del resultado
    resultIds.resize(bins.size());
    vector<uint8_t> combinacionBase(bins.size(), 0);
    uint32_t currentObj = totObjetos;
    while (combinacionMax != combinacionBase) {
        const valor_combinacion &vc = m[combinacionMax][currentObj];

        if (vc.usaObjetoJ) {
            uint32_t objId = objetosFlatten[currentObj - 1];
            const mkp_object &obj = objetos[objId];

            resultIds[vc.binObjetoJ].push_back(objId);

            combinacionMax[vc.binObjetoJ] -= obj.weight;
        }
        currentObj = vc.objetoAnterior;
    }

    // Como recorrimos los objetos desde el último hay que invertir el array
    // (lo piden ordenado)
    for(auto &v : resultIds) {
        reverse(v.begin(), v.end());
    }

    return valorMax;
}

int main() {
    // Parse
    int m, n;
    vector<uint8_t> bins;
    vector<mkp_object> objects;

    cin >> m >> n;
    bins.reserve(m);
    objects.reserve(n);

    for (int i = 0; i < m; i++) {
        uint32_t budget;
        cin >> budget;
        bins.push_back(budget);
    }

    for (int i = 0; i < n; i++) {
        mkp_object obj;
        cin >> obj.count >> obj.weight >> obj.value;
        objects.push_back(obj);
    }

    // Solve
    vector<vector<uint32_t>> resultIds;
    uint32_t finalValue;

    finalValue = mkp(bins, objects, resultIds);

    // Print
    cout << finalValue << endl;

    for (auto &bin : resultIds) {
        cout << bin.size();

        for (auto id : bin) {
            cout << " " << id;
        }

        cout << endl;
    }

    return 0;
}
