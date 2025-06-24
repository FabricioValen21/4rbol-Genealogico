#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

// Estructura que representa a una persona en el árbol familiar
struct Miembro {
    int id;                  // ID único de la persona
    int idPadre;             // ID del padre (-1 si no tiene)
    int idMadre;             // ID de la madre (-1 si no tiene)
    char nombre[50];         // Nombre de la persona
    int generacion;          // Generación (1=abuelos, 2=padres, 3=hijos)
    char fechaNacimiento[20]; // Fecha de nacimiento
    char ocupacion[30];      // Trabajo/profesión
    
    // Constructor con todos los datos
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

    // Constructor vacío con valores por defecto
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

// Nodo del árbol binario
struct Nodo {
    Miembro* miembro;    // Persona almacenada
    Nodo* izquierdo;     // Hijo izquierdo (IDs menores)
    Nodo* derecho;       // Hijo derecho (IDs mayores)

    Nodo(Miembro* m) : miembro(m), izquierdo(NULL), derecho(NULL) {}
};

// Clase principal que maneja el árbol genealógico
class ArbolGenealogico {
private:
    Nodo* raiz;              // Nodo raíz del árbol
    static int contadorID;   // Contador para generar IDs automáticos

    // Inserta un miembro en el árbol ordenado por ID
    Nodo* insertar(Nodo* nodo, Miembro* miembro) {
        if (nodo == NULL) return new Nodo(miembro);

        // Decidir si va a la izquierda o derecha según el ID
        if (miembro->id < nodo->miembro->id)
            nodo->izquierdo = insertar(nodo->izquierdo, miembro);
        else if (miembro->id > nodo->miembro->id)
            nodo->derecho = insertar(nodo->derecho, miembro);
        else
            cout << "Error: ID duplicado." << endl;

        return nodo;
    }

    // Busca un miembro por su ID
    Nodo* buscar(Nodo* nodo, int id) {
        if (nodo == NULL || nodo->miembro->id == id)
            return nodo;
        if (id < nodo->miembro->id)
            return buscar(nodo->izquierdo, id);
        return buscar(nodo->derecho, id);
    }

    // Encuentra el nodo con el ID más pequeño
    Nodo* encontrarMinimo(Nodo* nodo) {
        while (nodo->izquierdo != NULL)
            nodo = nodo->izquierdo;
        return nodo;
    }

    // Elimina un miembro del árbol
    Nodo* eliminar(Nodo* nodo, int id) {
        if (nodo == NULL) return nodo;

        if (id < nodo->miembro->id)
            nodo->izquierdo = eliminar(nodo->izquierdo, id);
        else if (id > nodo->miembro->id)
            nodo->derecho = eliminar(nodo->derecho, id);
        else {
            // No permitir eliminar si tiene hijos
            if (tieneHijos(id)) {
                cout << "No se puede eliminar: tiene descendientes." << endl;
                return nodo;
            }

            // Caso 1: Sin hijos
            if (nodo->izquierdo == NULL && nodo->derecho == NULL) {
                delete nodo->miembro;
                delete nodo;
                return NULL;
            }
            // Caso 2: Un hijo
            else if (nodo->izquierdo == NULL) {
                Nodo* temp = nodo->derecho;
                delete nodo->miembro;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == NULL) {
                Nodo* temp = nodo->izquierdo;
                delete nodo->miembro;
                delete nodo;
                return temp;
            } 
            // Caso 3: Dos hijos - reemplazar con el sucesor
            else {
                Nodo* temp = encontrarMinimo(nodo->derecho);
                
                delete nodo->miembro;
                nodo->miembro = new Miembro(temp->miembro->id, temp->miembro->idPadre, 
                                          temp->miembro->idMadre, temp->miembro->nombre,
                                          temp->miembro->generacion, temp->miembro->fechaNacimiento,
                                          temp->miembro->ocupacion);
                
                nodo->derecho = eliminar(nodo->derecho, temp->miembro->id);
            }
        }
        return nodo;
    }

    // Recorre el árbol en orden (izquierda -> raíz -> derecha)
    void recorridoInorden(Nodo* nodo) {
        if (nodo != NULL) {
            recorridoInorden(nodo->izquierdo);
            mostrarMiembroResumen(nodo->miembro);
            recorridoInorden(nodo->derecho);
        }
    }

