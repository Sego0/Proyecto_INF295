#include "stroke.h"
#include "stroke.h"
#include <iostream>
#include <random>


// RNG global
static std::mt19937 rng(std::random_device{}());
int randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int main() {
    // 1) Cargar brushes desde carpeta
    {
        ImageGray b0, b1, b2, b3;
        if (!loadImageGray("brushes/1.jpg", b0)) return 1;
        if (!loadImageGray("brushes/2.jpg", b1)) return 1;
        if (!loadImageGray("brushes/3.jpg", b2)) return 1;
        if (!loadImageGray("brushes/4.jpg", b3)) return 1;
        gBrushes.push_back(std::move(b0));
        gBrushes.push_back(std::move(b1));
        gBrushes.push_back(std::move(b2));
        gBrushes.push_back(std::move(b3));
    }
    // 2) Canvas, cambiar tamaño de acuerdo a la instancia
    Canvas P(48, 64);
    loadImageRGB_asCanvas("instancias/bach.png", P);

    // 4) Generar las "random strokes"
    auto strokes = randomInit(P, 1000);

    // 4) Renderizar resultado
    Canvas result(P.width, P.height);
    result.clear(255, 255, 255);
    render(strokes, result);

    if (!savePNG(result, "output1.png")) {
        std::cerr << "Error guardando output.png\n";
        return 1;
    }
    std::cout << "OK: guardado output1.png\n";
    return 0;
}
