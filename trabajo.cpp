#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Resultado {
    int estadoFinal;
    string palabraStack;
};

struct Transicion {
    int estadoInicial;
    char simbolo;
    char simboloStack;
    Resultado res;
};

struct DI{
    int estadoActual;
    string palabraPorLeer;
    stack<char> contenidoStack;
};

char unCaracter(string texto);
char unCaracterMayus(string texto);
int esNumero(string texto);
void evaluarPalabra(vector<Transicion> &vTransicion, string palabra, int estadoini, int estadofinal, bool metodo);
bool buscarTransicion(vector<Transicion> &v, DI *di, int estadoActual, char simboloLeer, char simboloStack);
void imprimirPila(stack<char> pila);
void imprimirDI(DI di);

int main(int argc, char **argv){

    int estadoini, estadofinal = -1;
    Transicion t;
    vector<Transicion> vTransicion;
    bool minus, metodo; // Stack vacío = true, Estado final = false;
    string palabra, seguir, entrada;
    char c;

    // TRANSICIONES
    do {
        cout << "-------------------------------------\nIngrese una transición T(q,a,z)=(p,ß)\n-------------------------------------" << endl;

        t.estadoInicial = esNumero("Ingrese el estado en el que se encuentra el APD (q) como un número entero >= 0: ");

        cin.ignore();
        do {
            cout << "Ingrese símbolo (a) que se lee como alguna letra minúscula, número o símbolo (enter para leer símbolo vacío): ";
            getline(cin, entrada);

        } while(entrada != "" && (entrada.length() != 1 || (isupper(entrada[0]) && !isdigit(entrada[0]))) );
        if(entrada == "") t.simbolo = 'E';
        else t.simbolo = entrada[0];

        t.simboloStack = unCaracterMayus("Ingrese símbolo (z) en el tope de la pila en mayúsculas: "); // Puede ser solo un char upper

        t.res.estadoFinal = esNumero("Ingrese estado al que llega la transición (p) como un número entero >= 0: ");

        cin.ignore();
        do {
            cout << "Ingrese conjunto de símbolos para reemplazar en la pila del stack (ß) como un string en mayúsculas (enter si quiere sacar elementos del stack): ";
            getline(cin,t.res.palabraStack); // Puede estar en mayúsculas o vacía
            
            minus = false;

            for(char c : t.res.palabraStack) {
                if( int(c)<65 || int(c)>90 ) minus = true;
            }
        } while(minus);

        //AGREGAR TRANSICIÓN
        cout << "\nSe agregará la siguiente transición: T(" << t.estadoInicial << ", ";
        if(t.simbolo == 'E') cout << "ε";
        else cout << t.simbolo;
        cout << ", " << t.simboloStack << ")=(" << t.res.estadoFinal << ", ";
        if(t.res.palabraStack == "") cout << "ε" << ")" << endl;
        else cout << t.res.palabraStack << ")" << endl;

        cout << "Presione enter para agregar la transición, cualquier otra letra para evitar su ingreso... ";
        getline(cin, seguir);
        if(seguir == "") {
            vTransicion.push_back(t);
            cout << "Se ha agregado la transición exitosamente" << endl;
        } else cout << "Se ha descartado la transición" << endl;

        //SEGUIR AGREGANDO TRANSICIONES
        cout << "Presione enter para seguir agregando transiciones, cualquier otra letra para parar... ";
        getline(cin, seguir);
    } while(seguir == "");

    // ESTADO INICIAL
    estadoini = esNumero("Ingrese el estado inicial como un número entero >= 0: ");

    // STACK VACIO O ESTADO FINAL
    do {
        c = unCaracter("Ingrese si el APD acepta por estado final 'f' o por stack vacío 'v': ");
    } while(c != 'f' && c != 'v');
    if(c == 'v') metodo = true;
    else metodo = false;

    //ESTADO FINAL
    if(metodo == false) { // Si acepta por estado final
        estadofinal = esNumero("Ingrese el estado final como un número entero >= 0: ");
    }

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    cout << "-------------------------------------\nTrasiciones\n-------------------------------------" << endl;
    for(Transicion a : vTransicion) {
        cout << "T(" << a.estadoInicial << ", ";
        if(a.simbolo == 'E') cout << "ε";
        else cout << a.simbolo;
        cout << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", ";
        if(a.res.palabraStack == "") cout << "ε" << ")" << endl;
        else cout << a.res.palabraStack << ")" << endl;
    }
    cout << "\nEstado inicial: " << estadoini << endl;
    if(metodo) cout << "APD acepta por stack vacío" << endl;
    else {
        cout << "APD acepta por estado final" << endl;
        cout << "Estado final: " << estadofinal << endl;
    }

    //EVALUAR LA PALABRA
    cin.ignore();
    do {
        //PALABRA
        cout << "Ingrese la palabra a evaluar: ";
        getline(cin, palabra);

        evaluarPalabra(vTransicion,palabra,estadoini,estadofinal,metodo);

        cout << "Para probar otra palabra pulse enter, cualquier otra letra para terminar... ";
        getline(cin, seguir);
    } while(seguir == "");

    return EXIT_SUCCESS;
}

