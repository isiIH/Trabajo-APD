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
void imprimirPila(stack<char> pila);

int main(int argc, char **argv){

    int estadoini, estadofinal = -1;
    Transicion t;
    vector<Transicion> vTransicion;
    bool minus, metodo; // Stack vacío = true, Estado final = false;
    string palabra, seguir, entrada;

    // TRANSICIONES
    do {
        cout << "Nota: Tomaremos el símbolo del stack (z) de la primera transición que se ingrese como símbolo inicial." << endl;
        cout << "-------------------------------------\nIngrese una transición T(q,a,z)=(p,ß)\n-------------------------------------" << endl;

        t.estadoInicial = esNumero("Ingrese el estado en el que se encuentra el APD (q) como un número entero >= 0: ");

        cin.ignore();
        do {
            cout << "Ingrese símbolo (a) que se lee en minúsculas (enter para leer símbolo vacío): ";
            getline(cin, entrada);

        } while(entrada != "" && (entrada.length() != 1 || (isupper(entrada[0]) && !isdigit(entrada[0]))) );
        if(entrada == "") t.simbolo = 'E';
        else t.simbolo = entrada[0];

        t.simboloStack = unCaracterMayus("Ingrese símbolo (z) en el tope de la pila: "); // Puede ser solo un char upper o número

        t.res.estadoFinal = esNumero("Ingrese estado al que llega la transición (p) como un número entero >= 0: ");

        cin.ignore();
        do {
            cout << "Ingrese conjunto de símbolos para reemplazar en la pila del stack (ß) como un string en mayúsculas (enter si quiere sacar elementos del stack): ";
            getline(cin,t.res.palabraStack); // Validar si está en mayúsculas o hay vacía
            
            minus = false;

            for(char c : t.res.palabraStack) {
                if(islower(c)) minus = true;
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
    if(unCaracter("Ingrese si el APD acepta por estado final 'f' o por stack vacío 'v' (o cualquier otro caracter): ") == 'v') metodo = true;
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
    cout << endl;

    //EVALUAR LA PALABRA
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
    while (descinst.palabraPorLeer != ""){
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

        // Despues del while se encuentra la transicion en la posicion j
        descinst.contenidoStack.pop();
        descinst.estadoActual = vTransicion[j].res.estadoFinal;
        i++;
        descinst.palabraPorLeer = palabra.substr(i,palabra.length()-i);

        if(vTransicion[j].res.palabraStack != "") { //Si hay que sacar el símbolo del stack no hace nada
            for (int k=vTransicion[j].res.palabraStack.length()-1; k>=0; k--){
                descinst.contenidoStack.push(vTransicion[j].res.palabraStack[k]);
            }
        }

        cout << "DI: (" << descinst.estadoActual << ",";
        if(descinst.palabraPorLeer == "") cout << "ε" << ",";
        else cout << descinst.palabraPorLeer << ",";
        if(descinst.contenidoStack.empty()) cout << "ε";
        else imprimirPila(descinst.contenidoStack);
        cout << ")" << endl;

    }

    if(metodo == false) { //ACEPTAR POR ESTADO FINAL
        while(descinst.estadoActual != estadoFinal) {
            j = 0;
            while(j < vTransicion.size() && ( descinst.estadoActual != vTransicion[j].estadoInicial || vTransicion[j].simbolo != 'E' || vTransicion[j].simboloStack != descinst.contenidoStack.top()) ) {
                j++;
            }
            if(j >= vTransicion.size()) {
                cout << "La palabra ingresada no es aceptada por el lenguaje" << endl;
                return;
            }
            descinst.estadoActual = vTransicion[j].res.estadoFinal;
            descinst.contenidoStack.pop();
            if(vTransicion[j].res.palabraStack != "") { //Si hay que sacar el símbolo del stack no hace nada
                for (int k=vTransicion[j].res.palabraStack.size()-1; k>=0; k--){
                    descinst.contenidoStack.push(vTransicion[j].res.palabraStack[k]);
                }
            }

            cout << "DI: (" << descinst.estadoActual << ",";
            if(descinst.palabraPorLeer == "") cout << "ε" << ",";
            else cout << descinst.palabraPorLeer << ",";
            if(descinst.contenidoStack.empty()) cout << "ε";
            else imprimirPila(descinst.contenidoStack);
            cout << ")" << endl;
        }
        cout << "Se aceptó la palabra " << palabra << " por estado final" << endl;
        return;

    } else { //ACEPTAR POR STACK VACÍO
        while(!descinst.contenidoStack.empty()){
            j = 0;
            while(j < vTransicion.size() && ( descinst.estadoActual != vTransicion[j].estadoInicial || vTransicion[j].simbolo != 'E' || vTransicion[j].simboloStack != descinst.contenidoStack.top()) ) {
                j++;
            }
            if(j >= vTransicion.size()) {
                cout << "La palabra ingresada no es aceptada por el lenguaje" << endl;
                return;
            }
            descinst.estadoActual = vTransicion[j].res.estadoFinal;
            descinst.contenidoStack.pop();
            if(vTransicion[j].res.palabraStack != "") { //Si hay que sacar el símbolo del stack no hace nada
                for (int k=vTransicion[j].res.palabraStack.size()-1; k>=0; k--){
                    descinst.contenidoStack.push(vTransicion[j].res.palabraStack[k]);
                }
            }

            cout << "DI: (" << descinst.estadoActual << ",";
            if(descinst.palabraPorLeer == "") cout << "ε" << ",";
            else cout << descinst.palabraPorLeer << ",";
            if(descinst.contenidoStack.empty()) cout << "ε";
            else imprimirPila(descinst.contenidoStack);
            cout << ")" << endl;
        }
        cout << "Se aceptó la palabra " << palabra << " por stack vacío" << endl;
        return;
    }

}

char unCaracter(string texto) { // Validacion para símbolo que lee
    string entrada;

    do {
        cout << texto;
        cin >> entrada;
    } while(entrada.length() != 1 || (isupper(entrada[0]) && !isdigit(entrada[0])) ); 
    //                             A        v                       v               v
    //                             a        f                       v               f
    //                             1        v                       f               f

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
    } while(entrada.length() != 1 || islower(entrada[0]));

    return entrada[0];
}

void imprimirPila(stack<char> pila) {
    while(!pila.empty()) {
        cout << pila.top();
        pila.pop();
    }
}