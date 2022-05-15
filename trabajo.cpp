#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

struct resultado {
    int estadoFinal;
    string palabraStack;
};

struct transicion {
    int estadoInicial;
    char simbolo;
    char simboloStack;
    resultado res;
};

string unCaracter(string texto);

int main(int argc, char **argv){

    int estadoini, estadofinal;
    transicion t;
    vector<transicion> vTransicion;
    bool metodo; // Stack vacío = true, Estado final = false;
    string palabra, seguir, entrada;
    stack<char> pila;

    // TRANSICIONES
    do {
        cout << "-------------------------------------\nIngrese una transición T(q,a,z)=(p,ß)\n-------------------------------------" << endl;

        do {
            cout << "Ingrese el estado en el que se encuentra el APD (q) como un número entero >= 0: ";
            cin >> t.estadoInicial;
        } while(t.estadoInicial < 0);

        t.simbolo = unCaracter("Ingrese símbolo (a) que se lee: ")[0];

        t.simboloStack = unCaracter("Ingrese símbolo (z) en el tope de la pila: ")[0];

        do {
            cout << "Ingrese estado al que llega la transición (p) como un número entero >= 0: ";
            cin >> t.res.estadoFinal;
        } while(t.res.estadoFinal < 0);

        cout << "Ingrese conjunto de símbolos a poner en el stack (ß) como un string: ";
        cin >> t.res.palabraStack;

        cout << "\nSe agregará la siguiente transición: T(" << t.estadoInicial << ", " << t.simbolo << ", " << t.simboloStack << ")=(" << t.res.estadoFinal << ", " << t.res.palabraStack << ")" << endl;
        
        cout << "Presione enter para agregar la transición, cualquier otra letra para evitar su ingreso... ";
        cin.ignore();
        getline(cin, seguir);
        if(seguir == "") {
            vTransicion.push_back(t);
            cout << "Se ha agregado la transición exitosamente" << endl;
        } else cout << "Se ha descartado la transición" << endl;

        cout << "Presione enter para seguir agregando transiciones, cualquier otra letra para parar... ";
        getline(cin, seguir);
    } while(seguir == "");

    // ESTADO INICIAL
    do {
        cout << "Ingrese el estado inicial como un número entero >= 0: ";
        cin >> estadoini;
    } while(estadoini < 0);

    // STACK VACIO O ESTADO FINAL
    if(unCaracter("Ingrese si el APD acepta por estado final (f) o por stack vacío (v): ") == "v") metodo = true;
    else metodo = false;

    //ESTADO FINAL
    if(metodo == false) {
        do {
            cout << "Ingrese el estado final como un número entero >= 0: ";
            cin >> estadofinal;
        } while(estadofinal < 0);
    }

    //PALABRA
    cout << "Ingrese la palabra a evaluar: ";
    cin >> palabra;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    for(transicion a : vTransicion) {
        /* cout << "T(" << a.estadoInicial << ", " << a.simbolo << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", " << a.res.palabraStack << ")" << endl; */


    }

    return EXIT_SUCCESS;
}

string unCaracter(string texto) {
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.size() != 1);

    return entrada;
}