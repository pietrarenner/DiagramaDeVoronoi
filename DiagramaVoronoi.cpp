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
                        //Diagrama[k].insereVizinho(l, i);
                        printf("Vizinho do poligono %d: %d\n", i, k);
                        //printf("vizinho do poligono %d: %d\n", k, i);
                        break;
                    }
                }
            }
        }
    }
}

Poligono* Voronoi::getDiagrama()
{
    return Diagrama;
}

//ta errado, mudar!!!
//teste feito somente com pontos que cruzarem a linha horizontal usada para teste
//contar n�mero de intersec��es: par = fora; �mpar = dentro
//p = ponto que definimos em ExibeVARIOSPoligonos
bool Voronoi::poligonosConcavos(Ponto p, Poligono pol) { //HaInterseccao
    int nVertices = pol.getNVertices();
    int contadorArestas = 0;

    //considerar v�rtices que est�o um ao lado do outro (formam arestas)
    //considerar v�rtices com x < ponto.x -> depende
    //se os v�rtices consecutivos tiverem o x < ponto.x, devem ser considerados
    //pensar em caso para �ltimo com o primeiro

    for(int i = 0; i < nVertices; i++)
    {
        float x1 = pol.getVertice(i).x;
        float y1 = pol.getVertice(i).y; //pega o v�rtice atual
        float x2 = pol.getVertice((i + 1) % nVertices).x; //pega sempre o pr�ximo v�rtice
        float y2 = pol.getVertice((i + 1) % nVertices).y; //quando i for o v�rtice final, pega o primeiro v�rtice

        if(((x1 < p.x) || (x2 < p.x)) && ((y1 < p.y && y2 >= p.y) || (y2 < p.y && y1 >= p.y)))
            //saber se ponto est� do lado direito das arestas calculando x
            //saber se as arestas est�o no mesmo eixo y do ponto
            //tri�ngulo e pol�gono do lado tem o mesmo n�mero nesse c�lculo
            //o que est� faltando?
        {
            contadorArestas++;
        }
    }

    printf("arestas: %d\n", contadorArestas);

    if(contadorArestas%2 == 0) return false; //n�o t� dentro -> � par
    else return true; //t� dentro -> � �mpar
}

bool Voronoi::poligonosConvexos(){  //ProdVetorial
}

