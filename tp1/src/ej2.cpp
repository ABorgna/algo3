
#include <math.h>
#include <stdint.h>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

void balancear(uint64_t peso, vector<uint8_t> &izq, vector<uint8_t> &der) {
    // peso = sum(3^izq) - sum(3^der)

    bool carry = false;
    uint8_t exponente = 0;

    // Recorremos los dígitos en base 3, empezando por el menos significativo
    while (peso > 0) {
        uint8_t digito = peso % 3;
        peso /= 3;

        // Le sumamos una pesa al dígito anterior, y quedó un carry
        if (carry) {
            digito += 1;
            digito %= 3;

            // Checkear si se generó otro carry mas
            carry = digito == 0;
        }

        if (digito == 2) {
            // A cada dígito '2' del peso hay que sumarle 1

            // Agregamos una pesa con este exponente
            der.push_back(exponente);

            // Actualizamos el peso
            digito = 0;
            carry = true;

        } else if (digito == 1) {
            // Si quedó un 1 agregamos una pesa del otro lado, para
            // contrarestarlo
            izq.push_back(exponente);
        }

        exponente++;
    }

    if (carry)
        izq.push_back(exponente);
}

int main() {
    uint64_t peso;

    // Leer el peso de la llave
    cin >> peso;

    // Exponentes de las pesas de cada lado
    // las de la izquierda se suman al valor de la llave
    vector<uint8_t> izq;
    vector<uint8_t> der;

    balancear(peso, izq, der);

    // Imprimimos el resultado
    cout << izq.size() << " " << der.size() << endl;

    bool first = true;
    for (uint32_t i = 0; i < izq.size(); i++) {
        if (!first)
            cout << " ";
        cout << (uint64_t) pow(3, izq[i]);
        first = false;
    }
    cout << endl;

    first = true;
    for (uint32_t i = 0; i < der.size(); i++) {
        if (!first)
            cout << " ";
        cout << (uint64_t)pow(3, der[i]);
        first = false;
    }
    cout << endl;

    return 0;
}
