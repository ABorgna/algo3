#include <generator.h>
#include <interfaz.h>
#include <pcg.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

typedef struct Range {
    uint64_t begin;
    uint64_t end;
    uint64_t step;

    Range(const string& arg) : begin(1), end(1), step(1) {
        size_t first = arg.find(':');
        if (first == string::npos) {
            begin = end = stoul(arg);
        } else {
            begin = stoul(arg.substr(0, first));
            size_t second = arg.find(':', first + 1);
            if (second == string::npos) {
                end = stoul(arg.substr(first + 1));
                if (begin > end) {
                    step = end;
                    end = begin;
                    begin = step;
                }
            } else {
                end = stoul(arg.substr(first + 1, second - first - 1));
                step = stoul(arg.substr(second + 1));
            }
        }
    }

    friend ostream& operator<<(ostream& os, const Range& r) {
        return os << r.begin << ':' << r.end << ':' << r.step;
    }
} Range;

typedef struct Parameter {
    string option;
    vector<string> args;
} Parameter;

typedef struct {
    OptionCall call;
    vector<string> args;
    bool is_default;
} Action;

pcg64 eng;
ofstream file_output;
ifstream file_input;
uint64_t num_problema = 0;
uint64_t lim_nsecs = 0;
string nombre_gen;

void print_help();

Option::Option(char s, string b, int32_t p, bool a, OptionCall c, string ha,
               string hd)
    : param_count(p), is_action(a), call(c), help_args(ha), help_desc(hd) {
    small = "-";
    small.push_back(s);
    big = "--";
    big += b;
}

uint64_t diff(timespec start, timespec end) {
    uint64_t seconds = end.tv_sec - start.tv_sec;
    return seconds * 1000000000ULL + end.tv_nsec - start.tv_nsec;
}

uint64_t rnd(uint64_t inicio, uint64_t fin) {
    return pcg64_bounded(&eng, fin - inicio + 1) + inicio;
}

ostream& print_rng_state(ostream& os) {
    return os << (uint64_t)(eng.state >> 64) << " " << (uint64_t)eng.state;
}

bool load_next(istream& is) {
    char c;
    while (is.get(c)) {
        if (iswspace(c))
            continue;
        is.unget();
        prob_load(is);
        return true;
    }
    return false;
}

// Toma mediciones de varias iteraciones y devuelve el resultado de la última
uint64_t measure(vector<uint64_t>& mediciones) {
    uint64_t res = 0;
    timespec startT, endT;
    ostream null_stream(0);
    // Conservamos el estado inicial del engine.
    pcg64 eng_copia(eng);
    for (uint64_t i = 0; i < mediciones.size(); i++) {
        // Comienzo a medir tiempo
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT);
        // Le envío un stream null para ignorar el tiempo de escritura
        res = prob_solve(null_stream);
        // Termino de medir
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT);
        mediciones[i] = diff(startT, endT);
        // Algunos problemas pueden haber usado variables internas
        prob_reload();
        // Hasta la décima iteración estimo cuánto más debería seguir midiendo.
        if (lim_nsecs && i < 10) {
            auto it = mediciones.begin();
            uint64_t target = max(i + 1, lim_nsecs / *min_element(it, it + i));
            if (target > mediciones.size())
                mediciones.resize(target);
        }
        // Restauro estado inicial.
        eng = eng_copia;
    }
    return res;
}

void print_measure(ostream& os, uint64_t iteraciones,
                   const vector<uint64_t>& vars) {
    vector<uint64_t> times(iteraciones);
    uint64_t res = measure(times);
    for (const uint64_t v : vars) os << v << " ";
    os << *min_element(times.begin(), times.end()) << " " << times.size() << " "
       << res << endl;
}

