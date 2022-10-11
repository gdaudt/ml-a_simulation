#include <iostream>
#include <math.h>
#include <vector>

int main() {
    srand (time (NULL));
    // corredor inferior
    int x_inferior = rand() % 270 + 12;
    // corredor à direita
    int y_direita = rand() % 334 + 10;
    // corredor superior
    int x_superior = rand() % 267 + 20;
    std::cout << x_inferior << " - " << y_direita << " - " << x_superior << "\n";

    std::vector<std::vector<int>> posicoes = {{x_inferior, 312}, {267, y_direita}, {x_superior, 9}};
    int i = 0;

    for (i = 0; i < posicoes.size(); i++) {
        posicoes[i][0] = (posicoes[i][0] - 105) * 4 / 2;
        posicoes[i][1] = (413 - posicoes[i][1] -(413-325)) * 4 / 2;
        //std::cout << "\n(" << posicoes[i][0] << ", " << posicoes[i][1] << ")\n";
    }
        
    const double theta = 29 * M_PI / 180;
    std::vector<std::vector<double>> rotacionado;

    for (i = 0; i < posicoes.size(); i++) {
        rotacionado.push_back ({ ((posicoes[i][0] * cos (theta) - posicoes[i][1] * sin (theta)) / 5),
        ((posicoes[i][0] * sin (theta) + posicoes[i][1] * cos (theta)) / 5) });
        std::cout << "\n(" << rotacionado[i][0] << ", " << rotacionado[i][1] << ")\n";
    }
}