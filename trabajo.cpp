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

struct DI{
    int estadoActual;
    string palabraPorLeer;
    string contenidoStack;
};

string unCaracter(string texto);
string unCaracterMayus(string texto);
void evaluarPalabra(vector<transicion> vTransicion, string palabra,int estadoini, int estadofinal,bool metodo);

int main(int argc, char **argv){

    int estadoini, estadofinal;
    transicion t;
    vector<transicion> vTransicion;
    bool metodo; // Stack vacío = true, Estado final = false;
    string palabra, seguir, entrada;

    // TRANSICIONES
    do {
        cout << "-------------------------------------\nIngrese una transición T(q,a,z)=(p,ß)\n-------------------------------------" << endl;

        do {
            cout << "Ingrese el estado en el que se encuentra el APD (q) como un número entero >= 0: ";
            cin >> t.estadoInicial;
        } while(t.estadoInicial < 0); //int mayor a 0 falta solucionar el caso cuando ingresa una letra

        t.simbolo = unCaracter("Ingrese símbolo (a) que se lee: ")[0]; //puede ser lower o int

        t.simboloStack = unCaracterMayus("Ingrese símbolo (z) en el tope de la pila: ")[0]; //puede ser solo un char upper

        do {
            cout << "Ingrese estado al que llega la transición (p) como un número entero >= 0: ";
            cin >> t.res.estadoFinal;
        } while(t.res.estadoFinal < 0); //int mayor a 0 falta solucionar el caso cuando ingresa una letra

        cout << "Ingrese conjunto de símbolos para reemplazar en la pila del stack (ß) como un string (mayusculas o e si quiere sacar elementos del stack): ";
        cin >> t.res.palabraStack; //no necesita mayores validaciones porque pueden ser solo mayusculas o e

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
        cout << "T(" << a.estadoInicial << ", " << a.simbolo << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", " << a.res.palabraStack << ")" << endl;


    }
    evaluarPalabra(vTransicion,palabra,estadoini,estadofinal,metodo);
    return EXIT_SUCCESS;
}

void evaluarPalabra(vector<transicion> vTransicion, string palabra,int estadoInicial,int estadoFinal, bool metodo){
    stack<char> pila;
    pila.push('R');
    int i = 0;
    DI descinst;
    descinst.estadoActual = estadoInicial;
    descinst.palabraPorLeer = palabra;
    descinst.contenidoStack = pila.top();
    cout << "DI antes de iniciar: (" << descinst.estadoActual << "," << descinst.palabraPorLeer << "," << descinst.contenidoStack << ")" << endl;
    if (metodo == false){
        while (i<palabra.size()){
            int j = 0;
            int estadoActual = descinst.estadoActual;
            char simboloLeer = palabra[i];
            char simboloStack = pila.top();
            while (estadoActual != vTransicion[j].estadoInicial && simboloLeer != vTransicion[j].simbolo && simboloStack != vTransicion[j].simboloStack){
                j++;
            }
            //despues del while se encuentra la transicion en la posicion j
            estadoActual = vTransicion[j].res.estadoFinal;
            pila.pop();
            descinst.estadoActual = estadoActual;
            i++;
            descinst.palabraPorLeer = palabra.substr(i,palabra.size()-i);
            for (int k=vTransicion[j].res.palabraStack.size()-1; i>=0; i--){
                pila.push(vTransicion[j].res.palabraStack[k]);
            }
            stack<char> aux = pila;
            while (!aux.empty()){
                descinst.contenidoStack += aux.top();
                aux.pop();
            }
            cout << "DI despues de la transicion: (" << descinst.estadoActual << "," << descinst.palabraPorLeer << "," << descinst.contenidoStack << ")" << endl;
        }
        
    }
}

string unCaracter(string texto) { //validacion para símbolo que lee
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.size() != 1 || (!islower(entrada[0]) && !isdigit(entrada[0]))); 

    return entrada;
}

string unCaracterMayus(string texto) {
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.size() != 1 || !isupper(entrada[0]));

    return entrada;
}