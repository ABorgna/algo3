#include <interfaz.h>

#include <math.h>
#include <stdint.h>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

uint64_t peso;

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

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream& is) {
    // Leer el peso de la llave
    is >> peso;
}

int prob_solve(std::ostream &os) {
    // Exponentes de las pesas de cada lado
    // las de la izquierda se suman al valor de la llave
    vector<uint8_t> izq;
    vector<uint8_t> der;

    balancear(peso, izq, der);

    // Imprimimos el resultado
    os << izq.size() << " " << der.size() << endl;

    bool first = true;
    for (uint32_t i = 0; i < izq.size(); i++) {
        if (!first)
            os << " ";
        os << (uint64_t) pow(3, izq[i]);
        first = false;
    }
    os << endl;

    first = true;
    for (uint32_t i = 0; i < der.size(); i++) {
        if (!first)
            os << " ";
        os << (uint64_t)pow(3, der[i]);
        first = false;
    }
    os << endl;

    return 0;
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    return {peso};
}

void prob_print_input(std::ostream& os) {
    os << peso << endl;
}

vector<Option> prob_custom_options() {
    return {};
}

void generator_simple(const std::vector<uint64_t>& v) {
    prob_reload();

    peso = v[0];
}

vector<Generator> prob_generators() {
    return {{"simple",generator_simple}};
}

