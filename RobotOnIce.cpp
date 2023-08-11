#include <iostream>
#include <vector>
#include <utility>
#include <functional>
using namespace std;

using mat =vector<vector<bool>>;
using pos = pair<int,int>;

// A continuación, las arrays detallan los ocho movimientos posibles de una celda
// (arriba, derecha, abajo, izquierda)
int movFila[] = {-1, 0, 1, 0};
int movCol[] = { 0, 1, 0, -1 };
pos posFinal = make_pair(0,1);

bool unicaIsla(pos &posActual,mat &hielo){
    int x = posActual.first;
    int y = posActual.second;
    int m = hielo.size();
    int n = hielo[0].size();
    static int i, j;
    vector<int> v(9,0);
    v[0] = x-1 < 0 || y-1 < 0 || hielo[x-1][y-1];
    v[1] = x-1 < 0 || hielo[x-1][y];
    v[2] = x-1 < 0 || y+1 >= n || hielo[x-1][y+1];
    v[3] = y+1 >= n || hielo[x][y+1];
    v[4] = x+1 >= m || y+1 >= n || hielo[x+1][y+1];
    v[5] = x+1 >= m || hielo[x+1][y];
    v[6] = x+1 >= m || y-1 < 0 || hielo[x+1][y-1];
    v[7] = y-1 < 0 || hielo[x][y-1];
    v[8] = v[0];
    for(i = 0,j = 0;i < 8;i++){
        if(v[i] != v[i+1]){
            j++;
        }
    }
    return j < 4;
}

bool mePuedoMover(pos &posactual,mat &hielo){
    return 0<=posactual.first && posactual.first  < hielo.size() && 0<=posactual.second && posactual.second < hielo[0].size() &&
        !hielo[posactual.first][posactual.second];
}

int distanciaManhattan(pos &pos1, pos &pos2){
    return abs(pos1.first-pos2.first) + abs(pos1.second-pos2.second);
}


int cantidadCasos = 0;

bool cumpleParadas(pos &pparada,pos &sparada,pos &tparada, pos &posActual, int &M, int &N, int pasos){
    if (pasos != (M * N ) && posActual == posFinal) {
        return false;
    }
        //Evaluo si llegue correctamente a las paradas o no
    else if(((pasos == M * N / 4 ) && posActual != pparada) || ((pasos < (M * N)/ 4 )&& posActual == pparada)){
        return false;
    }
    else if (((pasos == M * N / 2 ) && posActual != sparada) || ((pasos < (M * N)/ 2 )&& posActual == sparada )){
        return false;
    }
    else if(((pasos == M * N * 3 / 4 ) && posActual != tparada) || (pasos < (M * N) * (3 / 4) )&& posActual == tparada){
        return false;
    }
        //Evaluo si puedo llegar a las paradas con la cantidad de pasos necesarias
    else if (pasos < (M * N / 4) && distanciaManhattan(posActual,pparada) > M * N / 4 - pasos ) {
        return false;
    }
    else if (pasos < (M * N / 2) && distanciaManhattan(posActual,sparada) > M * N / 2 - pasos) {
        return false;
    }
    else if (pasos < (M * N * (3/4)) &&  distanciaManhattan(posActual,tparada) > M * N * (3/4) - pasos) {
        return false;
    }
    else if (pasos > (M * N * (3/4)) && distanciaManhattan(posActual, posFinal) > M * N - pasos ){
        return false;
    }
    return true;
}


void backtracking(pos &pparada,pos &sparada,pos &tparada, pos &posActual,mat &hielo, int pasos){
    int M = hielo.size();
    int N = hielo[0].size();
    //Evaluo si llegue correctamente al final o no.
    if(pasos == (M * N ) && posActual == posFinal){
        cantidadCasos = cantidadCasos + 1;
    }
    else {
        for (int k = 0; k < 4; ++k){
            pos posicionAMoverme = make_pair(posActual.first + movFila[k],posActual.second + movCol[k]);
            if (mePuedoMover(posicionAMoverme, hielo)){
                hielo[posicionAMoverme.first][posicionAMoverme.second] = true;
                if (unicaIsla(posicionAMoverme,hielo) && cumpleParadas(pparada,sparada,tparada,posicionAMoverme,M,N,pasos+1)){
                    backtracking(pparada,sparada,tparada, posicionAMoverme,hielo,pasos+1);
                }
                hielo[posicionAMoverme.first][posicionAMoverme.second] = false;
            }
        }
    }
}



int main() {
    vector<int> instancia(8,0);
    cin >> instancia[0] ;
    cin >> instancia[1] ;
    vector<vector<int>> instancias;
    while(instancia[0]!=0 && instancia[1] != 0){
        cin >> instancia[2] ;
        cin >> instancia[3] ;
        cin >> instancia[4] ;
        cin >> instancia[5] ;
        cin >> instancia[6] ;
        cin >> instancia[7] ;
        instancias.push_back(instancia);
        cin >> instancia[0] ;
        cin >> instancia[1] ;
    }

    pair<int,int> inicio = make_pair(0,0);
    for(int j=0;j<instancias.size();j++){
        auto bordes = make_pair(instancias[j][0],instancias[j][1]);
        auto pparada = make_pair(instancias[j][2],instancias[j][3]);
        auto sparada = make_pair(instancias[j][4],instancias[j][5]);
        auto tparada = make_pair(instancias[j][6],instancias[j][7]);
        mat hielo(bordes.first,vector<bool>(bordes.second,false));
        hielo[0][0]=true;
        cantidadCasos = 0;
        backtracking(pparada,sparada,tparada ,inicio,hielo,1);
        cout << "Case "<< j+1<< ": " << cantidadCasos << endl;
    }
    return 0;
}
