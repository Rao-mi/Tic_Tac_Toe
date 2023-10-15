#include "Tic_Tac_Toe.hpp"
#include <cmath>
#include <iostream>
#include "directions.hpp"

using std::cout;
using std::cin;
using std::endl;


//--------------------------------------FUNZIONI NON PUBBLICHE------------------------------------------



// Funzione che mi conta i tipi
int type_count (state type, std::vector<state> grid) {
    int flag=0;
    for (size_t i=0; i<grid.size(); i++) {
        if (grid[i]==type) {flag=flag+1;}
    }
    return flag;
}

// Funzione che verifica sulla direzione data tutte le celle
bool line_check (std::vector<state>& grid, int cell_number, int dimension, int line, int start,std::vector<int> v_dir, std::vector<std::vector<int>> constrain) {
    int u = line;
    int flag=1;
    state player =grid[start];
    while (player!=state::N){
         if (start+u>=static_cast<int>(grid.size()) || start+u<0){
            player=state::N;
         }
        else if (grid[start+u]==player) { 
            for (int i=0; i<static_cast<int>(constrain.size()); i++) {
            for (int j=0; j<static_cast<int>(constrain[i].size()); j++) {      
            if ((k_module(int_pow(cell_number,i+1-dimension),start+u)==constrain[i][j] && i>=dimension)||(k_module(int_pow(cell_number,i+1),start+u)==constrain[i][j] && i<dimension)) {
                    std::vector<int> check;
                    if (i>=dimension){check = canonic(cell_number-1,i);}
                    else {check = inv_canonic(cell_number-1,i);}
                    if (scalar_p(check,v_dir)>0) {
                    player=state::N;
                    j=constrain[i].size()-1;
                    i=constrain.size()-1;
                    }                
                }   
            }}        
            u+=u;
            flag +=1;
        }
        else {player=state::N;}           
    }
    if (flag==cell_number) {return true;}
    else {return false;}
}

// Funzione di supporto che fa il check della vincita
bool check_win(std::vector<state> grid, int cell_number, int dimension) {
    int tot_dim = int_pow(cell_number,dimension);
    std::vector<std::vector<int>> limit = constrains(cell_number,dimension); // Grazie a questo vettore posso definire i constrains, mediante aritmetica modulare
    std::vector<int> n_directions; // Essendo una matrice su vettore dovrò trasformare le direzioni in numeri da sommare
    state player = state::O; //Mi serve per ciclare senza riscrivere
    while (player!=state::N) {
    for (int i=0; i<tot_dim; i++) {
            std::vector<std::vector<int>> directions = k_comb_gen(dimension); // Genero le direzioni
            directions= zero_cut(directions); // Tolgo lo zero
                if (grid[i]==player) {  // con questo ciclo seleziono le direzioni accettabili confrontando con i constrains generati (es. cella 2 in un 3x3 solo (-1,0);(0,1);(-1,1))
                    for (int k=0; k<static_cast<int>(limit.size()); k++) {
                        for (size_t f=0; f<limit[k].size(); f++) {
                    if (k_module(int_pow(cell_number,k+1-dimension),i)==limit[k][f] && k>=dimension) {directions = j_a_remover(directions,k-dimension,1);}
                    else if (k_module(int_pow(cell_number,k+1),i)==limit[k][f] && k<dimension){directions = j_a_remover(directions,k,-1);}
                    }}
            n_directions = numeric_directions(cell_number,directions); // Trasformo i vettori in numeri
                       for (size_t k=0; k<n_directions.size(); k++){
                            if (grid[i+n_directions[k]]==player && line_check(grid,cell_number,dimension,n_directions[k],i,directions[k],limit)) //qui faccio check della direzione partendo dalla cella grid[i+n_directions[k]], solo se player!=N
                            {
                                return true;
                            }}


        }
    } if (player==state::O) {player=state::X;}
    else {player=state::N;}
    }
     return false;
}
//--------------------------------------CLASSE BOARD------------------------------------------

Board::Board (int set_cell_n,int set_dim)
    : cell_number(set_cell_n), dimension(set_dim), 
    grid(int_pow(set_cell_n,set_dim),state::N) {}

bool Board::Move(state player) {
    bool win;
    int tot_dim = int_pow(cell_number,dimension);
    int flag=0;
    int a=0;
    int cell=0;
    for (int i=0; i<tot_dim; i++) {
        if (grid[i]==state::N) {flag=flag+1;}
    }
    if (flag != 0) {

                a=0;
                cell=0;

            for (int i=0; i<dimension; i++) {

                cout<<"Inserire la componente x_"<<i<<" della cella che si vuole giocare"<<endl;
                cin>>a;
                cell+= a*int_pow(cell_number,i);
            }
                if (grid[cell]==state::N) {
                    grid[cell]=player;
                }
                else {cout<<"Cella già occupata"<<endl;
                cin.get();
                }
                win = check_win(grid,cell_number,dimension);
            if (win) {
        cout<<"La partita è conclusa, il vincitore è:";
        if (player==state::X) {cout<<" X"<<endl;}
        else if (player==state::O) {cout<<" O"<<endl;}
        return false;}
        else {return true;}
    }

    else { cout<<"La partita è conclusa. Patta"<<endl;
        return false;}
}
bool Board::Draw() {
    if (type_count(state::N,grid)==0) {return true;}
    else {return false;}
}
void Board::setgrid (std::vector<state> state) {
    grid = state;

}

