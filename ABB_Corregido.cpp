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


// ESTRUCTURA NODO
struct Nodo {
    Miembro* miembro;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(Miembro* m) : miembro(m), izquierdo(nullptr), derecho(nullptr) {}
};


// CLASE ARBOL GENEALOGICO 
class ArbolGenealogico {
private:
    private:
    Nodo* raiz;
    static int contadorID;  // Para generar IDs automáticos
    
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
    
    Nodo* buscar(Nodo* nodo, int id) {
        if (nodo == nullptr || nodo->miembro->id == id)
            return nodo;
        
        if (id < nodo->miembro->id)
            return buscar(nodo->izquierdo, id);
        
        return buscar(nodo->derecho, id);
    }
    
    Nodo* encontrarMinimo(Nodo* nodo) {
        while (nodo->izquierdo != nullptr)
            nodo = nodo->izquierdo;
        return nodo;
    }
    
    Nodo* eliminar(Nodo* nodo, int id) {
        if (nodo == nullptr) return nodo;
        
        if (id < nodo->miembro->id)
            nodo->izquierdo = eliminar(nodo->izquierdo, id);
        else if (id > nodo->miembro->id)
            nodo->derecho = eliminar(nodo->derecho, id);
        else {
            // Verificar si tiene hijos antes de eliminar
            if (tieneHijos(id)) {
                cout << "No se puede eliminar: tiene descendientes." << endl;
                return nodo;
            }
            
            // Casos de eliminación normales
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                delete nodo->miembro;
                delete nodo;
                return nullptr;
            }
            else if (nodo->izquierdo == nullptr) {
                Nodo* temp = nodo->derecho;
                delete nodo->miembro;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr) {
                Nodo* temp = nodo->izquierdo;
                delete nodo->miembro;
                delete nodo;
                return temp;
            }
            else {
                Nodo* temp = encontrarMinimo(nodo->derecho);
                *(nodo->miembro) = *(temp->miembro);
                nodo->derecho = eliminar(nodo->derecho, temp->miembro->id);
            }
        }
        return nodo;
    }
    
    void recorridoInorden(Nodo* nodo) {
        if (nodo != nullptr) {
            recorridoInorden(nodo->izquierdo);
            mostrarMiembroResumen(nodo->miembro);
            recorridoInorden(nodo->derecho);
        }
    }
    
    // Buscar todos los hijos de un ID específico
    void buscarHijos(Nodo* nodo, int idPadre, vector<Miembro*>& hijos) {
        if (nodo != nullptr) {
            if (nodo->miembro->idPadre == idPadre || nodo->miembro->idMadre == idPadre) {
                hijos.push_back(nodo->miembro);
            }
            buscarHijos(nodo->izquierdo, idPadre, hijos);
            buscarHijos(nodo->derecho, idPadre, hijos);
        }
    }
    
    // Verificar si alguien tiene hijos
    bool tieneHijos(int id) {
        vector<Miembro*> hijos;
        buscarHijos(raiz, id, hijos);
        return !hijos.empty();
    }
    
    // Buscar todos los miembros y almacenarlos en un vector
    void obtenerTodos(Nodo* nodo, vector<Miembro*>& todos) {
        if (nodo != nullptr) {
            todos.push_back(nodo->miembro);
            obtenerTodos(nodo->izquierdo, todos);
            obtenerTodos(nodo->derecho, todos);
        }
    }
    
    void mostrarMiembroResumen(Miembro* m) {
        cout << "ID: " << m->id << " | " << m->nombre 
             << " | Gen: " << m->generacion;
        if (m->idPadre != -1) cout << " | Padre: " << m->idPadre;
        if (m->idMadre != -1) cout << " | Madre: " << m->idMadre;
        cout << endl;
    }
    
    void mostrarMiembroCompleto(Miembro* m) {
        cout << "\n=== Informacion del Miembro ===" << endl;
        cout << "ID: " << m->id << endl;
        cout << "Nombre: " << m->nombre << endl;
        cout << "Generacion: " << m->generacion << endl;
        cout << "Fecha Nacimiento: " << m->fechaNacimiento << endl;
        cout << "Ocupacion: " << m->ocupacion << endl;
        if (m->idPadre != -1) {
            Nodo* padre = buscar(raiz, m->idPadre);
            if (padre) cout << "Padre: " << padre->miembro->nombre << " (ID: " << m->idPadre << ")" << endl;
        }
        if (m->idMadre != -1) {
            Nodo* madre = buscar(raiz, m->idMadre);
            if (madre) cout << "Madre: " << madre->miembro->nombre << " (ID: " << m->idMadre << ")" << endl;
        }
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
