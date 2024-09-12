#include <iostream>
#include <fstream>
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()

class Jugador {
private:
    std::string nombre;
    int puntaje;

public:
    Jugador(const std::string& nombre) : nombre(nombre), puntaje(0) {}

    std::string getNombre() const {
        return nombre;
    }

    int getPuntaje() const {
        return puntaje;
    }

    void acumularPuntaje(int puntos) {
        puntaje += puntos;
    }

    void reiniciarPuntaje() {
        puntaje = 0;
    }

    int lanzarDado() {
        return rand() % 6 + 1;  // Devuelve un número entre 1 y 6
    }
};

class Juego {
private:
    Jugador jugador1;
    Jugador jugador2;
    Jugador* jugadorActual;

public:
    Juego(const std::string& nombre1, const std::string& nombre2)
        : jugador1(nombre1), jugador2(nombre2), jugadorActual(&jugador1) {
        srand(static_cast<unsigned int>(time(0)));  // Inicializa la semilla para números aleatorios
    }

    void iniciarJuego() {
        while (jugador1.getPuntaje() < 30 && jugador2.getPuntaje() < 30) {
            std::cout << "Turno de " << jugadorActual->getNombre() << std::endl;
            int dado = jugadorActual->lanzarDado();
            std::cout << "Lanzó un " << dado << std::endl;

            if (dado == 6) {
                std::cout << "Sacas un 6. Tu puntaje se reinicia." << std::endl;
                jugadorActual->reiniciarPuntaje();
                cambiarTurno();
            } else {
                jugadorActual->acumularPuntaje(dado);
                std::cout << "Puntaje actual: " << jugadorActual->getPuntaje() << std::endl;

                char decision;
                std::cout << "¿Quieres seguir lanzando? (s/n): ";
                std::cin >> decision;

                if (decision != 's') {
                    cambiarTurno();
                }
            }
        }

        // Determinar ganador
        if (jugador1.getPuntaje() >= 30) {
            std::cout << jugador1.getNombre() << " gana con " << jugador1.getPuntaje() << " puntos!" << std::endl;
        } else {
            std::cout << jugador2.getNombre() << " gana con " << jugador2.getPuntaje() << " puntos!" << std::endl;
        }
    }

    void cambiarTurno() {
        if (jugadorActual == &jugador1) {
            jugadorActual = &jugador2;
        } else {
            jugadorActual = &jugador1;
        }
    }

    void guardarEstado() {
        std::ofstream archivo("estado_juego.txt");

        if (!archivo) {
            std::cerr << "Error al abrir el archivo para guardar el estado." << std::endl;
            return;
        }

        archivo << jugador1.getNombre() << '\n';
        archivo << jugador1.getPuntaje() << '\n';
        archivo << jugador2.getNombre() << '\n';
        archivo << jugador2.getPuntaje() << '\n';
        archivo << (jugadorActual == &jugador1 ? "1" : "2") << '\n';

        archivo.close();
    }

    void cargarEstado() {
        std::ifstream archivo("estado_juego.txt");

        if (!archivo) {
            std::cerr << "Error al abrir el archivo para cargar el estado." << std::endl;
            return;
        }

        std::string nombre1, nombre2;
        int puntaje1, puntaje2;
        std::string turno;

        std::getline(archivo, nombre1);
        archivo >> puntaje1;
        archivo.ignore(); // Ignorar el salto de línea
        std::getline(archivo, nombre2);
        archivo >> puntaje2;
        archivo.ignore(); // Ignorar el salto de línea
        std::getline(archivo, turno);

        jugador1 = Jugador(nombre1);
        jugador1.acumularPuntaje(puntaje1);
        jugador2 = Jugador(nombre2);
        jugador2.acumularPuntaje(puntaje2);

        jugadorActual = (turno == "1") ? &jugador1 : &jugador2;

        archivo.close();
    }
};

int main() {
    std::string nombre1, nombre2;

    std::cout << "Nombre del Jugador 1: ";
    std::cin >> nombre1;
    std::cout << "Nombre del Jugador 2: ";
    std::cin >> nombre2;

    Juego juego(nombre1, nombre2);

    char opcion;
    std::cout << "¿Deseas cargar un estado guardado? (s/n): ";
    std::cin >> opcion;

    if (opcion == 's') {
        juego.cargarEstado();
    }

    juego.iniciarJuego();

    std::cout << "¿Deseas guardar el estado? (s/n): ";
    std::cin >> opcion;

    if (opcion == 's') {
        juego.guardarEstado();
    }

    return 0;
}