    // Busca todos los hijos de un padre específico
    void buscarHijos(Nodo* nodo, int idPadre, vector<Miembro*>& hijos) {
        if (nodo != NULL) {
            if (nodo->miembro->idPadre == idPadre || nodo->miembro->idMadre == idPadre) {
                hijos.push_back(nodo->miembro);
            }
            buscarHijos(nodo->izquierdo, idPadre, hijos);
            buscarHijos(nodo->derecho, idPadre, hijos);
        }
    }

    // Verifica si un miembro tiene hijos
    bool tieneHijos(int id) {
        vector<Miembro*> hijos;
        buscarHijos(raiz, id, hijos);
        return !hijos.empty();
    }

    // Obtiene todos los miembros del árbol
    void obtenerTodos(Nodo* nodo, vector<Miembro*>& todos) {
        if (nodo != NULL) {
            obtenerTodos(nodo->izquierdo, todos);
            todos.push_back(nodo->miembro);
            obtenerTodos(nodo->derecho, todos);
        }
    }

    // Muestra información básica de un miembro
    void mostrarMiembroResumen(Miembro* m) {
        cout << "ID: " << m->id << " | " << m->nombre 
             << " | Gen: " << m->generacion;
        if (m->idPadre != -1) cout << " | Padre: " << m->idPadre;
        if (m->idMadre != -1) cout << " | Madre: " << m->idMadre;
        cout << endl;
    }

