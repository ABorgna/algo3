
#include <math.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

vector<uint8_t> digitosBase3(uint64_t n);

int main() {
    uint64_t peso;

    // Leer el peso de la llave
    cin >> peso;

    // Descomponemos el peso en sus digitos base 3
    vector<uint8_t> digitos = digitosBase3(peso);

    // Exponentes de las pesas de cada lado
    // las de la izquierda se suman al valor de la llave
    set<uint8_t> izq;
    set<uint8_t> der;

    // A cada dígito '2' del peso hay que sumarle 1
    bool carry = false;
    for(uint8_t i=0; i < digitos.size(); i++) {
        if(carry) {
            digitos[i] += 1;
            digitos[i] %= 3;

            // Checkear si se generó otro carry mas
            carry = digitos[i] == 0;
        }

        if(digitos[i] == 2) {
            // Agregamos una pesa con este exponente
            izq.insert(i);

            // Actualizamos el peso
            digitos[i] = 0;
            carry = true;
        }
    }
    if(carry) digitos.push_back(1);

    // Ahora por cada dígito '1' agregamos una pesa del otro lado,
    // para contrarestarlo
    for(uint8_t i=0; i < digitos.size(); i++) {
        if(digitos[i] == 1) {
            der.insert(i);
        }
    }

    // Imprimimos el resultado
    cout << izq.size() << " " << der.size() << endl;

    bool first = true;
    for(auto e : izq) {
        if(!first) cout << " ";
        cout << pow(3,e);
        first = false;
    }
    cout << endl;

    first = true;
    for(auto e : der) {
        if(!first) cout << " ";
        cout << pow(3,e);
        first = false;
    }
    cout << endl;

    return 0;
}

/*
 * Devuelve los dígitos de n en base 3, least significant first
 * El vector retornado no excede los 32 elementos
 */
vector<uint8_t> digitosBase3(uint64_t n) {
    vector<uint8_t> res;

    // log(n) nos da el tamaño del array resultante * ~1.1
    // No hace falta algo exacto
    res.reserve(log(n)+1);

    while (n > 0) {
        res.push_back(n % 3);
        n /= 3;
    }

    return res;
}
