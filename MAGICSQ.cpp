#include <iostream>
#include "vector"
#include <ctime>

using namespace std;

typedef vector<vector<int>> matrix;

struct sumas {
    vector<int> &filas;
    vector<int> &columnas;
    int diagonalAscendente;
    int diagonalDescendente;
    int esquinas;
    int centros;
    int cruces;
};

void printMatrix(std::vector<std::vector<int>> &matrix) {
    for (const std::vector<int>& row : matrix) {
        for (int elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

//Sumatoria
int sumatoria1aN (const int &n){
    return n*(n+1)/2;
}
//Segun los terminos matematicos del problema, el cuadrado magico siempre es posible si las sumas son iguales a esto
//Esto realiza una sumatoria de 1 a n² y las divide de forma uniforme. Esa suma uniforme es la que tiene que dar cada fila
//columna y diagonal

//Todas estas propiedades que estan aca las saque de la lista de propiedades de cuadrados magicos de wikipedia
int numeroMagico(const int &n){
    return sumatoria1aN(n*n)/n;
}

//Este es el numero base, el centro esta equilibrado en base a este numero.
//Si es par el orden, los 4 que estan alrededor deben dar el numero base multiplicado por 4
//Que es el mimso que el numero magico de segundo orden, si es impar el del medio siempre es el numero base
int numeroBase(const int &n){
    return numeroMagico(n)/n;
}

//Tiene props interesantes con las diagonales y las cruces
int numeroMagico2(const int &n){
    return 4*numeroMagico(n)/n;
}

int sumDiagonalDesc(matrix &m){
    int sum = 0;
    for (int i = 0; i < m.size(); i++){
        sum = sum + m[i][i];
    }
    return sum;
}


//Agregar la propiedad de numero magico

bool cuadradoMagico (matrix &m,const int &nMagico){
    int sumatoriaDiagonalDes = sumDiagonalDesc(m);
    if (sumatoriaDiagonalDes != nMagico){
        return false;
    }
    return true;
}

bool podaEnFinalDeFila(matrix &m,const int &fila,const int &col, const sumas& sumasGuardadas,const int &nMagico){
    return col == m.size()-1 && sumasGuardadas.filas[fila] != nMagico;
}

bool podaEnDiagonalAsc(matrix &m,const int &col,const int &fila, const sumas& sumasGuardadas,const int &nMagico){
    return fila == m.size()-1 && col == 0 && nMagico != sumasGuardadas.diagonalAscendente;
}

bool podasConSumas(sumas &sumasGuardadas, const int &fila, const int &col, const unsigned long &n,const int &nMagico){
    if(sumasGuardadas.filas[fila] > nMagico){
        return true;
    }
    if(sumasGuardadas.columnas[col] > nMagico){
        return true;
    }
    if (fila == col && sumasGuardadas.diagonalDescendente > nMagico){
        return true;
    }
    if (col == n-1-fila && sumasGuardadas.diagonalAscendente  > nMagico){
        return true ;
    }
    return false;
}


bool
mePase(matrix &m, const int &caso, sumas &sumasGuardadas, const int &nMagico2, const int &dobleNMagico2) {
    int n = m.size();
    if (caso == 0) {//Caso de la poda por centro par
        if (m[(n / 2)][(n / 2)] != 0){
            return sumasGuardadas.centros >
                   nMagico2;
        } else {
            return sumasGuardadas.centros >=
                   nMagico2;
        }
    } else if (caso == 1) { //Caso de las esquinas
        if (m[n-1][n-1] != 0){
            return sumasGuardadas.esquinas > nMagico2;
        } else {
            return sumasGuardadas.esquinas >= nMagico2;
        }
    } else if (caso == 2) { //caso cruz de numero impar
        if (m[n-1][n/2] != 0){
            return sumasGuardadas.cruces >
                   nMagico2;
        } else {
            return sumasGuardadas.cruces >=
                   nMagico2;
        }
    } else if (caso == 3) {
        if (m[n-1][n/2] != 0) {
            return sumasGuardadas.cruces > dobleNMagico2;
        } else {
            return sumasGuardadas.cruces >= dobleNMagico2;
        }
    }
    return false;
}
bool noLlego(matrix &m, vector<bool> &numerosUsados ,const int &f, const int &c, const int &caso,sumas &sumasGuardadas,const int &nMagico2, const int &dobleNMagico2){
    int n = m.size();
    if (caso == 0){ //Caso de la poda por centro par
        if ( f == n/2 && c == n/2-1){
            //Verifico que la diferencia del numero magico 2 y las sumas de los centros la puedo poner eventualmente
            //O si ya no está disponible
            int dif = nMagico2 - sumasGuardadas.centros;
            return numerosUsados[dif];
        }
    } else if (caso == 1){
        if (f==n-1 && c == 0){
            //Verifico que la diferencia del numero magico 2 y las sumas de los centros la puedo poner eventualmente
            //O si ya no está disponible
            int dif = nMagico2 - sumasGuardadas.esquinas;
            return numerosUsados[dif];
        }
    } else if (caso == 2){
        if(f==n-1 && c == n/2){
            int dif = nMagico2 - sumasGuardadas.cruces;
            return numerosUsados[dif];
        }
    } else if (caso == 3){ //En los casos pares es 2*SegundoNumeroMagico
        if(f==n-1 && c == n/2-1){
            int dif = dobleNMagico2 - sumasGuardadas.cruces;
            return numerosUsados[dif];
        }
    }
    return false;
}
/*
bool podaPorCentro(matrix &m,vector<bool> &numerosUsados,const int &fila,const int &col,sumas &sumasGuardadas,const int &nMagico2,const int &nBase, const int &dobleNMagico2){
    if (m.size() % 2 == 1){
        if ( fila == m.size()/2 && col == m.size()/2 && m[fila][col] != nBase){
            return true;
        }
    } else {
        if (mePase(m, 0, sumasGuardadas, nMagico2, dobleNMagico2) || (fila == m.size() / 2 && col == m.size() / 2 - 1 && noLlego(m, numerosUsados, fila, col, 0, sumasGuardadas, nMagico2, dobleNMagico2))){
            return true;
        }
    }
    return false;
}

bool podaPorEsquinas(matrix &m, vector<bool> &numerosUsados, const int &fila, const int &col, sumas &sumasGuardadas,
                     const int &nMagico2, const int &dobleNMagico2) {
    return mePase(m, 1, sumasGuardadas, nMagico2, dobleNMagico2) || (fila == m.size() - 1 && col == 0 && noLlego(m, numerosUsados, fila, col, 1, sumasGuardadas, nMagico2, dobleNMagico2));
}

bool podaPorCruz(matrix &m, vector<bool> &numerosUsados, const int &fila, const int &col, sumas &sumasGuardadas,const int &nMagico2, const int &dobleNMagico2) {
    if (m.size() % 2 == 1){
        if (mePase(m, 2, sumasGuardadas, nMagico2, dobleNMagico2) || (fila == m.size() - 1 && col == m.size() / 2 && noLlego(m, numerosUsados, fila, col, 2, sumasGuardadas, nMagico2, dobleNMagico2))){
            return true;
        }
    } else {
        if (mePase(m, 3, sumasGuardadas, nMagico2, dobleNMagico2) || (fila == m.size() - 1 && col == m.size() / 2 - 1 && noLlego(m, numerosUsados, fila, col, 3, sumasGuardadas, nMagico2, dobleNMagico2))){
            return true;
        }
    }
    return false;
}

bool posCentro(matrix &m,const int &f, const int &c){
    if (m.size() % 2 == 1){
        return f == m.size()/2 && c == m.size()/2;
    } else {
        return (f == m.size()/2 && c == m.size()/2) || (f == m.size()/2-1 && c == m.size()/2-1)
               || (f == m.size()/2-1 && c == m.size()/2) || (f == m.size()/2 && c == m.size()/2-1);
    }
}

bool posEsquina(matrix &m,const int &f, const int &c){
    return (f == 0 && c == 0 ) || (f == m.size() - 1 && c == 0 ) || (f == 0 && c == m.size() - 1) ||
           (f == m.size()-1 && c == m.size() -1 );
}

bool posCruz(matrix &m,const int &f, const int &c){
    if (m.size() % 2 == 1){
        return (f == 0 && c == m.size()/2) || (f == m.size()/2 && c == 0) || (f == m.size()/2 && c == m.size()-1)
               || (f == m.size()-1 && c == m.size()/2) ;
    } else {
        return (f == 0 && c == m.size()/2) || (f == m.size()/2 && c == 0) || (f == m.size()/2 && c == m.size()-1)
               || (f == m.size()-1 && c == m.size()/2) ||
               (f == 0 && c == m.size()/2-1) || (f == m.size()/2-1 && c == 0) || (f == m.size()/2-1 && c == m.size()-1)
               || (f == m.size()-1 && c == m.size()/2-1);
    }
}

bool posicionDeInteres(matrix &m,const int &f, const int &c){
    return posCentro(m,f,c) || posCentro(m,f,c) || posCruz(m,f,c);
}
*/
bool podasGeneral(matrix &m,vector<bool> &numerosUsados,const int &fila,const int &col, sumas &sumasGuardadas,const int &nMagico, const int &nMagico2, const int &nBase,const int &dobleNMagico2,const int &num) {
    //Podas con numero mágico
    if (podaEnFinalDeFila(m, fila, col, sumasGuardadas,nMagico)) {
        return true;
    }
    if (podaEnDiagonalAsc(m, col, fila, sumasGuardadas,nMagico)) {
        return true;
    }
    if (podasConSumas(sumasGuardadas, fila, col, m.size(),nMagico)) {
        return true;
    }/*
    //Podas con numero base o numero magico de segundo orden
    if (num == nBase && !posCentro(m,fila,col) && m.size() % 2 == 1){ //Si el nbase no lo pondre en el centro podo solo para impares
        return true;
    }*/
    /*
    if (posicionDeInteres(m, fila, col)) {
        if (posCentro(m, fila, col)) {
            if (podaPorCentro(m, numerosUsados, fila, col, sumasGuardadas,nMagico2,nBase,dobleNMagico2)) {
                return true;
            }
        } else if (posEsquina(m, fila, col)) {
            if (podaPorEsquinas(m, numerosUsados, fila, col, sumasGuardadas, nMagico2, dobleNMagico2)) {
                return true;
            }
        } else if (posCruz(m, fila, col)) {
            if (podaPorCruz(m, numerosUsados, fila, col, sumasGuardadas, nMagico2, dobleNMagico2)) {
                return true;
            }
        }
    }*/
    return false;
}


void cambioDeEstado(matrix &m, const int &fila,const int &col,const int &num,vector<bool> &numerosUsados, sumas &sumasGuardadas){
    m[fila][col] = num;
    numerosUsados[num] = true;
    sumasGuardadas.columnas[col] = sumasGuardadas.columnas[col] + num;
    sumasGuardadas.filas[fila] = sumasGuardadas.filas[fila] + num;/*
    if (posCentro(m,fila,col)){
        sumasGuardadas.centros=sumasGuardadas.centros+num;
    }
    if(posEsquina(m,fila,col)){
        sumasGuardadas.esquinas = sumasGuardadas.esquinas+num;
    }
    if(posCruz(m,fila,col)){
        sumasGuardadas.cruces = sumasGuardadas.cruces+num;
    }*/
    if (fila == col) {
        sumasGuardadas.diagonalDescendente = sumasGuardadas.diagonalDescendente + num;
    }
    if (col == m.size() - 1 - fila) {
        sumasGuardadas.diagonalAscendente = sumasGuardadas.diagonalAscendente + num;
    }
}

void vueltaAlOriginal(matrix &m, const int &fila,const int &col,const int &num,vector<bool> &numerosUsados, sumas &sumasGuardadas){
    if (fila == col) {
        sumasGuardadas.diagonalDescendente = sumasGuardadas.diagonalDescendente - num;
    }
    if (col == m.size() - 1 - fila) {
        sumasGuardadas.diagonalAscendente = sumasGuardadas.diagonalAscendente - num;
    }/*
    if (posCentro(m,fila,col)){
        sumasGuardadas.centros=sumasGuardadas.centros-num;
    }
    if(posEsquina(m,fila,col)){
        sumasGuardadas.esquinas = sumasGuardadas.esquinas-num;
    }
    if(posCruz(m,fila,col)){
        sumasGuardadas.cruces = sumasGuardadas.cruces-num;
    }*/
    sumasGuardadas.filas[fila] = sumasGuardadas.filas[fila] - num;
    sumasGuardadas.columnas[col] = sumasGuardadas.columnas[col] - num;
    numerosUsados[num] = false;
    m[fila][col] = 0;
}
double t1 = clock();
bool encontrado = false;
void backtracking(matrix &m, const int &fila,const int &col, vector<bool> &numerosUsados, sumas &sumasGuardadas,const int &k, int &contador,const int &nMagico, const int &nMagico2, const int &nBase,const int &dobleNMagico2){
    if (fila == m.size() && col == 0 && cuadradoMagico(m,nMagico)){ //Termino la recursion, la fila n representa que llene la matriz
        contador = contador + 1;
        if (contador == k){
            encontrado = true;
            return;
        }
    }
    else {
        //definicion del vector de booleanos para no repetir por filas ni columnas el numero
        for (int num = 1; num <= m.size()*m.size();num++ ) {
            if (!numerosUsados[num]) {
                cambioDeEstado(m,fila,col,num,numerosUsados,sumasGuardadas);
                if (!podasGeneral(m,numerosUsados,fila,col,sumasGuardadas,nMagico,nMagico2,nBase,dobleNMagico2,num))
                {
                    if (col == m.size() - 1) {
                        backtracking(m, fila + 1, 0, numerosUsados, sumasGuardadas,k,contador,nMagico,nMagico2,nBase,dobleNMagico2);
                    } else {
                        backtracking(m, fila, col + 1, numerosUsados, sumasGuardadas,k,contador,nMagico,nMagico2,nBase,dobleNMagico2);
                    }
                    if (encontrado){
                        return;
                    }
                }
                vueltaAlOriginal(m,fila,col,num,numerosUsados,sumasGuardadas);
            }
        }
    }
}


//Esto debería a partir de los parametros n y k llamar a la funcion de backtracking

void funcion(int n, int k){
    vector<vector<int>> m (n,vector<int> (n,0));
    vector<int> sumafilas (n,0);
    vector<int> sumacolumnas (n,0);
    sumas sumasGuardadas {sumafilas,sumacolumnas,0,0};
    int contador = 0;
    int nMagico = numeroMagico(n);
    int nMagicoSegundoOrden = numeroMagico2(n);
    int nBase = numeroBase(n);
    int dobleDelNMagicoDeSegundoOrden = 2* nMagicoSegundoOrden;
    vector<bool> numerosUsados (n*n + 1, false);
    if (k == 0){
        cout << "-1" << endl;
    } else {
        backtracking(m,0,0,numerosUsados,sumasGuardadas,k,contador,nMagico,nMagicoSegundoOrden,nBase,dobleDelNMagicoDeSegundoOrden);
        if (contador < k ){
            cout << "-1" << endl;
        } else {
            printMatrix(m);
        }
    }
}

double t0 = ::clock();

int main() {
    int n;
    int k;
    cin >> n >> k;
    funcion(n, k);
    return 0;
}
