#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
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
        cout << "Sistema de Arbol Genealogico creado." << endl;
        cout << "Nota: Use ID -1 cuando no conozca el padre o madre." << endl;
    }
    
    int generarID() {
        return ++contadorID;
    }
    
    void insertarMiembro(int id, int idPadre, int idMadre, const char* nombre, 
                        int gen, const char* fecha, const char* ocup) {
        // Validar que los padres existan si se especifican
        if (idPadre != -1 && buscar(raiz, idPadre) == nullptr) {
            cout << "Error: El padre con ID " << idPadre << " no existe." << endl;
            return;
        }
        if (idMadre != -1 && buscar(raiz, idMadre) == nullptr) {
            cout << "Error: La madre con ID " << idMadre << " no existe." << endl;
            return;
        }
        
        // Calcular generación automáticamente si tiene padres
        int generacionCalculada = gen;
        if (idPadre != -1) {
            Nodo* padre = buscar(raiz, idPadre);
            if (padre) generacionCalculada = padre->miembro->generacion + 1;
        } else if (idMadre != -1) {
            Nodo* madre = buscar(raiz, idMadre);
            if (madre) generacionCalculada = madre->miembro->generacion + 1;
        }
        
        Miembro* nuevo = new Miembro(id, idPadre, idMadre, nombre, generacionCalculada, fecha, ocup);
        raiz = insertar(raiz, nuevo);
        cout << "Miembro insertado correctamente con ID: " << id << endl;
        
        if (id > contadorID) contadorID = id;  // Actualizar contador
    }
    
    void insertarHijo() {
        int idPadre, idMadre;
        char nombre[50], fecha[20], ocup[30];
        
        cout << "\n=== INSERTAR HIJO/DESCENDIENTE ===" << endl;
        cout << "ID del padre (-1 si no aplica): ";
        cin >> idPadre;
        cout << "ID de la madre (-1 si no aplica): ";
        cin >> idMadre;
        
        if (idPadre == -1 && idMadre == -1) {
            cout << "Error: Debe especificar al menos un padre o madre." << endl;
            return;
        }
        
        cin.ignore();
        cout << "Nombre del hijo: ";
        cin.getline(nombre, 50);
        cout << "Fecha nacimiento: ";
        cin.getline(fecha, 20);
        cout << "Ocupacion: ";
        cin.getline(ocup, 30);
        
        int nuevoID = generarID();
        insertarMiembro(nuevoID, idPadre, idMadre, nombre, 0, fecha, ocup);
    }
    
    void buscarMiembro(int id) {
        Nodo* encontrado = buscar(raiz, id);
        if (encontrado != nullptr) {
            mostrarMiembroCompleto(encontrado->miembro);
        } else {
            cout << "Miembro con ID " << id << " no encontrado." << endl;
        }
    }
    
    void mostrarAncestros(int id) {
        cout << "\n=== Ancestros de ID " << id << " ===" << endl;
        Nodo* nodo = buscar(raiz, id);
        if (nodo == nullptr) {
            cout << "Miembro no encontrado." << endl;
            return;
        }
        
        cout << "Linea de ancestros:" << endl;
        Miembro* actual = nodo->miembro;
        int nivel = 0;
        
        while (actual != nullptr && (actual->idPadre != -1 || actual->idMadre != -1)) {
            if (actual->idPadre != -1) {
                Nodo* padre = buscar(raiz, actual->idPadre);
                if (padre) {
                    for (int i = 0; i <= nivel; i++) cout << "  ";
                    cout << "Padre: " << padre->miembro->nombre << " (ID: " << actual->idPadre << ")" << endl;
                }
            }
            if (actual->idMadre != -1) {
                Nodo* madre = buscar(raiz, actual->idMadre);
                if (madre) {
                    for (int i = 0; i <= nivel; i++) cout << "  ";
                    cout << "Madre: " << madre->miembro->nombre << " (ID: " << actual->idMadre << ")" << endl;
                }
            }
            
            // Continuar con el padre para la siguiente iteración
            if (actual->idPadre != -1) {
                Nodo* siguiente = buscar(raiz, actual->idPadre);
                if (siguiente) {
                    actual = siguiente->miembro;
                    nivel++;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
    
    void mostrarDescendientes(int id) {
        cout << "\n=== Descendientes de ID " << id << " ===" << endl;
        Nodo* nodo = buscar(raiz, id);
        if (nodo == nullptr) {
            cout << "Miembro no encontrado." << endl;
            return;
        }
        
        cout << "Mostrando: " << nodo->miembro->nombre << endl;
        mostrarDescendientesRecursivo(id, 1);
    }
    
    void mostrarDescendientesRecursivo(int idAncestro, int nivel) {
        vector<Miembro*> hijos;
        buscarHijos(raiz, idAncestro, hijos);
        
        for (Miembro* hijo : hijos) {
            for (int i = 0; i < nivel; i++) cout << "  ";
            cout << "└─ " << hijo->nombre << " (ID: " << hijo->id << ")" << endl;
            mostrarDescendientesRecursivo(hijo->id, nivel + 1);
        }
    }
    
    void verificarRelacionFamiliar() {
        int id1, id2;
        cout << "\n=== Verificar Relacion Familiar ===" << endl;
        cout << "ID del posible ancestro: ";
        cin >> id1;
        cout << "ID del posible descendiente: ";
        cin >> id2;
        
        if (esDescendiente(id1, id2)) {
            cout << "SI: " << id2 << " es descendiente de " << id1 << endl;
        } else if (esDescendiente(id2, id1)) {
            cout << "SI: " << id1 << " es descendiente de " << id2 << endl;
        } else {
            cout << "NO: No hay relacion directa de ancestro-descendiente." << endl;
        }
    }
    
    bool esDescendiente(int idAncestro, int idPosibleDesc) {
        Nodo* nodo = buscar(raiz, idPosibleDesc);
        if (nodo == nullptr) return false;
        
        Miembro* actual = nodo->miembro;
        while (actual != nullptr && (actual->idPadre != -1 || actual->idMadre != -1)) {
            if (actual->idPadre == idAncestro || actual->idMadre == idAncestro) {
                return true;
            }
            
            // Continuar buscando en los ancestros
            if (actual->idPadre != -1) {
                Nodo* padre = buscar(raiz, actual->idPadre);
                if (padre) {
                    actual = padre->miembro;
                } else {
                    break;
                }
            } else if (actual->idMadre != -1) {
                Nodo* madre = buscar(raiz, actual->idMadre);
                if (madre) {
                    actual = madre->miembro;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        return false;
    }
    
    void mostrarGenealogiaCompleta() {
        cout << "\n=== Genealogia Completa (Orden por ID) ===" << endl;
        if (raiz == nullptr) {
            cout << "El arbol esta vacio." << endl;
        } else {
            recorridoInorden(raiz);
        }
    }
    
    void mostrarArbolFamiliar() {
        cout << "\n=== Arbol Familiar Completo ===" << endl;
        if (raiz == nullptr) {
            cout << "El arbol esta vacio." << endl;
            return;
        }
        
        // Encontrar los miembros sin padres (raíces familiares)
        vector<Miembro*> todos;
        obtenerTodos(raiz, todos);
        
        for (Miembro* m : todos) {
            if (m->idPadre == -1 && m->idMadre == -1) {
                cout << "\nFamilia de: " << m->nombre << " (ID: " << m->id << ")" << endl;
                mostrarDescendientesRecursivo(m->id, 1);
            }
        }
    }
    
    void eliminarMiembro(int id) {
        if (tieneHijos(id)) {
            cout << "Error: No se puede eliminar porque tiene descendientes." << endl;
            cout << "Elimine primero a los descendientes." << endl;
            return;
        }
        
        if (buscar(raiz, id) != nullptr) {
            raiz = eliminar(raiz, id);
            cout << "Miembro eliminado correctamente." << endl;
        } else {
            cout << "No se puede eliminar. ID no existe." << endl;
        }
    }
};

// Inicializar el contador estático
int ArbolGenealogico::contadorID = 1000;

void mostrarMenu() {
    cout << "\n===== MENU PRINCIPAL =====" << endl;
    cout << "1. Insertar miembro (con ID manual)" << endl;
    cout << "2. Insertar hijo/descendiente (ID automatico)" << endl;
    cout << "3. Buscar miembro por ID" << endl;
    cout << "4. Mostrar ancestros de un miembro" << endl;
    cout << "5. Mostrar descendientes de un miembro" << endl;
    cout << "6. Verificar relacion familiar" << endl;
    cout << "7. Mostrar genealogia completa" << endl;
    cout << "8. Mostrar arbol familiar" << endl;
    cout << "9. Eliminar miembro" << endl;
    cout << "10. Cargar datos de ejemplo" << endl;
    cout << "0. Salir" << endl;
}

int main() {
    ArbolGenealogico arbol;
    int opcion;
    
    cout << "=== SISTEMA DE ARBOL GENEALOGICO ABB ===" << endl;
    cout << "=== Con Relaciones Familiares Reales ===" << endl;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                int id, idPadre, idMadre, gen;
                char nombre[50], fecha[20], ocup[30];
                
                cout << "\n=== INSERTAR MIEMBRO ===" << endl;
                cout << "ID: ";
                cin >> id;
                cout << "ID del padre (-1 si no tiene): ";
                cin >> idPadre;
                cout << "ID de la madre (-1 si no tiene): ";
                cin >> idMadre;
                cin.ignore();
                cout << "Nombre: ";
                cin.getline(nombre, 50);
                cout << "Generacion (0 para calcular automaticamente): ";
                cin >> gen;
                cin.ignore();
                cout << "Fecha nacimiento: ";
                cin.getline(fecha, 20);
                cout << "Ocupacion: ";
                cin.getline(ocup, 30);
                
                arbol.insertarMiembro(id, idPadre, idMadre, nombre, gen, fecha, ocup);
                break;
            }
            
            case 2:
                arbol.insertarHijo();
                break;
                
            case 3: {
                int id;
                cout << "\nID a buscar: ";
                cin >> id;
                arbol.buscarMiembro(id);
                break;
            }
            
            case 4: {
                int id;
                cout << "\nID para ver ancestros: ";
                cin >> id;
                arbol.mostrarAncestros(id);
                break;
            }
            
            case 5: {
                int id;
                cout << "\nID para ver descendientes: ";
                cin >> id;
                arbol.mostrarDescendientes(id);
                break;
            }
            
            case 6:
                arbol.verificarRelacionFamiliar();
                break;
                
            case 7:
                arbol.mostrarGenealogiaCompleta();
                break;
                
            case 8:
                arbol.mostrarArbolFamiliar();
                break;
                
            case 9: {
                int id;
                cout << "\nID a eliminar: ";
                cin >> id;
                arbol.eliminarMiembro(id);
                break;
            }
            
            case 10:
                cargarDatosEjemplo(arbol);
                break;
                
            case 0:
                cout << "\nGracias por usar el sistema!" << endl;
                break;
                
            default:
                cout << "\nOpcion no valida." << endl;
        }
    } while (opcion != 0);
    
    return 0;
}
