#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Dado {
public:
    int resultados[4];
    
    Dado() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < 4; i++) {
            resultados[i] = rand() % 6 + 1;
        }
    }

    void mostrar() {
        for (int i : resultados) {
            cout << i << " ";
        }
    }
};
