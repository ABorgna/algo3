
#include <stdint.h>
#include <iostream>
#include <map>
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
    for (auto &obj : objetos) {
        totObjetos += obj.count;
    }

    // m tiene como claves los pesos de las mochilas
    // y como valor la máxima ganancia y su combinación de objetos necesaria en
    // cada mochila
    // Una ganancia -1 significa que no hay una configuración posible
    typedef struct valor_combinacion_t {
        vector<vector<bool>> combinaciones;
        int32_t valor;
    } valor_combinacion;

    map<vector<uint8_t>, vector<valor_combinacion>> m;

    // Recorremos cada combinacion
    vector<uint8_t> combinacion(bins.size(), 0);
    bool done = false;
    valor_combinacion *combinacion_max = NULL;

    while (!done) {
        // Agregamos la combinación a la lista
        // Inicializamos todas las ganancias en -1
        // Excepto para 0 objetos (ganancia 0)
        valor_combinacion vc{
            vector<vector<bool>>(bins.size(), vector<bool>(totObjetos)), -1};
        vector<valor_combinacion> vcs_tmp(totObjetos + 1, vc);
        vcs_tmp[0].valor = 0;

        auto out = m.insert({combinacion, vcs_tmp});
        vector<valor_combinacion> &vcs = out.first->second;

        // Para cada primeros j objetos buscamos el máximo
        uint32_t j = 0;
        for (auto obj : objetos) {
            for (uint32_t cnt = 0; cnt < obj.count; cnt++) {
                j++;

                // TODO: es todo horrible pero funciona, refactor

                // Decidimos a qué bin conviene agregar el objeto,
                // o si conviene no usarlo
                int32_t mejor_bin = -1;
                valor_combinacion &mejor_vc = vcs[j - 1];

                for (uint8_t bin = 0; bin < bins.size(); bin++) {
                    // No podemos agregar el objeto al bin si no entra
                    if (combinacion[bin] < obj.weight) continue;

                    // Veamos si combiene usar este bin
                    vector<uint8_t> comb(combinacion);
                    comb[bin] -= obj.weight;
                    valor_combinacion &vc_a_agregar = m[comb][j-1];
                    // TODO: horrible
                    if(vc_a_agregar.valor >= 0 && mejor_vc.valor <= vc_a_agregar.valor + (mejor_bin >= 0 ? obj.value : 0)) {
                        mejor_bin = bin;
                        mejor_vc = vc_a_agregar;
                    }
                }

                vcs[j] = mejor_vc;

                if (mejor_bin != - 1) {
                    // Usamos el objeto, hay que marcarlo en el vc que copiamos
                    vcs[j] = mejor_vc;
                    vcs[j].valor += obj.value;
                    vcs[j].combinaciones[mejor_bin][j] = 1;
                }

                // Recalcular el máximo
                if (combinacion_max == NULL ||
                    combinacion_max->valor < vcs[j].valor) {
                    combinacion_max = &vcs[j];
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
    // TODO: feo
    resultIds.resize(bins.size());
    uint32_t j = 0;
    for (uint32_t i=0; i<objetos.size(); i++) {
        const mkp_object& obj = objetos[i];
        for (uint32_t cnt = 0; cnt < obj.count; cnt++) {
            j++;
            for(uint32_t bin=0; bin<bins.size(); bin++) {
                if(combinacion_max->combinaciones[bin][j]) {
                    resultIds[bin].push_back(i);
                }
            }
        }
    }

    return combinacion_max->valor;
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
