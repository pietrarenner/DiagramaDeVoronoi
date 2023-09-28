//
//  DiagramaVoronoi.cpp
//  OpenGLTest
//
//  Created by M�rcio Sarroglia Pinho on 23/08/23.
//  Copyright � 2023 M�rcio Sarroglia Pinho. All rights reserved.
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

//retorna c�pia do pol�gono
//adicionar & antes de Poligono
//Poligono& poligono = getPoligono(indice)
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
    Ponto p, p1, p2, p3;

    // Inicializar arrays de vizinhos
    for (int i = 0; i < getNPoligonos(); i++)
        Diagrama[i].inicializaVizinhos(); // Inicializa o vetor de vizinhos com a quantidade de v�rtices

    // Criar dentro do pol�gono lista com os vizinhos
    // Considerar nessa lista o v�rtice no qual a aresta come�a
    // Procurar por arestas que tenham o v�rtice atual e o anterior iguais ao atual e o pr�ximo do pol�gono
    for (int i = 0; i < getNPoligonos(); i++) {
        for (int j = 0; j < Diagrama[i].getNVertices(); j++) { // Pega todos os v�rtices de um determinado pol�gono
            // Pegar v�rtice[j] e v�rtice[(j + 1) % getNVertices()]
            p = Diagrama[i].getVertice(j);
            p1 = Diagrama[i].getVertice((j + 1) % Diagrama[i].getNVertices());

            // Passar por todos os v�rtices do pr�ximo pol�gono a ser comparado com o v�rtice atual do pol�gono atual
            for (int k = 0; k < getNPoligonos(); k++) {
                if (k == i) // N�o comparar o pol�gono com ele mesmo
                    continue;

                for (int l = 0; l < Diagrama[k].getNVertices(); l++) {
                    // Pegar v�rtice[l] e v�rtice[(l + 1) % getNVertices()]
                    p2 = Diagrama[k].getVertice(l);
                    p3 = Diagrama[k].getVertice((l + 1) % Diagrama[k].getNVertices());

                    // Comparar v�rtice[j] do pol�gono atual com v�rtice[(l + 1) % getNVertices()] do pr�ximo pol�gono
                    // Comparar v�rtice[(j + 1) % getNVertices()] do pol�gono atual com v�rtice[l] do pr�ximo pol�gono
                    if ((p == p3) && (p1 == p2)) {
                        Diagrama[i].insereVizinho(j, k);
                        //printf("Vizinho do poligono %d: %d\n", i, k);
                        //printf("entrou na aresta %d\n", j);
                        break;
                    }
                }
            }
        }
        //printf("\n");
    }
}

Poligono* Voronoi::getDiagrama()
{
    return Diagrama;
}

bool Voronoi::poligonosConcavos(Ponto ponto, Ponto Esq, Poligono pol) {
    int nVertices = pol.getNVertices();
    int contadorArestas = 0;
    Ponto p1, p2;

    for(int i = 0; i < nVertices; i++)
    {
        float x1 = pol.getVertice(i).x;
        float y1 = pol.getVertice(i).y; //pega o v�rtice atual
        float x2 = pol.getVertice((i + 1) % nVertices).x; //pega sempre o pr�ximo v�rtice
        float y2 = pol.getVertice((i + 1) % nVertices).y; //quando i for o v�rtice final, pega o primeiro v�rtice

        p1 = Ponto(x1,y1);
        p2 = Ponto(x2,y2);

        if(HaInterseccao(ponto, Esq, p1, p2))
        {
            contadorArestas++;
        }
    }

    printf("arestas: %d\n", contadorArestas);

    if(contadorArestas%2 == 0) return false; //n�o t� dentro -> � par
    else return true; //t� dentro -> � �mpar
}

bool Voronoi::poligonosConvexos(Poligono pol, Ponto p){
    int contadorArestas = 0;
    float x1, x2, x3, x4, y1, y2, y3, y4;
    Ponto V1, V2, prodVetorialVP;
    int n = pol.getNVertices();

    for(int i = 0; i < n; i++)
    {

        x1 = pol.getVertice(i).x;
        y1 = pol.getVertice(i).y; //pega o v�rtice atual
        x2 = pol.getVertice((i + 1) % n).x; //pega sempre o pr�ximo v�rtice
        y2 = pol.getVertice((i + 1) % n).y;
        //Ponto P1 = Ponto(x1,y1);
        //Ponto P2 = Ponto (x2,y2);

        //calculando os vetores
        x3 = x2 - x1;
        y3 = y2 - y1;
        //V1 = P2-P1;
        V1 = Ponto(x3,y3); //primeiro vetor

        x4 = p.x - x1;
        y4 = p.y - y1;
        V2 = Ponto(x4,y4); //segundo vetor

        ProdVetorial(V1,V2,prodVetorialVP); //como chamar a fun��o ProdVetorial???

        if(prodVetorialVP.z < 0) contadorArestas++; //considera todos os pontos que estiverem de um mesmo lado
        else if(prodVetorialVP.z > 0) return false;
    }

    if(contadorArestas==n) return true;
    return false;
}

void Voronoi::criaEnvelopes()
{
    envelopes = new Envelope[qtdDePoligonos];
}

void Voronoi::setEnvelopes(int i, Ponto p1, Ponto p2)
{
    envelopes[i].GeraEnvelope(p1, p2);
}

Envelope Voronoi::getEnvelope(int i)
{
    return envelopes[i];
}
