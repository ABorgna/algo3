
#include <math.h>
#include <stdint.h>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

void balancear(uint64_t peso, set<uint8_t> &izq, set<uint8_t> &der) {
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
            der.insert(exponente);

            // Actualizamos el peso
            digito = 0;
            carry = true;

        } else if (digito == 1) {
            // Si quedó un 1 agregamos una pesa del otro lado, para
            // contrarestarlo
            izq.insert(exponente);
        }

        exponente++;
    }

    if (carry)
        izq.insert(exponente);
}

int main() {
    uint64_t peso;

    // Leer el peso de la llave
    cin >> peso;

    // Exponentes de las pesas de cada lado
    // las de la izquierda se suman al valor de la llave
    set<uint8_t> izq;
    set<uint8_t> der;

    balancear(peso, izq, der);

    // Imprimimos el resultado
    cout << izq.size() << " " << der.size() << endl;

    bool first = true;
    for (auto e : izq) {
        if (!first)
            cout << " ";
        cout << pow(3, e);
        first = false;
    }
    cout << endl;

    first = true;
    for (auto e : der) {
        if (!first)
            cout << " ";
        cout << pow(3, e);
        first = false;
    }
    cout << endl;

    return 0;
}