void evaluarPalabra(vector<Transicion> &vTransicion, string palabra, int estadoInicial,int estadoFinal, bool metodo){
    DI descinst;
    int j, estadoActual;
    char simboloLeer, simboloStack;

    descinst.estadoActual = estadoInicial;
    descinst.palabraPorLeer = palabra;
    descinst.contenidoStack.push('R'); // Símbolo en la base del stack R

    imprimirDI(descinst);

    //Lee la palabra
    for(int i = 0; i<palabra.size(); i++){

        estadoActual = descinst.estadoActual;
        simboloLeer = palabra[i];
        simboloStack = descinst.contenidoStack.top();

        if(buscarTransicion(vTransicion, &descinst, estadoActual, simboloLeer, simboloStack)) {

            descinst.palabraPorLeer = palabra.substr(i+1,palabra.length()-i-1);

            imprimirDI(descinst);
        } else return;

    }

    //Luego de leer la palabra

    simboloLeer = 'E';

    if(metodo == false) { //ACEPTAR POR ESTADO FINAL
        while(descinst.estadoActual != estadoFinal) {

            estadoActual = descinst.estadoActual;
            simboloStack = descinst.contenidoStack.top();

            if(buscarTransicion(vTransicion, &descinst, estadoActual, simboloLeer, simboloStack)) imprimirDI(descinst);
            else return;

        }
        cout << "Se aceptó la palabra " << palabra << " por estado final" << endl;

    } else { //ACEPTAR POR STACK VACÍO
        while(!descinst.contenidoStack.empty()){

            estadoActual = descinst.estadoActual;
            simboloStack = descinst.contenidoStack.top();

            if(buscarTransicion(vTransicion, &descinst, estadoActual, simboloLeer, simboloStack)) imprimirDI(descinst);
            else return;
        }
        cout << "Se aceptó la palabra " << palabra << " por stack vacío" << endl;

    }

}

char unCaracter(string texto) { // Validacion para símbolo que lee
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.length() != 1 || (isupper(entrada[0]) && !isdigit(entrada[0])) ); 

    return entrada[0];
}

int esNumero(string texto) {
    string entrada;
    bool seguir;

    do {
        seguir = false;
        cout << texto;
        cin >> entrada;
        for(char c : entrada){
            if(!isdigit(c)) seguir = true;
        }
    } while(seguir || stoi(entrada) < 0);

    return stoi(entrada);
}

char unCaracterMayus(string texto) {
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.length() != 1 || int(entrada[0])<65 || int(entrada[0])>90);

    return entrada[0];
}

bool buscarTransicion(vector<Transicion> &v, DI *di, int estadoActual, char simboloLeer, char simboloStack) {
    int j = 0;

    //Busca la transición correspondiente al DI actual
    while (j < v.size() && ( estadoActual != v[j].estadoInicial || simboloLeer != v[j].simbolo || simboloStack != v[j].simboloStack )){
        j++;
    }
    if(j >= v.size()) {  //Si no hay transición para el estado actual
        cout << "La palabra ingresada no es aceptada por el lenguaje" << endl;
        return false;
    }

    // Despues del while se encuentra la transicion en la posicion j
    (*di).estadoActual = v[j].res.estadoFinal;
    
    (*di).contenidoStack.pop();
    if(v[j].res.palabraStack != "") { //Si hay que sacar el símbolo del stack no hace nada
        for (int k=v[j].res.palabraStack.length()-1; k>=0; k--){
            (*di).contenidoStack.push(v[j].res.palabraStack[k]); //Agrega los símbolos al stack
        }
    }

    return true;
}

void imprimirPila(stack<char> pila) {
    while(!pila.empty()) {
        cout << pila.top();
        pila.pop();
    }
}

void imprimirDI(DI di) {
    cout << "DI: (" << di.estadoActual << ",";
    if(di.palabraPorLeer == "") cout << "ε" << ",";
    else cout << di.palabraPorLeer << ",";
    if(di.contenidoStack.empty()) cout << "ε";
    else imprimirPila(di.contenidoStack);
    cout << ")" << endl;
}