// Funzione per printare nel caso bidimensionale
void Board::print2D() {
    for (size_t i=0; i<grid.size(); i++) {
        if (k_module(cell_number,i)==cell_number-1) {
            if (grid[i]==state::X) {cout<<" X "<<endl;}
            else if (grid[i]==state::O) {cout<<" O "<<endl;}
            else if (grid[i]==state::N) {cout<<"[ ]"<<endl;}
        }
        else {
            if (grid[i]==state::X) {cout<<" X ";}
            else if (grid[i]==state::O) {cout<<" O ";}
            else if (grid[i]==state::N) {cout<<"[ ]";}
        }
    }    
}
// Funzione ricorsiva per generare le mosse
void generate_moves(std::vector<state> grid, int cell_number, int dimension, std::vector<std::vector<std::vector<state>>>& result, int move) {
    if (move >= int_pow(cell_number, dimension)) {
        return; // Se tutte le mosse sono state fatte, esci dalla ricorsione
    }
        
    if (move==0) {
        std::vector<std::vector<state>> start;
        start.push_back(grid);
        result.push_back(start);
    }
    else{
        std::vector<std::vector<state>> collector;
    for (size_t i=0; i<result[move-1].size(); i++) {
    for (int j = 0; j < int_pow(cell_number, dimension); j++) {
        if (result[move-1][i][j] == state::N && !check_win(result[move-1][i],cell_number,dimension)) {
           std::vector<state> grid=result[move-1][i];
            // Effettua la mossa
            grid[j] = (move % 2 == 0) ? state::X : state::O;
            // Copia la griglia attuale nel vettore dei risultati
            collector.push_back(grid);
            // Annulla la mossa
            grid[j] = state::N; 
        }
    }}
    result.push_back(collector);
    }
    generate_moves(grid, cell_number, dimension, result, move + 1);
}
    std::vector<std::vector<std::vector<state>>> Board::all_plays() {
       std::vector<std::vector<std::vector<state>>> result; 
        // Richiama la funzione ricorsiva per generare le mosse
        generate_moves(grid, cell_number, dimension, result, 0);

        return result;
    }
//---------------------------------NOTES---------------------------
/* OLD LINE CHECK 
    int stop=0;
    int sign;
    if (abs(line[order])==line[order]) {sign= 1;}
    else {sign=-1;}
    state player =grid[start];
    while (player!=state::N){
        cout << "player: " << static_cast<int>(player) << ", u: " << u << ", flag: " << flag << ", start: " << start << ", order: " << order << ", stop: " << stop << endl;//DA TOGLIERE
        if (start+u>=static_cast<int>(grid.size())){
            if (u>=line[order]) {
            u = -abs(line[order]);
            sign=-sign;
            }
            else {player=state::N;}
        }
        else if (start+u<0) {
            cout<<"CIAO"<<endl;
            player=state::N;}
        if (grid[start+u]==player && stop==0 && player!=state::N) {            
            for (size_t i=0; i<constrain.size(); i++) {
            for (size_t j=0; j<constrain[i].size(); j++) {
                if (start+u==constrain[i][j]) {
                    std::vector<int> check;
                    if (i>=(constrain.size()/2)){check = canonic(cell_number-1,i);}
                    else {check = inv_canonic(cell_number-1,i);}
                    cout<<"check è:"<<endl;
                    prin(check);
                    if (sign*scalar_p(check,directions[order])>0) {stop = 1;}

                    // dovrei qui confrontare con la direzione u vettoriale e i=(0,0,0,...1...0)
                    // se ne ho anche solo una (u) che ha una componente su i (si potrebbe dire se <k,i>!=0)
                    // allora stop e si continua con lo stesso principio al contrario
                }
            }
            }
            u+=u;
            flag +=1;        
        }                
        else if (grid[start+u]==player && stop!=0 && player!=state::N) {
            u = -abs(line[order]);
            sign=-sign;
            stop=0;
        }
        else if (grid[start+u]!=player && stop==0 && player!=state::N) {
            if (u>=line[order]) {
            u = -abs(line[order]);
            sign=-sign;
            }
            else {player=state::N;}
        }
        
    }
    if (flag==cell_number) {return true;}
    else {return false;}
}
    */
// check sul ciclo di line check 
/*if (i>=dimension) {cout<<"k_module("<<int_pow(cell_number,i+1-dimension)<<","<<start+u<<")==constrain["<<i<<"]["<<j<<"] -> "<<k_module(int_pow(cell_number,i+1-dimension),start+u)<<"=="<<constrain[i][j]<<endl;}
                else {cout<<"k_module("<<int_pow(cell_number,i+1)<<","<<start+u<<")==constrain["<<i<<"]["<<j<<"] -> "<<k_module(int_pow(cell_number,i+1),start+u)<<"=="<<constrain[i][j]<<endl;}*/
// CHECK 4 CHECK_WIN
/*    cout<<"k+1 è: "<<k+1<<endl;
    cout<<"K è:"<<k<<endl;
  if (k+1<cell_number) {cout<<"Il corrispettivo k_module per i("<<i<<") f("<<f<<") e k("<<k<<") è : "<<k_module(int_pow(cell_number,k+1),i)<<endl;
   cout<<"mentre limit[k][f] è: "<<limit[k][f]<<endl;
    if (k_module(int_pow(cell_number,k+1),i)==limit[k][f]) {cout<<"TRUE"<<endl;}}
else if (k+1>=cell_number) {cout<<"Il corrispettivo k_module per i("<<i<<") f("<<f<<") e k("<<k<<") è : "<<k_module(int_pow(cell_number,k+2-cell_number),i)<<endl;
   cout<<"mentre limit[k][f] è: "<<limit[k][f]<<endl;
    if (k_module(int_pow(cell_number,k+2-cell_number),i)==limit[k][f]) {cout<<"TRUE"<<endl;}}*/
    // inoltre sul ciclo k con line_check
//cout<<"line check[k="<<k<<"] = line check[n_directions["<<k<<"]] = "<<line_check(grid,cell_number,dimension,n_directions[k],i, limit)<<endl;
