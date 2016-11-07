#include <algoritmos.h>
#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

#include <math.h>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 4;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

double lastResult = 0;

double expLimite = 1;
double expInicios = 1;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    vector<int64_t> orden;

    tie(d, k) = grasp(orden, expLimite, expInicios);

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

void prob_extra_info(std::ostream& os) {
    os << lastResult << ",grasp,"
       << expLimite << " " << expInicios;
}

int setExpLimite(const vector<string>& s) {
    size_t sz = 0;
    double num = stod(s[0], &sz);
    if (sz == 0 or num < 0) {
        cerr << "El exponente del límite debe ser un número positivo" << endl;
        return 1;
    }
    expLimite = num;
    return 0;
}

int setExpInicios(const vector<string>& s) {
    size_t sz = 0;
    double num = stod(s[0], &sz);
    if (sz == 0 or num < 0) {
        cerr << "El exponente de la cantidad de inicios debe ser un número positivo" << endl;
        return 1;
    }
    expInicios = num;
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'L', "limite", 1, false, &setExpLimite, "<exponente>",
             "Setea un exponente para calcular la cantidad máxima de iteraciones \n"
             "de cada búsqueda local. Cada una correrá a lo sumo nodos^<exp> pasos."},
            {'I', "inicios", 1, false, &setExpInicios, "<exponente>",
             "Setea un exponente para calcular el número de puntos de inicio \n"
             "aleatorios del grasp. La cantidad será igual a nodos^<exp>."}};
}