int compare_streams(ostream& os, istream& resultados, istream& respuestas) {
    int64_t n, m;
    int64_t count = 0;
    while (resultados >> n) {
        if (!(respuestas >> m)) {
            os << "Incorrecto. Resultado: " << n << " sobrante" << endl;
            return EXIT_FAILURE;
        }
        if (n != m) {
            os << "Incorrecto. Resultado Nro " << count << ": " << n << " ≠ "
               << m << endl;
            return EXIT_FAILURE;
        }
        count++;
    }
    if (!resultados.eof()) {
        os << "Incorrecto. No se pudo leer un número entero" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_correct(ostream& os, istream& is, istream& respuestas) {
    stringstream resultado;
    for (uint64_t valor = 0; load_next(is); valor++) {
        resultado.clear();
        prob_solve(resultado);
        os << "Caso nro " << valor << endl;
        if (compare_streams(os, resultado, respuestas)) {
            cerr << "Discrepancia en el caso nro " << valor << endl;
            return EXIT_FAILURE;
        }
    }
    uint64_t dummy;
    if (respuestas >> dummy) {
        cerr << "El problema otorgó menos resultados de los esperados" << endl;
        return EXIT_FAILURE;
    }
    cerr << "OK!" << endl;
    return EXIT_SUCCESS;
}

bool increment_ranges(vector<uint64_t>& vars, const vector<Range>& ranges) {
    for (int i = vars.size() - 1; i >= 0; i--) {
        vars[i] += ranges[i].step;
        if (vars[i] > ranges[i].end && i != 0)
            vars[i] = ranges[i].begin;
        else
            return vars[i] <= ranges[i].end;
    }
    // Llega acá si incrementa la primer variable sin problemas.
    return true;
}

vector<uint64_t> get_vars_begin(const vector<Range>& ranges) {
    vector<uint64_t> vars;
    vars.reserve(ranges.size());
    for (const Range& range : ranges) vars.push_back(range.begin);
    return vars;
}

void log_generate(const uint64_t cantidad, const vector<Range>& ranges) {
    ofstream testlog("tests.log", ofstream::app);
    testlog << "Cantidad: " << cantidad << ", Estado: ";
    print_rng_state(testlog) << ", Rangos: ";
    for (const Range& range : ranges) testlog << range << " ";
    testlog << endl;
}

void measure_multiple(ostream& os, istream& is, uint64_t iteraciones) {
    if (num_problema > 0)
        cerr << "Avanzando hasta el checkpoint." << endl;
    for (uint64_t i = 0; i < num_problema && load_next(is); i++)
        ;
    for (uint64_t p = num_problema; load_next(is); p++) {
        cerr << "Problema Nᵒ " << p << endl;
        print_measure(os, iteraciones, prob_vars());
    }
    cerr << "Terminado." << endl;
}

void generate_measure(ostream& os, const vector<Range>& ranges,
                      uint64_t iteraciones, uint64_t cantidad,
                      GeneratorCall generator) {
    log_generate(cantidad, ranges);
    vector<uint64_t> vars = get_vars_begin(ranges);
    const uint64_t indice_range = num_problema / cantidad;
    uint64_t indice_instancia = num_problema % cantidad;
    if (num_problema > 0)
        cerr << "Avanzando hasta el checkpoint." << endl;
    // Incremento variables indice_range veces para llegar a num_problema.
    bool cont = true;
    for (uint64_t i = 0; i < indice_range && cont; i++)
        cont = increment_ranges(vars, ranges);
    while (cont) {
        for (uint64_t i = indice_instancia; i < cantidad; i++, num_problema++) {
            cerr << "Problema Nᵒ " << num_problema << ", RNG State: ";
            print_rng_state(cerr) << " Vars:";
            generator(vars);
            for (const uint64_t v : vars) cerr << " " << v;
            cerr << endl;

            // Si es all, sino generate
            if (iteraciones)
                print_measure(os, iteraciones, vars);
            else
                prob_print_input(os);
        }
        indice_instancia = 0;
        cont = increment_ranges(vars, ranges);
    }
    cerr << "Terminado." << endl;
}

istream& get_input() {
    if (file_input.is_open())
        return file_input;
    return cin;
}

ostream& get_output() {
    if (file_output.is_open())
        return file_output;
    return cout;
}

vector<Range> get_ranges(const vector<string>& args, uint64_t count) {
    vector<Range> ranges;
    ranges.reserve(count);
    for (size_t i = 0; i < count; i++) ranges.emplace_back(args[i]);
    return ranges;
}

int option_help(const vector<string>&) {
    print_help();
    return EXIT_SUCCESS;
}

int option_output(const vector<string>& args) {
    file_output.open(args[0], ofstream::app);
    file_output.setf(ios::fixed, ios::floatfield);
    return EXIT_SUCCESS;
}

int option_input(const vector<string>& args) {
    file_input.open(args[0]);
    return EXIT_SUCCESS;
}

int option_seed(const vector<string>& args) {
    pcg64_seed(&eng, U128(stoul(args[0]), stoul(args[1])));
    return EXIT_SUCCESS;
}

int option_checkpoint(const vector<string>& args) {
    num_problema = stoul(args[0]);
    return EXIT_SUCCESS;
}

int option_adjust(const vector<string>& args) {
    size_t argc = args.size();
    if (argc > 1) {
        cerr << "Se esperaba a lo sumo un parámetro adicional" << endl;
        return EXIT_FAILURE;
    }
    // 4 segundos de límite default
    lim_nsecs = 4000000000ULL;
    if (argc > 0)
        lim_nsecs = (uint64_t)(stod(args[0]) * 1000000000.0);
    return EXIT_SUCCESS;
}

int option_generador(const vector<string>& args) {
    nombre_gen = args[0];
    return EXIT_SUCCESS;
}

int option_medir(const vector<string>& args) {
    if (args.size() > 1) {
        cerr << "Cantidad de parámetros inválida" << endl;
        cerr << "Se esperaba a lo sumo un parámetro adicional" << endl;
        return EXIT_FAILURE;
    }
    uint64_t iteraciones = 1;
    if (args.size() == 1)
        iteraciones = stoul(args[0]);
    if (iteraciones == 0) {
        cerr << "Iteraciones es 0." << endl;
        return EXIT_FAILURE;
    }
    measure_multiple(get_output(), get_input(), iteraciones);
    return EXIT_SUCCESS;
}

int option_test(const vector<string>& args) {
    ifstream resultados(args[0]);
    int res = test_correct(get_output(), get_input(), resultados);
    resultados.close();
    return res;
}

int option_run(const vector<string>&) {
    istream& is = get_input();
    if (num_problema > 0)
        cerr << "Avanzando hasta el checkpoint." << endl;
    for (uint64_t i = 0; i < num_problema && load_next(is); i++)
        ;
    cerr << "Resolviendo..." << endl;
    while (load_next(is)) prob_solve(get_output());
    return EXIT_SUCCESS;
}

int option_once(const vector<string>&) {
    prob_load(get_input());
    prob_solve(get_output());
    return EXIT_SUCCESS;
}

void print_generators(const vector<Generator>& gens) {
    for (const Generator& g : gens) cerr << g.name << " ";
    cerr << endl;
}

int generar_all(const vector<string>& args, uint64_t all) {
    size_t count = prob_vars().size(), argc = args.size();
    if (argc < count || argc > count + all + 1) {
        cerr << "Cantidad de parámetros inválida\nSe esperaban " << count;
        cerr << " rangos, y no más de " << all + 1 << " adicionales" << endl;
        return EXIT_FAILURE;
    }
    uint64_t iteraciones = 1, cantidad = 1;
    if (argc > count)
        iteraciones = stoul(args[count]);
    if (argc > count + 1)
        cantidad = stoul(args[count + 1]);
    if (cantidad == 0 || iteraciones == 0) {
        cerr << "Cantidad o Iteraciones son 0." << endl;
        return EXIT_FAILURE;
    }
    vector<Generator> gens = prob_generators();
    if (gens.empty()) {
        cerr << "No hay un generador definido en el problema" << endl;
        return EXIT_FAILURE;
    }
    GeneratorCall gen_call = gens[0].call;
    if (nombre_gen != "") {
        auto is_generador = [&](const Generator& g) {
            return g.name == nombre_gen;
        };
        auto generador = find_if(gens.begin(), gens.end(), is_generador);
        if (generador == gens.end()) {
            cerr << "Generador inexistente, se esperaba alguno de:" << endl;
            print_generators(gens);
            return EXIT_FAILURE;
        }
        gen_call = generador->call;
    }
    if (!all) {
        cantidad = iteraciones;
        iteraciones = 0;
    }
    vector<Range> ranges = get_ranges(args, count);
    generate_measure(get_output(), ranges, iteraciones, cantidad, gen_call);
    return EXIT_SUCCESS;
}

int option_generar(const vector<string>& args) { return generar_all(args, 0); }

int option_all(const vector<string>& args) { return generar_all(args, 1); }

const vector<Option> GENERAL_OPTIONS = {
    {'o', "output", 1, false, &option_output, "<archivo>",
     "Archivo donde enviar el output, por default es stdout."},
    {'i', "input", 1, false, &option_input, "<archivo>",
     "Archivo donde leer el input, por default es stdin."},
    {'s', "seed", 2, false, &option_seed, "<upper> <lower>",
     "Asigna el estado del generador de números aleatorios.\n"
     "Referirse al paper de la familia de generadores PCG para más detalles.\n"
     "http://www.pcg-random.org/paper.html"},
    {'c', "checkpoint", -1, false, &option_checkpoint, "<número>",
     "Ajusta el engine para generar los problemas a partir de <número>.\n"
     "Notar que la numeración comienza desde cero."},
    {'A', "adjust", -1, false, &option_adjust, "[limite = 4.0]",
     "Predice y limita las iteraciones por medición a un tiempo máximo.\n"
     "[limite] es el tiempo en segundos que se usará para cada problema.\n"
     "Este puede ser cualquier número punto flotante de precisión doble."},
    {'G', "generador", 1, false, &option_generador, "<generador>",
     "Asigna el <generador> usado por -g y -a"},
    {'h', "help", -1, true, &option_help, "", "Imprime esta ayuda."},
    {'m', "medir", -1, true, &option_medir, "[iteraciones = 1]",
     "Mide [iteraciones] veces lo que tarda en resolver los problemas.\n"
     "Imprime las variables, la mínima medicion y el valor de retorno.\n"
     "Esto silencia la salida estándar del problema\n"
     "y la reemplaza por estadísticas crudas de las mediciones."},
    {'t', "test", 1, true, &option_test, "<archivo>",
     "Resuelve los problemas y compara los resultados con <archivo>.\n"
     "En la salida estándar se informa el resultado de la comparación\n"
     "de cada instancia individual. En las salida de error estándar\n"
     "se informa si hubo alguna discrepancia o no al terminar."},
    {'r', "run", 0, true, &option_run, "",
     "Resuelve todos los problemas de la entrada."},
    {'O', "once", 0, true, &option_once, "",
     "Resuelve un solo problema de la entrada, acción default."},
    {'g', "generar", -1, true, &option_generar, "<rangos> [cantidad = 1]",
     "Genera tests aleatorios para cada posible combinación de rangos.\n"
     "Los <rangos> son tuplas [inicio]:[fin]:[step] para cada variable.\n"
     "A:B se parsea como A:B:1 si A < B, si no, se parsea como B:A:B.\n"
     "B se parsea como B:B:1, lo cual es equivalente a fijar una variable.\n"
     "Se generan [cantidad] tests para cada combinación."},
    {'a', "all", -1, true, &option_all,
     "<rangos> [iteraciones = 1] [cantidad = 1]",
     "Combina -g y -m, tomando la salida de -g como entrada de -m."},
};

void print_help_options(const vector<Option>& options) {
    for (const Option& o : options) {
        cerr << o.small << " | " << o.big << " " << o.help_args << endl;
        cerr << o.help_desc << endl << endl;
    }
}

void print_help() {
    cerr << "Parámetros posibles de la interfaz de problemas:\n\n";
    print_help_options(GENERAL_OPTIONS);
    vector<Option> custom_options = prob_custom_options();
    if (custom_options.size() > 0) {
        cerr << "Parámetros custom del problema:\n\n";
        print_help_options(custom_options);
    }
    vector<Generator> gens = prob_generators();
    if (gens.size() > 0) {
        cerr << "Generadores disponibles del problema:\n";
        print_generators(gens);
    }
}

int parse_options(vector<Parameter>& params, const vector<Option>& options,
                  Action& action) {
    for (const Option& o : options) {
        auto is_option = [&](const Parameter& p) {
            return (p.option == o.big) || (p.option == o.small);
        };
        auto p = find_if(params.begin(), params.end(), is_option);
        if (p == params.end())
            continue;
        if (o.param_count == -1 || (size_t)o.param_count == p->args.size()) {
            if (o.is_action) {
                if (!action.is_default) {
                    cerr << "No se puede pedir más de una acción" << endl;
                    return EXIT_FAILURE;
                }
                action.call = o.call;
                action.args = p->args;
                action.is_default = false;
            } else {
                if (o.call(p->args))
                    return EXIT_FAILURE;
            }
        } else {
            cerr << "Cantidad de parámetros inválida para " << p->option
                 << ", se esperan " << o.param_count << " parámetros\n"
                 << endl;
            return EXIT_FAILURE;
        }
        params.erase(p);
    }
    return EXIT_SUCCESS;
}

int parse_params(vector<Parameter>& params) {
    // Esta es la acción default, resolver los problemas
    Action action = {&option_once, {}, true};
    if (parse_options(params, prob_custom_options(), action))
        return EXIT_FAILURE;
    if (parse_options(params, GENERAL_OPTIONS, action))
        return EXIT_FAILURE;
    if (params.empty())
        return action.call(action.args);
    for (const Parameter& p : params)
        cerr << "Opción inválida o repetida " << p.option << endl;
    return EXIT_FAILURE;
}

int main(int argc, char* argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    pcg64_seed(&eng, U128(U64(random_device()(), random_device()()),
                          U64(random_device()(), random_device()())));
    vector<Parameter> pm;
    pm.reserve(argc);
    for (int i = 1; i < argc;) {
        Parameter p;
        p.option = argv[i++];
        while (i < argc && argv[i][0] != '-') p.args.emplace_back(argv[i++]);
        pm.push_back(p);
    }
    int ret = parse_params(pm);
    if (file_output.is_open())
        file_output.close();
    if (file_input.is_open())
        file_input.close();
    return ret;
}
