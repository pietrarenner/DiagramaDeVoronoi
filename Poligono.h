//
//  Poligono.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Poligono_hpp
#define Poligono_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include <vector>

class Poligono
{
    vector <Ponto> Vertices;
    Ponto Min, Max;
    //Poligono Vizinhos[1000];
    int *vizinhos;
    //String corDoPoligono;
public:
    Poligono();
    Ponto getVertice(int);
    unsigned long getNVertices();
    void insereVertice(Ponto);
    void insereVertice(Ponto p, int pos);
    void desenhaPoligono();
    void desenhaVertices();
    void pintaPoligono();
    void imprime();
    void atualizaLimites();
    void obtemLimites(Ponto &Min, Ponto &Max);
    void LePoligono(const char *nome);
    void desenhaAresta(int n);
    void getAresta(int i, Ponto &P1, Ponto &P2);
    Ponto getMaximo();
    Ponto getMinimo();
    void obtemLimites2();
    void insereVizinho(int pos, int poligono);
    void inicializaVizinhos();
    int getVizinho(int i);
    //void mudaCorPoligono(int cor);
    //String getCor();
    void imprimeVizinhos();
};

#endif