    // Muestra toda la información de un miembro
    void mostrarMiembroCompleto(Miembro* m) {
        cout << "\n=== Informacion del Miembro ===" << endl;
        cout << "ID: " << m->id << endl;
        cout << "Nombre: " << m->nombre << endl;
        cout << "Generacion: " << m->generacion << endl;
        cout << "Fecha Nacimiento: " << m->fechaNacimiento << endl;
        cout << "Ocupacion: " << m->ocupacion << endl;
        
        // Buscar y mostrar información de los padres
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
    // Constructor de la clase
    ArbolGenealogico() : raiz(NULL) {
        cout << "Sistema de Arbol Genealogico creado." << endl;
        cout << "Nota: Use ID -1 cuando no conozca el padre o madre." << endl;
    }

    // Genera un nuevo ID automático
    int generarID() {
        return ++contadorID;
    }

    // Inserta un nuevo miembro en el árbol
    void insertarMiembro(int id, int idPadre, int idMadre, const char* nombre,
                         int gen, const char* fecha, const char* ocup) {
        // Verificar que el ID no exista
        if (buscar(raiz, id) != NULL) {
            cout << "Error: El ID " << id << " ya existe." << endl;
            return;
        }
        
        // Verificar que los padres existan si se especifican
        if (idPadre != -1 && buscar(raiz, idPadre) == NULL) {
            cout << "Error: El padre con ID " << idPadre << " no existe." << endl;
            return;
        }
        if (idMadre != -1 && buscar(raiz, idMadre) == NULL) {
            cout << "Error: La madre con ID " << idMadre << " no existe." << endl;
            return;
        }

        // Calcular generación automáticamente si es 0
        int generacionCalculada = gen;
        if (gen == 0) {
            if (idPadre != -1) {
                Nodo* padre = buscar(raiz, idPadre);
                generacionCalculada = padre->miembro->generacion + 1;
            } else if (idMadre != -1) {
                Nodo* madre = buscar(raiz, idMadre);
                generacionCalculada = madre->miembro->generacion + 1;
            } else {
                generacionCalculada = 1; // Primera generación
            }
        }

        // Crear e insertar el nuevo miembro
        Miembro* nuevo = new Miembro(id, idPadre, idMadre, nombre, generacionCalculada, fecha, ocup);
        raiz = insertar(raiz, nuevo);
        cout << "Miembro insertado correctamente con ID: " << id << endl;

        // Actualizar contador si es necesario
        if (id > contadorID) contadorID = id;
    }

    // Función para insertar hijos con ID automático
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

    // Busca y muestra un miembro por ID
    void buscarMiembro(int id) {
        Nodo* encontrado = buscar(raiz, id);
        if (encontrado != NULL)
            mostrarMiembroCompleto(encontrado->miembro);
        else
            cout << "Miembro con ID " << id << " no encontrado." << endl;
    }

    // Muestra la línea de ancestros de un miembro
    void mostrarAncestros(int id) {
        Nodo* nodo = buscar(raiz, id);
        if (nodo == NULL) {
            cout << "Miembro no encontrado." << endl;
            return;
        }

        cout << "\nLinea de ancestros de " << nodo->miembro->nombre << ":" << endl;
        mostrarAncestrosRecursivo(id, 0);
    }
    
    // Función auxiliar recursiva para mostrar ancestros
    void mostrarAncestrosRecursivo(int id, int nivel) {
        Nodo* nodo = buscar(raiz, id);
        if (nodo == NULL) return;
        
        Miembro* actual = nodo->miembro;
        
        // Mostrar padre si existe
        if (actual->idPadre != -1) {
            Nodo* padre = buscar(raiz, actual->idPadre);
            if (padre) {
                for (int i = 0; i < nivel; i++) cout << "  ";
                cout << "Padre: " << padre->miembro->nombre << " (ID: " << actual->idPadre << ")" << endl;
                mostrarAncestrosRecursivo(actual->idPadre, nivel + 1);
            }
        }
        
        // Mostrar madre si existe
        if (actual->idMadre != -1) {
            Nodo* madre = buscar(raiz, actual->idMadre);
            if (madre) {
                for (int i = 0; i < nivel; i++) cout << "  ";
                cout << "Madre: " << madre->miembro->nombre << " (ID: " << actual->idMadre << ")" << endl;
                mostrarAncestrosRecursivo(actual->idMadre, nivel + 1);
            }
        }
    }

    // Muestra todos los descendientes de un miembro
    void mostrarDescendientes(int id) {
        Nodo* nodo = buscar(raiz, id);
        if (nodo == NULL) {
            cout << "Miembro no encontrado." << endl;
            return;
        }

        cout << "Mostrando descendencia de: " << nodo->miembro->nombre << endl;
        mostrarDescendientesRecursivo(id, 1);
    }

    // Función auxiliar recursiva para mostrar descendientes
    void mostrarDescendientesRecursivo(int idAncestro, int nivel) {
        vector<Miembro*> hijos;
        buscarHijos(raiz, idAncestro, hijos);

        for (size_t i = 0; i < hijos.size(); i++) {
            Miembro* hijo = hijos[i];
            for (int j = 0; j < nivel; j++) cout << "  ";
            cout << ">> " << hijo->nombre << " (ID: " << hijo->id << ")" << endl;
            mostrarDescendientesRecursivo(hijo->id, nivel + 1);
        }
    }

    // Verifica si hay relación familiar entre dos miembros
    void verificarRelacionFamiliar() {
        int id1, id2;
        cout << "ID del posible ancestro: ";
        cin >> id1;
        cout << "ID del posible descendiente: ";
        cin >> id2;

        if (esDescendiente(id1, id2))
            cout << "SI: " << id2 << " es descendiente de " << id1 << endl;
        else if (esDescendiente(id2, id1))
            cout << "SI: " << id1 << " es descendiente de " << id2 << endl;
        else
            cout << "NO hay relación directa de ancestro-descendiente." << endl;
    }

    // Verifica si una persona es descendiente de otra
    bool esDescendiente(int idAncestro, int idPosibleDesc) {
        Nodo* nodo = buscar(raiz, idPosibleDesc);
        if (nodo == NULL) return false;

        Miembro* actual = nodo->miembro;
        while (actual != NULL && (actual->idPadre != -1 || actual->idMadre != -1)) {
            if (actual->idPadre == idAncestro || actual->idMadre == idAncestro)
                return true;

            // Verificar ambos padres recursivamente
            if (actual->idPadre != -1 && esDescendiente(idAncestro, actual->idPadre))
                return true;
            if (actual->idMadre != -1 && esDescendiente(idAncestro, actual->idMadre))
                return true;
                
            break; // Evitar bucle infinito
        }
        return false;
    }

    // Muestra todos los miembros ordenados por ID
    void mostrarGenealogiaCompleta() {
        if (raiz == NULL)
            cout << "El arbol esta vacio." << endl;
        else {
            cout << "\n=== GENEALOGIA COMPLETA (ordenada por ID) ===" << endl;
            recorridoInorden(raiz);
        }
    }

    // Muestra la estructura familiar organizada
    void mostrarArbolFamiliar() {
        vector<Miembro*> todos;
        obtenerTodos(raiz, todos);

        cout << "\n=== ESTRUCTURA FAMILIAR ===" << endl;
        for (size_t i = 0; i < todos.size(); i++) {
            Miembro* m = todos[i];
            // Mostrar solo miembros sin padres (fundadores de familia)
            if (m->idPadre == -1 && m->idMadre == -1) {
                cout << "\nFamilia de: " << m->nombre << " (ID: " << m->id << ")" << endl;
                mostrarDescendientesRecursivo(m->id, 1);
            }
        }
    }

    // Elimina un miembro del árbol
    void eliminarMiembro(int id) {
        if (buscar(raiz, id) == NULL) {
            cout << "Error: No se puede eliminar. ID no existe." << endl;
            return;
        }
        
        if (tieneHijos(id)) {
            cout << "Error: No se puede eliminar porque tiene descendientes." << endl;
            return;
        }

        raiz = eliminar(raiz, id);
        cout << "Miembro eliminado correctamente." << endl;
    }

    // Carga datos de ejemplo para probar el sistema
    void cargarDatosEjemplo() {
        cout << "\nCargando datos de ejemplo..." << endl;
        insertarMiembro(1001, -1, -1, "Ezio Auditore", 1, "1459", "Asesino");
        insertarMiembro(1002, -1, -1, "Sofia Sartor", 1, "1468", "Escritora");
        insertarMiembro(1003, 1001, 1002, "Flavia Auditore", 0, "1490", "Arquitecta");
        insertarMiembro(1004, 1001, 1002, "Marcello Auditore", 0, "1492", "Comerciante");
        cout << "Datos de ejemplo cargados correctamente." << endl;
    }
    
    // Destructor para liberar memoria
    ~ArbolGenealogico() {
        destruirArbol(raiz);
    }
    
private:
    // Función auxiliar para destruir el árbol
    void destruirArbol(Nodo* nodo) {
        if (nodo != NULL) {
            destruirArbol(nodo->izquierdo);
            destruirArbol(nodo->derecho);
            delete nodo->miembro;
            delete nodo;
        }
    }
};

// Variable estática para generar IDs únicos
int ArbolGenealogico::contadorID = 1000;

// Muestra el menú principal
void mostrarMenu() {
    cout << "\n===== MENU PRINCIPAL =====" << endl;
    cout << "1. Insertar miembro (ID manual)" << endl;
    cout << "2. Insertar hijo/descendiente (ID automatico)" << endl;
    cout << "3. Buscar miembro por ID" << endl;
    cout << "4. Mostrar ancestros de un miembro" << endl;
    cout << "5. Mostrar descendientes de un miembro" << endl;
    cout << "6. Verificar relacion familiar" << endl;
    cout << "7. Mostrar genealogia completa (Inorden)" << endl;
    cout << "8. Mostrar arbol familiar" << endl;
    cout << "9. Eliminar miembro" << endl;
    cout << "10. Cargar datos de ejemplo" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

// Función principal
int main() {
    ArbolGenealogico arbol;  // Crear el árbol genealógico
    int opcion;

    // Bucle principal del programa
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                // Insertar miembro con ID manual
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
                // Insertar hijo con ID automático
                arbol.insertarHijo();
                break;

            case 3: {
                // Buscar miembro por ID
                int id;
                cout << "ID a buscar: ";
                cin >> id;
                arbol.buscarMiembro(id);
                break;
            }

            case 4: {
                // Mostrar ancestros
                int id;
                cout << "ID para ver ancestros: ";
                cin >> id;
                arbol.mostrarAncestros(id);
                break;
            }

            case 5: {
                // Mostrar descendientes
                int id;
                cout << "ID para ver descendientes: ";
                cin >> id;
                arbol.mostrarDescendientes(id);
                break;
            }

            case 6:
                // Verificar relación familiar
                arbol.verificarRelacionFamiliar();
                break;
                
            case 7:
                // Mostrar genealogía completa
                arbol.mostrarGenealogiaCompleta();
                break;
                
            case 8:
                // Mostrar árbol familiar
                arbol.mostrarArbolFamiliar();
                break;
                
            case 9: {
                // Eliminar miembro
                int id;
                cout << "\nID a eliminar: ";
                cin >> id;
                arbol.eliminarMiembro(id);
                break;
            }
            
            case 10:
                // Cargar datos de ejemplo
                arbol.cargarDatosEjemplo();
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