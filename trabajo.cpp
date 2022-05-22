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
    int estado;
    string porLeer;
    string simbolosStack;
};

string unCaracter(string texto);
string unCaracterMayus(string texto);
void evaluarPalabra(vector<transicion> vTransicion, string palabra, bool metodo, int estadoFinal, int estadoInicial);
string extraerStr(stack<char> pila);
void imprimeDI(vector<DI> descripcionesInst);

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
    do {
        cout << "Ingrese la palabra a evaluar: ";
        cin >> palabra;
        evaluarPalabra(vTransicion, palabra, metodo, estadofinal, estadoini);
        cout << "Presione enter si quiere evaluar otra palabra, cualquier otra letra para parar... ";
        cin.ignore();
        getline(cin, seguir);
    } while(seguir == "");



    for(transicion a : vTransicion) {
        cout << "T(" << a.estadoInicial << ", " << a.simbolo << ", " << a.simboloStack << ")=(" << a.res.estadoFinal << ", " << a.res.palabraStack << ")" << endl;


    }

    return EXIT_SUCCESS;
}

void evaluarPalabra(vector<transicion> vTransicion, string palabra, bool metodo, int estadoFinal, int estadoInicial){
    vector<DI> descripcionesInst;
    DI di;
    stack<char> pila;

    //comenzar definiendo la primera descripcion segun la primera transicion de vTransicion
    di.estado = estadoInicial;
    di.porLeer = palabra;
    di.simbolosStack = vTransicion[0].simboloStack;
    descripcionesInst.push_back(di);
    pila.push(vTransicion[0].simboloStack);
    bool aceptado=false;

    //recorremos toda la palabra
    for (int i=0; i<palabra.size(); i++){
        aceptado = false;
        // buscamos en vTransicione que coincida vTransicion[x].simbolo y vTransicion[x].simboloStack
        for (int j=0; j<vTransicion.size();j++){
            if (vTransicion[j].estadoInicial == descripcionesInst[descripcionesInst.size()-1].estado && vTransicion[j].simbolo == palabra[i] && vTransicion[j].simboloStack==descripcionesInst[descripcionesInst.size()-1].simbolosStack[0]){
                di.estado = vTransicion[j].res.estadoFinal;
                // ve el caso cuando no queda mas palabra por leer
                di.porLeer = palabra.substr(i+1);
                if (i==palabra.size()-1) di.porLeer = "e";
                pila.pop();
                if (vTransicion[j].res.palabraStack!="e"){
                    for (int x=vTransicion[j].res.palabraStack.size()-1; x>=0; x--){
                        pila.push(vTransicion[j].res.palabraStack[x]);
                    }
                }
                di.simbolosStack = extraerStr(pila);
                descripcionesInst.push_back(di);
                aceptado = true;
            }
            if (aceptado) break;
        }
        if (!aceptado) break;
    }

    if (descripcionesInst[descripcionesInst.size()-1].simbolosStack != "R") aceptado = false;

    if (aceptado & metodo){
        di.estado = vTransicion[vTransicion.size()-1].res.estadoFinal;
        di.porLeer = "e";
        di.simbolosStack = "e";
    }
    else if (aceptado & !metodo)
    {
        di.estado = estadoFinal;
        di.porLeer = "e";
        di.simbolosStack = "R";
    }
    descripcionesInst.push_back(di);

    if (aceptado){
        cout << "Acepta la palabra " << palabra << endl;
        cout << "Descpriciones instantáneas: " << endl;
        imprimeDI(descripcionesInst);
    }
    else {
        cout << "No acepta la palabra " << palabra << endl;
        cout << "Porque: " << endl;
        imprimeDI(descripcionesInst);
    }
}

void imprimeDI(vector<DI> descripcionesInst){
    for (int i=0; i<descripcionesInst.size(); i++){
        cout << "(" << descripcionesInst[i].estado << ", " << descripcionesInst[i].porLeer << ", " << descripcionesInst[i].simbolosStack << ")";
        if (i!=descripcionesInst.size()-1){
            cout << "|------" ;
        }
        else{
            cout <<  endl;
        }
    }
}

string extraerStr(stack<char> pila){
    stack<char> pilaAux = pila;
    string pilaString = "";
    while (!pilaAux.empty()){
        pilaString += pilaAux.top();
        pilaAux.pop();
    }
    return pilaString;
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