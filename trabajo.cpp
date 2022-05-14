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

int main(int argc, char **argv){

    int estadoini, estadofinal;
    transicion t;
    vector<transicion> vTransicion;
    bool metodo; // Stack vacío = true, Estado final = false;
    string palabra, seguir;
    stack<char> pila;

    do {
        cout << "Ingrese el estado inicial como un número entero >= 0: ";
        cin >> estadoini;
    } while(estadoini < 0);

    do {
        cout << "-------------------------------------\nIngrese una transición T(q,a,z)=(p,ß)" << endl;

        do {
            cout << "Ingrese el estado en el que se encuentra el APD (q) como un número entero >= 0: ";
            cin >> t.estadoInicial;
        } while(t.estadoInicial < 0);

        cout << "Ingrese símbolo (a) que se lee: ";
        cin >> t.simbolo;

        cout << "Ingrese símbolo (z) en el tope de la pila: ";
        cin >> t.simboloStack;

        do {
            cout << "Ingrese estado al que llega la transición (p) como un número entero >= 0: ";
            cin >> t.res.estadoFinal;
        } while(t.res.estadoFinal < 0);

        cout << "Ingrese conjunto de símbolos a poner en el stack (ß) como un string donde el símbolo de más a la izquierda queda en el tope del stack: ";
        cin >> t.res.palabraStack;

        cout << "\nSe agregará la siguiente transición: T(" << t.estadoInicial << ", " << t.simbolo << ", " << t.simboloStack << ")=(" << t.res.estadoFinal << ", " << t.res.palabraStack << ")" << endl;
        
        cin.ignore();
        cout << "Presione enter para agregar la transición, cualquier otra letra para evitar su ingreso... ";
        getline(cin, seguir);
        if(seguir == "") {
            vTransicion.push_back(t);
            cout << "Se ha agregado la transición exitosamente" << endl;
        }

        cout << "Presione enter para seguir agregando transiciones, cualquier otra letra para parar... ";
        getline(cin, seguir);
    } while(seguir == "");

    cout << "Ingrese si el APD acepta por estado final (f) o por stack vacío (v): ";
    cin >> seguir;
    if(seguir == "v") metodo = true;
    else metodo = false;

    cout << "Ingrese la palabra a evaluar: ";
    cin >> palabra;

    for(transicion a : vTransicion) {
        /* cout << "T(" << a.estadoInicial << ", " << a.simbolo << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", " << a.res.palabraStack << ")" << endl; */


    }

    return EXIT_SUCCESS;
}