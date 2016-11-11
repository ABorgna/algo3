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

enum tipo_grasp_t {
    tipo_grasp_alternado  = 0,
    tipo_grasp_trim = 1,
    tipo_grasp_2opt = 2,
    tipo_grasp_swap = 3,
};

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

tipo_grasp_t tipo_grasp = tipo_grasp_alternado;
string tipo_grasp_name = "grasp_alternado";

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

    switch (tipo_grasp) {
        case tipo_grasp_alternado:
            tie(d, k) = grasp_alternado(orden, expLimite, expInicios);
            break;
        case tipo_grasp_trim:
            tie(d, k) = grasp_trim(orden, expLimite, expInicios);
            break;
        case tipo_grasp_swap:
            tie(d, k) = grasp_swap(orden, expInicios);
            break;
        case tipo_grasp_2opt:
            tie(d, k) = grasp_2opt(orden, expInicios);
            break;
    }

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10 + tipo_grasp;
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

int setTipo(const vector<string>& s) {
    tipo_grasp_name = s[0];
    if (s[0] == "grasp_alternado") {
        tipo_grasp = tipo_grasp_alternado;
    } else if (s[0] == "grasp_trim") {
        tipo_grasp = tipo_grasp_trim;
    } else if (s[0] == "grasp_swap") {
        tipo_grasp = tipo_grasp_swap;
    } else if (s[0] == "grasp_2opt") {
        tipo_grasp = tipo_grasp_2opt;
    } else {
        cerr << s[0] << " no es un tipo válido" << endl;
        return 1;
    }
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'L', "limite", 1, false, &setExpLimite, "<exponente>",
             "Setea un exponente para calcular la cantidad máxima de iteraciones \n"
             "de cada búsqueda local. Cada una correrá a lo sumo nodos^<exp> pasos."},
            {'I', "inicios", 1, false, &setExpInicios, "<exponente>",
             "Setea un exponente para calcular el número de puntos de inicio \n"
             "aleatorios del grasp. La cantidad será igual a nodos^<exp>."},
            {'T', "tipo", 1, false, &setTipo, "<tipo>",
            "Seleccionar el tipo de grasp a realizar \n"
            "Opciones: grasp_alternado, grasp_swap, grasp_2opt, grasp_trim.Default=grasp_alternado"}};
}
