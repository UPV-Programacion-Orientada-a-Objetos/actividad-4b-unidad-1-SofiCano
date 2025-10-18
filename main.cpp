#include <iostream>
#include "Simulador2D.h"

int main() {
    std::cout << "Simulador Generico de Fluidos (Difusion) \n";

    //seccion de prueba float
    std::cout << "Inicializando Sistema (Tipo FLOAT) \n";
    Simulador2D<float> simFloat(5, 5); 

    std::cout << "Agregando Fuentes de Concentracion \n";
    simFloat.agregarFuente(100.0f); 
    simFloat.agregarFuente(50.0f);

    std::cout << "Aplicando Valores Iniciales \n";
    simFloat.setValor(2, 2, 100.0f); 
    simFloat.setValor(4, 0, 50.0f);

    std::cout << "Grid Inicial (Paso 0) \n";
    simFloat.imprimirGrid();

    std::cout << "Opicion: Simular 1 Paso\n";
    simFloat.simularPaso();

    std::cout << "Grid Despues del Paso 1 \n";
    simFloat.imprimirGrid();

    std::cout << "Opicion: Redimensionar\n";
    simFloat.redimensionarGrid(6, 6); 
    std::cout << "Grid Despues de Redimensionar \n";
    simFloat.imprimirGrid();
    // Seccion de prueba int
    std::cout << "Inicializando Sistema (Tipo INT) \n";
    Simulador2D<int> simInt(4, 4);

    simInt.agregarFuente(1000);
    simInt.setValor(1, 1, 1000);

    std::cout << "Grid INT (Paso 0) \n";
    simInt.imprimirGrid();

    std::cout << "Opicion: Simular 1 Paso (INT)\n";
    simInt.simularPaso();

    std::cout << "Grid INT (Paso 1) \n";
    simInt.imprimirGrid();

    std::cout << "Opicion: Salir \n";
    return 0;
}