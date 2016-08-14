
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int64_t tiempoMinimo(vector<uint32_t> &arq, vector<uint32_t> &can) {
    // Necesitamos que los arqueólogos sean al menos la mitad
    if (arq.size() < can.size()) {
        return -1;
    }

    uint64_t minimo = -1;
    uint64_t total = 0;

    for (uint64_t e : arq) {
        minimo = min(e, minimo);
        total += e;
    }
    for (uint64_t e : can) {
        minimo = min(e, minimo);
        total += e;
    }

    int64_t cantidad = arq.size() + can.size();
    return (total - minimo) + (cantidad - 2) * minimo;
}

int main() {
    uint32_t cantArq;
    uint32_t cantCan;

    vector<uint32_t> arq;
    vector<uint32_t> can;

    cin >> cantArq >> cantCan;

    arq.reserve(cantArq);
    can.reserve(cantCan);

    for (uint32_t i = 0; i < cantArq; i++) {
        uint32_t a;
        cin >> a;
        arq.push_back(a);
    }

    for (uint32_t i = 0; i < cantCan; i++) {
        uint32_t a;
        cin >> a;
        can.push_back(a);
    }

    int64_t t = tiempoMinimo(arq, can);

    cout << t << endl;

    return 0;
}
