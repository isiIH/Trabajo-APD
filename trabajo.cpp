#include <iostream>
#include <stack>
#include <string>
#include <vector>
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

string unCaracter(string texto);
string unCaracterMayus(string texto);
void evaluarPalabra(vector<Transicion> &vTransicion, string palabra, int estadoini, int estadofinal, bool metodo);
void imprimirPila(stack<char> pila);

int main(int argc, char **argv){

    int estadoini, estadofinal = -1;
    Transicion t;
    vector<Transicion> vTransicion;
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

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */



    /* for(Transicion a : vTransicion) {
        cout << "T(" << a.estadoInicial << ", " << a.simbolo << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", " << a.res.palabraStack << ")" << endl;
    } */

    do {
        //PALABRA
        cout << "Ingrese la palabra a evaluar: ";
        cin >> palabra;

        evaluarPalabra(vTransicion,palabra,estadoini,estadofinal,metodo);

        cout << "Para probar otra palabra pulse enter, cualquier otra letra para terminar... ";
        cin.ignore();
        getline(cin, seguir);
    } while(seguir == "");

    return EXIT_SUCCESS;
}

void evaluarPalabra(vector<Transicion> &vTransicion, string palabra, int estadoInicial,int estadoFinal, bool metodo){
    int i = 0;
    DI descinst;

    descinst.estadoActual = estadoInicial;
    descinst.palabraPorLeer = palabra;
    descinst.contenidoStack.push(vTransicion[0].simboloStack); // R u otro símbolo en la base del stack

    cout << "DI: (" << descinst.estadoActual << "," << descinst.palabraPorLeer << ",";
    imprimirPila(descinst.contenidoStack);
    cout << ")" << endl;

    int j, estadoActual;
    char simboloLeer, simboloStack;
    while (i<palabra.length()){
        j = 0;
        estadoActual = descinst.estadoActual;
        simboloLeer = palabra[i];
        simboloStack = descinst.contenidoStack.top();

        while (j < vTransicion.size() && ( estadoActual != vTransicion[j].estadoInicial || simboloLeer != vTransicion[j].simbolo || simboloStack != vTransicion[j].simboloStack )){
            j++;
        }
        if(j >= vTransicion.size()) {  //Si no hay transición para el estado actual
            cout << "La palabra ingresada no es aceptada por el lenguaje" << endl;
            return;
        }
        //despues del while se encuentra la transicion en la posicion j
        estadoActual = vTransicion[j].res.estadoFinal;
        descinst.contenidoStack.pop();
        descinst.estadoActual = estadoActual;
        i++;
        descinst.palabraPorLeer = palabra.substr(i,palabra.length()-i);

        if(vTransicion[j].res.palabraStack != "e") { //Si hay que sacar el símbolo del stack no hace nada
            for (int k=vTransicion[j].res.palabraStack.length()-1; k>=0; k--){
                descinst.contenidoStack.push(vTransicion[j].res.palabraStack[k]);
            }
        }

        cout << "DI: (" << descinst.estadoActual << "," << descinst.palabraPorLeer << ",";
        imprimirPila(descinst.contenidoStack);
        cout << ")" << endl;

    }

    j = 0;
    while(j < vTransicion.size() && ( vTransicion[j].simbolo != 'e' || vTransicion[j].simboloStack != descinst.contenidoStack.top()) ) {
        j++;
    }
    if(j < vTransicion.size()) {
        descinst.estadoActual = vTransicion[j].res.estadoFinal;
        descinst.contenidoStack.pop();
        if(vTransicion[j].res.palabraStack != "e") { //Si hay que sacar el símbolo del stack no hace nada
            for (int k=vTransicion[j].res.palabraStack.size()-1; k>=0; k--){
                descinst.contenidoStack.push(vTransicion[j].res.palabraStack[k]);
            }
        }
    } else {
        cout << "La palabra ingresada no es aceptada por el lenguaje" << endl;
        return;
    }

    cout << "DI: (" << descinst.estadoActual << "," << descinst.palabraPorLeer << ",";
    imprimirPila(descinst.contenidoStack);
    cout << ")" << endl;
        
    if(metodo == false) {
        if(descinst.estadoActual == estadoFinal) {
            cout << "Se aceptó la palabra " << palabra << " por estado final" << endl;
            return;
        }
    } else {
        if(descinst.contenidoStack.empty()) {
            cout << "Se aceptó la palabra " << palabra << " por stack vacío" << endl;
            return;
        }
    }


}

string unCaracter(string texto) { //validacion para símbolo que lee
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.length() != 1 || (!islower(entrada[0]) && !isdigit(entrada[0]))); 

    return entrada;
}

string unCaracterMayus(string texto) {
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.length() != 1 || !isupper(entrada[0]));

    return entrada;
}

void imprimirPila(stack<char> pila) {
    while(!pila.empty()) {
        cout << pila.top();
        pila.pop();
    }
}