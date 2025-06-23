#include <iostream>
#include <cstring>
using namespace std;

struct Miembro {
    int id;
    //En este caso se usa -1 por que el miembro no tiene padre o madre en el árbol porque no se conoce, no se quiere registrar, o es un fundador.
    int idPadre;              // NUEVO: ID del padre (-1 si no tiene)
    int idMadre;              // NUEVO: ID de la madre (-1 si no tiene)
    char nombre[50];
    int generacion;
    char fechaNacimiento[20];
    char ocupacion[30];

    //Se asigna el valor recibido al atributo interno del miembro.
    Miembro(int _id, int _idPadre, int _idMadre, const char* _nombre, int _gen, 
            const char* _fecha, const char* _ocup) {
        id = _id;
        idPadre = _idPadre;
        idMadre = _idMadre;
        generacion = _gen;
        strcpy(nombre, _nombre);
        strcpy(fechaNacimiento, _fecha);
        strcpy(ocupacion, _ocup);
    }
    
    //Se usa -1 porque aún no se ha registrado el padre o la madre. Es como decir "no tiene" o "todavía no se sabe".
    Miembro() {
        id = 0;
        idPadre = -1;
        idMadre = -1;
        generacion = 0;
        strcpy(nombre, "");
        strcpy(fechaNacimiento, "");
        strcpy(ocupacion, "");
    }
};

struct Nodo {
    Miembro* miembro;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(Miembro* m) : miembro(m), izquierdo(nullptr), derecho(nullptr) {}
};

class ArbolGenealogico {
private:
    Nodo* raiz;

    Nodo* insertar(Nodo* nodo, Miembro* miembro) {
        if (nodo == nullptr) return new Nodo(miembro);
        if (miembro->id < nodo->miembro->id)
            nodo->izquierdo = insertar(nodo->izquierdo, miembro);
        else if (miembro->id > nodo->miembro->id)
            nodo->derecho = insertar(nodo->derecho, miembro);
        else
            cout << "Error: ID duplicado." << endl;
        return nodo;
    }

public:
    ArbolGenealogico() : raiz(nullptr) {
        cout << "Arbol genealogico creado." << endl;
    }

    void insertarMiembro(int id, const char* nombre, int gen, const char* fecha, const char* ocup) {
        Miembro* nuevo = new Miembro(id, nombre, gen, fecha, ocup);
        raiz = insertar(raiz, nuevo);
        cout << "Miembro insertado correctamente." << endl;
    }

    void mostrarMenu() {
        cout << "\n===== MENU PRINCIPAL =====" << endl;
        cout << "1. Insertar miembro" << endl;
        cout << "2. Buscar miembro por ID" << endl;
        cout << "3. Eliminar miembro" << endl;
        cout << "4. Mostrar genealogia" << endl;
        cout << "5. Mostrar jerarquia" << endl;
        cout << "6. Mostrar descendientes" << endl;
        cout << "7. Verificar pertenencia a rama" << endl;
        cout << "8. Salir" << endl;
        cout << "Seleccione una opcion (1-8): ";
    }
};

int main() {
    ArbolGenealogico arbol;
    int opcion;

    do {
        arbol.mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int id, gen;
                char nombre[50], fecha[20], ocup[30];
                cout << "\n=== INSERTAR MIEMBRO ===" << endl;
                cout << "ID: "; cin >> id; cin.ignore();
                cout << "Nombre: "; cin.getline(nombre, 50);
                cout << "Generacion: "; cin >> gen; cin.ignore();
                cout << "Fecha nacimiento: "; cin.getline(fecha, 20);
                cout << "Ocupacion: "; cin.getline(ocup, 30);
                arbol.insertarMiembro(id, nombre, gen, fecha, ocup);
                break;
            }
            default:
                cout << "\nEsta opcion aun no esta implementada." << endl;
                break;
        }

    } while (opcion != 8);

    return 0;
}
