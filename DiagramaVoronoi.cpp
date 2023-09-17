//
//  DiagramaVoronoi.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 23/08/23.
//  Copyright © 2023 Márcio Sarroglia Pinho. All rights reserved.
//

#include "DiagramaVoronoi.h"

ifstream input;            // ofstream arq;

Voronoi::Voronoi()
{

}
Poligono Voronoi::LeUmPoligono()
{
    Poligono P;
    unsigned int qtdVertices;
    input >> qtdVertices;  // arq << qtdVertices
    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le um ponto
        input >> x >> y;
        Ponto(x, y).imprime();
        if(!input)
        {
            cout << "Fim inesperado da linha." << endl;
            break;
        }
        P.insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;
    return P;
}

void Voronoi::LePoligonos(const char *nome)
{
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    string S;

    input >> qtdDePoligonos;
    cout << "qtdDePoligonos:" << qtdDePoligonos << endl;
    Ponto A, B;
    Diagrama = new Poligono[qtdDePoligonos];
    Diagrama[0] = LeUmPoligono();
    Diagrama[0].obtemLimites(Min, Max);// obtem o envelope do poligono
    for (int i=1; i< qtdDePoligonos; i++)
    {
        Diagrama[i] = LeUmPoligono();
        Diagrama[i].obtemLimites(A, B); // obtem o envelope do poligono

        Min = ObtemMinimo (A, Min);
        Max = ObtemMaximo (B, Max);

        //printf("Diagrama[%d]: %d", i, Diagrama[i]);
    }
    cout << "Lista de Poligonos lida com sucesso!" << endl;
    input.close();
}

Poligono Voronoi::getPoligono(int i)
{
    if (i >= qtdDePoligonos)
    {
        cout << "Nro de Poligono Inexistente" << endl;
        return Diagrama[0];
    }
    return Diagrama[i];
}
unsigned int Voronoi::getNPoligonos()
{
    return qtdDePoligonos;
}
void Voronoi::obtemLimites(Ponto &min, Ponto &max)
{
    min = this->Min;
    max = this->Max;
}

void Voronoi::obtemVizinhosDasArestas()
{
    for(int i = 0; i < getNPoligonos(); i++) {
        for(int j = 0; j < Diagrama[i].getNVertices(); j++){

        }
    }
}

Poligono* Voronoi::getDiagrama()
{
    return Diagrama;
}

//ta errado, mudar!!!
//teste feito somente com pontos que cruzarem a linha horizontal usada para teste
//contar número de intersecções: par = fora; ímpar = dentro
bool Voronoi::poligonosConcavos(Ponto p, Poligono pol) { //p = ponto que definimos em ExibeVARIOSPoligonos
    int nVertices = pol.getNVertices();
    int contadorArestas = 0;

    //considerar vértices que estão um ao lado do outro (formam arestas)
    //considerar vértices com x < ponto.x -> depende
    //se os vértices consecutivos tiverem o x < ponto.x, devem ser considerados
    //pensar em caso para último com o primeiro

    for(int i = 0; i < nVertices; i++)
    {
        float x1 = pol.getVertice(i).x;
        float y1 = pol.getVertice(i).y; //pega o vértice atual
        float x2 = pol.getVertice((i + 1) % nVertices).x; //pega sempre o próximo vértice
        float y2 = pol.getVertice((i + 1) % nVertices).y; //quando i for o vértice final, pega o primeiro vértice

        if(((x1 < p.x) || (x2 < p.x)) && ((y1 < p.y && y2 >= p.y) || (y2 < p.y && y1 >= p.y)))
            //saber se ponto está do lado direito das arestas calculando x
            //saber se as arestas estão no mesmo eixo y do ponto
            //triângulo e polígono do lado tem o mesmo número nesse cálculo
            //o que está faltando?
        {
            contadorArestas++;
        }
    }

    printf("arestas: %d\n", contadorArestas);

    if(contadorArestas%2 == 0) return false; //não tá dentro -> é par
    else return true; //tá dentro -> é ímpar
}

