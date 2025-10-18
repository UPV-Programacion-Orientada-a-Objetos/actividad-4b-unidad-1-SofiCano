#pragma once

#include <iostream>
#include <iomanip> 
#include <algorithm> 

template <typename T>
class Simulador2D {
private:
    //matriz de la cuadrícula
    T **_grid;
    int _filas;
    int _columnas;

    //vector de Fuentes
    T *_fuentes;
    int _numFuentes;
    int _capacidadFuentes;

    //vector de Constantes (estatico)
    float _constantes[3];

public:
    Simulador2D(int f, int c)
        : _filas(f), _columnas(c), _numFuentes(0), _capacidadFuentes(10) //empezamos con capacidad de 10
    {
        //se asigna la memoria para la matriz 2D  (filas)
        _grid = new T*[_filas];
        for (int i = 0; i < _filas; ++i) {
            //se asignaa los valores de cada columna
            _grid[i] = new T[_columnas];
            //se inicializa cada celda a 0 para evitar basura  
            for (int j = 0; j < _columnas; ++j) {
                _grid[i][j] = T{};
            }
        }

        //asignar memoria inicial para el vector de fuentes
        _fuentes = new T[_capacidadFuentes];
        
        std::cout << "Creando Grid (Tipo " << typeid(T).name() << ") de " 
        << _filas << "x" << _columnas << "...\n";
    }
    //constructor de Copia
    Simulador2D(const Simulador2D<T>& other) {
        std::cout << "Invocando Constructor de Copia...\n";

        //se copian los atributos primitivos
        _filas = other._filas;
        _columnas = other._columnas;
        _numFuentes = other._numFuentes;
        _capacidadFuentes = other._capacidadFuentes;
        
        //se copia el vector de constantes
        for (int i = 0; i < 3; ++i) {
            _constantes[i] = other._constantes[i];
        }

        //se realiza una copia profunda del vector de fuentes
        _fuentes = new T[_capacidadFuentes];
        for (int i = 0; i < _numFuentes; ++i) {
            _fuentes[i] = other._fuentes[i];
        }

        //se realiza una copia profunda de la matriz 2D
        _grid = new T*[_filas];
        for (int i = 0; i < _filas; ++i) {
            //asignar nueva memoria para las columnas de esta fila
            _grid[i] = new T[_columnas];
            
            //copiar los valores de la matriz
            for (int j = 0; j < _columnas; ++j) {
                _grid[i][j] = other._grid[i][j];
            }
        }
    }
    //operador de Asignacion
    Simulador2D<T>& operator=(const Simulador2D<T>& other) {
        //se verifica autoasignacion
        if (this == &other) {
            return *this; 
        }

        //se libera la memoria existente
        for (int i = 0; i < _filas; ++i) {
            delete[] _grid[i];
        }
        delete[] _grid;
        delete[] _fuentes;

        _filas = other._filas;
        _columnas = other._columnas;
        _numFuentes = other._numFuentes;
        _capacidadFuentes = other._capacidadFuentes;
        for (int i = 0; i < 3; ++i) {
            _constantes[i] = other._constantes[i];
        }
        _fuentes = new T[_capacidadFuentes];
        for (int i = 0; i < _numFuentes; ++i) {
            _fuentes[i] = other._fuentes[i];
        }
        _grid = new T*[_filas];
        for (int i = 0; i < _filas; ++i) {
            _grid[i] = new T[_columnas];
            for (int j = 0; j < _columnas; ++j) {
                _grid[i][j] = other._grid[i][j];
            }
        }
        return *this;
    }
    ~Simulador2D() {
        //liberamos la memoria asignada para la matriz 2D
        for (int i = 0; i < _filas; ++i) {
            delete[] _grid[i];
        }
        //se libera el puntero de la matriz
        delete[] _grid;

        //liberar memoria del vector de fuentess
        delete[] _fuentes;
    }
    void agregarFuente(T valor) {
        //primero verificamos si hay espacio en el vector
        if (_numFuentes == _capacidadFuentes) {
            std::cout << "Expandiendo capacidad de fuentes a " << _capacidadFuentes * 2 << "...\n";
            int nuevaCapacidad = _capacidadFuentes * 2;
            
            //creamos un nuevo vector con mayor capacidad
            T* nuevasFuentes = new T[nuevaCapacidad];
            
            //copiamos los valores antiguos al nuevo vector
            for (int i = 0; i < _numFuentes; ++i) {
                nuevasFuentes[i] = _fuentes[i];
            }
            
            //borramos el vector antiguo
            delete[] _fuentes;

            //actualizamos el puntero y la capacidad
            _fuentes = nuevasFuentes;
            _capacidadFuentes = nuevaCapacidad;
        }

        //agregamos el nuevo valor de la fuente
        _fuentes[_numFuentes] = valor;
        _numFuentes++;
        std::cout << "Agregar Fuente (Valor " << valor << "): Exito.\n";
    }
    void redimensionarGrid(int nuevaF, int nuevaC) {
        //se crea un nuevo grid con las nuevas dimensiones
        T** nuevoGrid = new T*[nuevaF];
        for (int i = 0; i < nuevaF; ++i) {
            nuevoGrid[i] = new T[nuevaC];
            for (int j = 0; j < nuevaC; ++j) {
                nuevoGrid[i][j] = T{}; //inicializar a 0 para evitar basura
            }
        }

        //se calcula la region comun 
        int filasACopiar = std::min(_filas, nuevaF);
        int colsACopiar = std::min(_columnas, nuevaC);

        //copiamos los datos del grid antiguo al nuevo
        for (int i = 0; i < filasACopiar; ++i) {
            for (int j = 0; j < colsACopiar; ++j) {
                nuevoGrid[i][j] = _grid[i][j];
            }
        }

        //liberar la memoria del grid antiguo
        for (int i = 0; i < _filas; ++i) {
            delete[] _grid[i];
        }
        delete[] _grid;

        //actualizar los punteros y dimensiones
        _grid = nuevoGrid;
        _filas = nuevaF;
        _columnas = nuevaC;

        std::cout << "Redimensionando Grid a " << _filas << "x" << _columnas << " Datos copiados.\n";
    }
    void simularPaso() {
        std::cout << "Simulando Propagacion \n";

        //se crea una matriz temporal para almacenar los nuevos valores
        T** tempGrid = new T*[_filas];
        for (int i = 0; i < _filas; ++i) {
            tempGrid[i] = new T[_columnas];
        }

        //calculo de nuevos valores basado en reglas simples de difusion
        for (int i = 0; i < _filas; ++i) {
            for (int j = 0; j < _columnas; ++j) {
                //los bordes permanecen igual
                if (i == 0 || i == _filas - 1 || j == 0 || j == _columnas - 1) {
                    tempGrid[i][j] = _grid[i][j];
                } 
                //celdas internas se actualizan promediando los vecinos
                else {
                    tempGrid[i][j] = (_grid[i-1][j] +  //arriba
                                      _grid[i+1][j] +  //abajo
                                      _grid[i][j-1] +  //izquierda
                                      _grid[i][j+1])   //derecha
                                    / 4.0; 
                }
            }
        }

        //3-Liberar la memoria del grid antiguo
        for (int i = 0; i < _filas; ++i) {
            delete[] _grid[i];
        }
        delete[] _grid;

        //apuntamos al nuevo grid
        _grid = tempGrid;
    }
    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            _grid[f][c] = valor;
            std::cout << "Grid[" << f << "," << c << "] = " << valor << "\n";
        }
    }
    void imprimirGrid() const {
        for (int i = 0; i < _filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < _columnas; ++j) {
                //formato para que se vea alineado
                std::cout << std::fixed << std::setprecision(1) 
                << std::setw(5) << _grid[i][j] << " | ";
            }
            std::cout << "\n";
        }
    }
};