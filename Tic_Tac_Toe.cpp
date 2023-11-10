#include "Tic_Tac_Toe.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include "directions.hpp"

using std::cout;
using std::cin;
using std::endl;

// VALORI NECESSARI A MINMAX
//-------------------------------//
#define WIN 100000               //
#define	DRAW 0                   //
#define LOSS -100000             //
#define AI_MARKER state::O       //
#define PLAYER_MARKER state::X   //
//-------------------------------//

/////////////////////////////////////// FUNZIONI NON PUBBLICHE ///////////////////////////////////////

//[0]------------------------------- FUNZIONI NECESSARIE A BOARD ------------------------------------

//[0][0] Funzione che mi conta i tipi
int type_count (state type, std::vector<state> grid) {
    int flag=0;
    for (size_t i=0; i<grid.size(); i++) {
        if (grid[i]==type) {flag=flag+1;}
    }
    return flag;
}

//[0][1] Funzione per rimuovere uno stato nel vettore degli stati
void removeVectors(std::vector<std::vector<state>>& v1,std::vector<state> v2) {
    std::vector<std::vector<state>> result;
    for (size_t i=0; i<v1.size(); i++) {
        if (v1[i]!=v2) {
            result.push_back(v2);
        }
    }
    v1=result;
}

//[0][2] Funzione che gestisce l'fstream del file di stati
std::vector<std::vector<state>> state_finder(size_t from_move) {
    std::vector<std::vector<state>> TicTacToeStates;
    std::ifstream inputFile("TTT_3^2.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Errore nell'apertura del file CSV." << std::endl;
        TicTacToeStates.clear();
        return TicTacToeStates;
    }

    std::vector<state> currentMove;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (line.find("-------------") != std::string::npos) {
            // Fine della mossa, aggiungi il vettore di stato corrente a ticTacToeStates
            TicTacToeStates.push_back(currentMove);
            currentMove.clear();
        } else {
            // Assicurati che la riga non sia vuota prima di leggere i caratteri
            if (!line.empty()) {
                // Leggi la riga e aggiungi gli stati a currentMove
                for (char c : line) {
                    if (c == 'X') {
                        currentMove.push_back(state::X);
                    } else if (c == 'O') {
                        currentMove.push_back(state::O);
                    } else if (c == 'N') {
                        currentMove.push_back(state::N);
                    }
                }
            }
        }
    }
    if (from_move==0) {return TicTacToeStates;}
    else {
        for (size_t i=0; i<from_move; i++) {
            removeVectors(TicTacToeStates,TicTacToeStates[i]);
        }
        return TicTacToeStates;
    }
}
//[0][3] Funzione che verifica sulla direzione data tutte le celle per vedere se c'è una vincita
bool line_check (std::vector<state>& grid, int cell_number, int dimension, int line, int start,std::vector<int> v_dir, std::vector<std::vector<int>> constrain) {
    int u = line;
    int flag=1;
    //cout<<"cell_number è: "<<cell_number<<endl;
    //cout<<"dimension è: "<<dimension<<endl;
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
                    if (i>=dimension){check = canonic(dimension,i-dimension);}
                    else {check = inv_canonic(dimension,i);}
                    // cout<<"check è: "<<endl;
                   // print(check);
                    //cout<<"v_dir è: "<<endl;
                    //print(v_dir); 
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
//[0][4] Funzione che verifica se la seconda griglia è evoluzione della prima (generico temporale)
bool areVectorsEquivalent(const std::vector<state>& vec1, const std::vector<state>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); i++) {
        if (vec2[i] != state::N) {
            if (vec1[i] != vec2[i]) {
                return false;
            }
        }
    }
    return true;
}

//[0][5] Funzione per confrontare l'uguaglianza di due vettori state
bool areVectorsEqual(const std::vector<state>& vec1, const std::vector<state>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }

    for (size_t i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }

    return true;
}

//[0][6] Funzione per rimuovere i vettori duplicati in un vector<vector<state>>
void removeDuplicateVectors(std::vector<std::vector<state>>& history) {
    std::vector<std::vector<state>> uniqueVectors;

    for (const auto& vector : history) {
        bool isDuplicate = false;

        for (const auto& uniqueVector : uniqueVectors) {
            if (vector.size() != uniqueVector.size()) {
                continue; // Dimensioni diverse, non possono essere uguali
            }

            if (areVectorsEqual(vector, uniqueVector)) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            uniqueVectors.push_back(vector);
        }
    }

    // Sovrascrivi history con i vettori univoci
    history = uniqueVectors;
}

//[0][7] Funzione per eliminare i vettori duplicati nei vettori di vettori di state
void removeDuplicateVectors(std::vector<std::vector<std::vector<state>>>& history) {
    for (size_t i=0; i<history.size(); i++) {
        removeDuplicateVectors(history[i]);
    }
}


//[0][8] Funzione di supporto che fa il check della vincita
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
//[0][9] Funzione ricorsiva per generare le mosse possibili
void generate_moves(std::vector<state> grid, int cell_number, int dimension, std::vector<std::vector<std::vector<state>>>& result, int move) {
    if (move > int_pow(cell_number, dimension)) {
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
            grid[j] = (move % 2 == 0) ? state::O : state::X;
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

//[1]--------------------------------FUNZIONI NECESSARIE A MINMAX------------------------------------

//[1][0] Funzione che verifica la vincita per un tipo state
bool reduced_check_win(std::vector<state> grid, int cell_number, int dimension, state player) {
    int tot_dim = int_pow(cell_number,dimension);
    std::vector<std::vector<int>> limit = constrains(cell_number,dimension); // Grazie a questo vettore posso definire i constrains, mediante aritmetica modulare
    std::vector<int> n_directions; // Essendo una matrice su vettore dovrò trasformare le direzioni in numeri da sommare
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
    }
    return false;
}
//[1][1] Funzione che verifica se non ci sono più mosse legali
bool board_is_full(std::vector<state> grid) {
    int flag=0;
    for (size_t i=0; i<grid.size(); i++) {
        if (grid[i]==state::N) {flag=flag+1;}
    }
    if (flag==0) return true;
    else return false;
}
//[1][2] Funzione che restituisce lo stato opposto
state get_opponent(state player) {
    state opponent;
    if (player==state::X) {opponent=state::O;}
    else if (player==state::O) {opponent=state::X;}
    else {cout<<"Errore di inversione get_opponent"<<endl;
    opponent=player;}
    return opponent;
}
//[1][3] Funzione che restituisce lo stato intero della griglia
int get_board_state (std::vector<state> grid, int cell_number, int dimension, state player) {
state opponent=get_opponent(player);

    if (reduced_check_win(grid,cell_number,dimension,player)) {return WIN;}

    if (reduced_check_win(grid,cell_number,dimension,opponent)) {return LOSS;}

    return DRAW;

}
std::vector<int> get_legal_moves (std::vector<state> grid) {
    std::vector<int> result;
for (size_t i=0; i<grid.size(); i++) {
    if (grid[i]==state::N) {
        result.push_back(i);
    }
}
return result;
}
//[1][4] Funzione minimax
std::pair<int, int> minimax_optimization( std::vector<state>& grid, int cell_number, int dimension,state marker, int depth, int alpha, int beta){
    // Initialize best move
    int best_move = -1;
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;

    // If we hit a terminal state (leaf node), return the best score and move
    if (board_is_full(grid) || DRAW != get_board_state(grid, cell_number, dimension, AI_MARKER))
    {
        best_score = get_board_state(grid, cell_number, dimension, AI_MARKER);
        return std::make_pair(best_score, best_move);
    }

    std::vector<int> legal_moves = get_legal_moves(grid);

    for (size_t i = 0; i < legal_moves.size(); i++)
    {
        int curr_move = legal_moves[i];
        grid[curr_move] = marker;

        // Maximizing player's turn
        if (marker == AI_MARKER)
        {
            int score = minimax_optimization(grid, cell_number, dimension, PLAYER_MARKER, depth + 1, alpha, beta).first;

            // Get the best scoring move
            if (best_score < score)
            {
                best_score = score - depth * 10;
                best_move = curr_move;

                // Check if this branch's best move is worse than the best
                // option of a previously searched branch. If it is, skip it
                alpha = std::max(alpha, best_score);
                grid[curr_move] = state::N;
                if (beta <= alpha) 
                { 
                    break; 
                }
            }
        }
        else // Minimizing opponent's turn
        {
            int score = minimax_optimization(grid, cell_number, dimension, AI_MARKER, depth + 1, alpha, beta).first;

            if (best_score > score)
            {
                best_score = score + depth * 10;
                best_move = curr_move;

                // Check if this branch's best move is worse than the best
                // option of a previously searched branch. If it is, skip it
                beta = std::min(beta, best_score);
                grid[curr_move] = state::N;
                if (beta <= alpha) 
                { 
                    break; 
                }
            }
        }

        grid[curr_move] = state::N; // Undo move
    }
    return std::make_pair(best_score, best_move);
}
//[2]------------------------- FUNZIONI NECESSARIE A ISING-LIKE-MODEL -------------------------------

//[2][0] Funzione per inveertire lo stato
state state_inverter (state player) {
    if (player==state::X) {
        return state::O;
    }
    else if (player==state::O){
        return state::X;
    }
    else {
        return state::N;
    }
}

//[2][1] Funzione per calcolare lungo la direzione l'energia
long double line_energy (char type,std::vector<state> grid, int start,int cell_number, int dimension, int n_direction_k, std::vector<int> v_dir,state player, std::vector<std::vector<int>> constrain) {
    long double unit=pow(factorial(cell_number),(-1.0/cell_number));
    //opzione decrescente 
    if (type=='D'){unit=pow((factorial(cell_number)),(1.0/cell_number));}
    if (type=='A'){unit=pow((cell_number+1.0),(1.0/cell_number));} //funziona con {unit=pow((factorial(cell_number+1)),(1.0/cell_number));} 
    int u = n_direction_k;
    int flag=1;
    if (grid[start]==state::N) {flag=0;}
    int anti_flag=0;
     if (grid[start]==get_opponent(player)) {anti_flag=1;}
    while (player!=state::N){
         if (start+u>=static_cast<int>(grid.size()) || start+u<0){
            player=state::N;
         }
        else if (grid[start+u]==player) { 
            for (int i=0; i<static_cast<int>(constrain.size()); i++) {
            for (int j=0; j<static_cast<int>(constrain[i].size()); j++) {      
            if ((k_module(int_pow(cell_number,i+1-dimension),start+u)==constrain[i][j] && i>=dimension)||(k_module(int_pow(cell_number,i+1),start+u)==constrain[i][j] && i<dimension)) {
                    std::vector<int> check;
                    if (i>=dimension){check = canonic(dimension,i-dimension);}
                    else {check = inv_canonic(dimension,i);}
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
        else if (grid[start+u]==get_opponent(player) && player!=state::N){
            for (int i=0; i<static_cast<int>(constrain.size()); i++) {
            for (int j=0; j<static_cast<int>(constrain[i].size()); j++) {      
            if ((k_module(int_pow(cell_number,i+1-dimension),start+u)==constrain[i][j] && i>=dimension)||(k_module(int_pow(cell_number,i+1),start+u)==constrain[i][j] && i<dimension)) {
                    std::vector<int> check;
                    if (i>=dimension){check = canonic(dimension,i-dimension);}
                    else {check = inv_canonic(dimension,i);}
                    if (scalar_p(check,v_dir)>0) {
                    player=state::N;
                    j=constrain[i].size()-1;
                    i=constrain.size()-1;
                    } 

                }   
            }}        
            u+=u;
            anti_flag +=1;
        } 
        else {player=state::N;}          
    }
    long double line_energy=1.0;
    if(cell_number-flag-anti_flag!=0){ 
    line_energy = 1.0;}
    for (double i=1; i<flag+1; i++) {
        // opzione decrescente
        if (type=='D' /*|| type=='A'*/ ){line_energy=line_energy*unit/i;}
        else {line_energy = line_energy*unit*i;}
    }
   // if (type=='A'&& flag==cell_number){ line_energy=pow(cell_number,dimension);}
    if (anti_flag!=0) {line_energy=0;}
    return line_energy;
}

//[2][2] Funzione per calcolare l'energia secondo Ising-like
long double Energy(std::vector<state> grid,int cell_number, int dimension, state player, double k, char type) {
    long double tot_energy=0.0;
    int tot_dim = int_pow(cell_number,dimension);
    std::vector<std::vector<int>> limit = constrains(cell_number,dimension); // Grazie a questo vettore posso definire i constrains, mediante aritmetica modulare
    std::vector<int> n_directions;
    std::vector<std::vector<std::vector<int>>> terminal_directions = hyper_constrains(limit,cell_number,dimension);
    for (int i=0; i<tot_dim; i++) {
            n_directions = numeric_directions(cell_number,terminal_directions[i]); // Trasformo i vettori in numeri  
                       for (size_t k=0; k<n_directions.size(); k++){
                        if (terminal_directions[i][k].size()!=0){
                            long double linear_energy =line_energy(type,grid,i,cell_number,dimension,n_directions[k],terminal_directions[i][k],player,limit);
                            tot_energy=tot_energy+linear_energy;
                            }                            
        }
}
tot_energy=k*tot_energy;
return tot_energy;
}

//[2][3] Funzione che returna la mossa che minimizza l'energia

int min_Energy(std::vector<state> grid,int cell_number, int dimension, state player, double bias) {
    long double init_energy = Energy(grid,cell_number,dimension,player,0.8,'A');
    std::vector<long double> energy_record;
    int tot_dim = int_pow(cell_number,dimension);
    for(int i=0; i<tot_dim; i++){
        if (grid[i]==state::N) {
            if (type_count(state::N,grid)==1){return i;}
            long double count=0;
            grid[i]=player;
            count=Energy(grid,cell_number,dimension,player,1.0,'A');
            grid[i]=state::N;
            grid[i]=get_opponent(player);
            count += bias*Energy(grid,cell_number,dimension,get_opponent(player),1.0,'A');
            grid[i]=state::N;
            energy_record.push_back(count);
        }
        else {energy_record.push_back(-100);}
    }
    std::vector<long double> energy_min;
    long double fake_energy = -pow(init_energy,2);
    
    for (size_t j=0; j<energy_record.size(); j++){
       if (energy_record[j]!=-100){
        if (energy_record[j]>init_energy && fake_energy<=energy_record[j]) {
            fake_energy =energy_record[j];
           if (energy_min.size()==0){
            energy_min.push_back(fake_energy);
            energy_min.push_back(static_cast<long double>(j));
           }
           else if (energy_min[0]==fake_energy){
            energy_min.push_back(static_cast<long double>(j));
           }
           else {
            energy_min.clear();
            energy_min.push_back(fake_energy);
            energy_min.push_back(static_cast<long double>(j));
           }
        }
        }
    }

    if (energy_min.size()==2){
        return static_cast<int>(energy_min[1]);
    }
    std::random_device rd;  // Genera un seed casuale
    std::mt19937 gen(rd()); // Generatore di numeri casuali Mersenne Twister
    std::uniform_int_distribution<int> dist(1,static_cast<int>(energy_min.size()-1)); // Distribuzione uniforme
    size_t random_number = dist(gen);
    return static_cast<int>(energy_min[random_number]);
    
return -1; // Valore predefinito errore

}

//[3]---------------------------- FUNZIONI NECESSARIE ALLA VERIFICA ---------------------------------

//[3][0] Funzione che returna una mossa random

int Ai_random_play(std::vector<state> grid) {
    std::vector<int> possible_moves;
    for (size_t i=0; i<grid.size(); i++){
        if (grid[i]==state::N){
            possible_moves.push_back(i);
        }
    }
    std::random_device rd;  // Genera un seed casuale
    std::mt19937 gen(rd()); // Generatore di numeri casuali Mersenne Twister
    std::uniform_int_distribution<int> dist(0,static_cast<int>(possible_moves.size()-1)); // Distribuzione uniforme
    size_t random_number = dist(gen);
    return possible_moves[random_number];
}

// [3][1] Funzione che assegna un intero ad ogni coppia ordinata di giocatori
int possible_players (char first, char second) {
    int flag=0;                                 //
    if (first==second) {                        // first è R se flag == 1,4,5  
    if (first=='R'){flag=1;}                    // first è M se flag == 2,6,7
    else if (first=='M'){flag=2;}               // first è I se flag == 3,8,9
    else if (first=='I'){flag=3;}               //
}
else if (first=='R') {                          //
    if (second=='M'){flag=4;}                   // second è R se flag == 1,6,8
    else if (second=='I'){flag=5;}              // second è M se flag == 2,4,9
}                                               // second è I se flag == 3,5,7
else if (first=='M') {                          //
    if (second=='R'){flag=6;}
    else if (second=='I'){flag=7;}
}
else if (first=='I') {
    if (second=='R'){flag=8;}
    else if (second=='M'){flag=9;}
}
return flag;
}
//////////////////////////////////////////// CLASSE BOARD ////////////////////////////////////////////

// (1) Costruttore
    Board::Board (int set_cell_n,int set_dim)
    : cell_number(set_cell_n), dimension(set_dim), 
    grid(int_pow(set_cell_n,set_dim),state::N) {}

// (2) Funzione per giocare con due giocatori da terminale
bool Board::Move(state player) {
    bool win;
    int a=0;
    int cell=0;
    
    if (!board_is_full(grid)) {

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

// (3) Funzione per giocare contro AI stile min-max con alfa-beta pruning
bool Board::AI_Move(){
    bool win;
    int a=0;
    int cell=0;
    bool flag=true;
     if (!board_is_full(grid)) {
            while (flag) {
            a=0;
            cell=0;
            for (int i=0; i<dimension; i++) {
        
                cout<<"Inserire la componente x_"<<i<<" della cella che si vuole giocare"<<endl;
                cin>>a;
                cell+= a*int_pow(cell_number,i);}

                if (grid[cell]==state::N) {
                    flag=false;
                    grid[cell]=PLAYER_MARKER;
                }
                else {cout<<"Cella già occupata"<<endl;
                cin.get();
                }}
                win = check_win(grid,cell_number,dimension);
            if (win) {
        cout<<"La partita è conclusa, il vincitore è: X"<<endl;
        return !win;}
        else if(board_is_full(grid)) { cout<<"La partita è conclusa. Patta"<<endl;
        return false;}
        else {
            std::pair<int,int> ai_move = minimax_optimization(grid,cell_number,dimension,AI_MARKER,0,LOSS,WIN);
            grid[ai_move.second]=AI_MARKER;
            win = check_win(grid,cell_number,dimension);
            if (win) {
        cout<<"La partita è conclusa, il vincitore è: O"<<endl;
        return !win;}
            else return !win;
        }
    }

    else { cout<<"La partita è conclusa. Patta"<<endl;
        return false;}
}

// (4) Funzione per giocare contro AI stile Ising
bool Board::AI_Ising_Move(){
    bool win;
    int a=0;
    int cell=0;
    bool flag=true;
     if (!board_is_full(grid)) {
            while (flag) {
            a=0;
            cell=0;
            for (int i=0; i<dimension; i++) {
        
                cout<<"Inserire la componente x_"<<i<<" della cella che si vuole giocare"<<endl;
                cin>>a;
                cell+= a*int_pow(cell_number,i);}

                if (grid[cell]==state::N) {
                    flag=false;
                    grid[cell]=PLAYER_MARKER;
                }
                else {cout<<"Cella già occupata"<<endl;
                cin.get();
                }}
                win = check_win(grid,cell_number,dimension);
            if (win) {
        cout<<"La partita è conclusa, il vincitore è: X"<<endl;
        return !win;}

        else if(board_is_full(grid)) { cout<<"La partita è conclusa. Patta"<<endl;
        return false;}
        else {
            int ai_move = min_Energy(grid,cell_number,dimension,state::O,0.8);
            grid[ai_move]=AI_MARKER;
            win = check_win(grid,cell_number,dimension);
            if (win) {
        cout<<"La partita è conclusa, il vincitore è: O"<<endl;
        return !win;}
        else return !win;
        }
    }

    else { cout<<"La partita è conclusa. Patta"<<endl;
        return false;}

}

// (5) Funzione per far giocare le AI una contro l'altra: 'R' per random, 'M' per minimax, 'I' per ising
state Board::Arena(char first, char second){
bool win;
int flag=possible_players(first,second);

while (!board_is_full(grid)) {
    if (flag==1 || flag==4 || flag==5) {grid[Ai_random_play(grid)]=PLAYER_MARKER;}
    else if (flag==2 || flag==6 || flag==7) {std::pair<int,int> result = minimax_optimization(grid,cell_number,dimension,PLAYER_MARKER,0,LOSS,WIN);
    grid[result.second]=PLAYER_MARKER;}
    else if (flag==3 || flag==8 || flag==9) {grid[min_Energy(grid,cell_number,dimension,PLAYER_MARKER,0.8)]=PLAYER_MARKER;}
    
    win = check_win(grid,cell_number,dimension);
        
    if(board_is_full(grid)) {return state::N;}

    else if (win) {return state::X;}

    else {
        if (flag==1 || flag==6 || flag==8) {grid[Ai_random_play(grid)]=AI_MARKER;}
        else if (flag==2 || flag==4 || flag==9) {std::pair<int,int> result = minimax_optimization(grid,cell_number,dimension,AI_MARKER,0,LOSS,WIN);
        grid[result.second]=AI_MARKER;}
        else if (flag==3 || flag==5 || flag==7) {grid[min_Energy(grid,cell_number,dimension,AI_MARKER,0.8)]=AI_MARKER;}
        
        win = check_win(grid,cell_number,dimension);

        if(board_is_full(grid)) {return state::N;}

        if (win) {return state::O;}
    
}
}
return state::N;
}

// (6) Funzione per stampare la griglia 2D (volendo anche in dimensioni superiori)
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

// (7) Funzione per generare tutte le possibili partite e salvarle in 
// un std::vector<std::vector<std::vector<state>>> [i][j][k] ove i è il numero di mosse,
// j le possibile griglie(partite) a quel numero di mosse ad ogni k corrisponde l'elemento state della griglia
std::vector<std::vector<std::vector<state>>> Board::all_plays() {
   std::vector<std::vector<std::vector<state>>> result; 
    // Richiama la funzione ricorsiva per generare le mosse
    generate_moves(grid, cell_number, dimension, result, 0);
    removeDuplicateVectors(result);
    return result;
}

// (8) Funzione per verificare se è finito il gioco e nel caso chi ha vinto
char Board::End() {
   if (check_win(grid,cell_number,dimension)) {
    if (type_count(state::X,grid)>type_count(state::O,grid)){
        return 'X';
    }
    else if (type_count(state::X,grid)<=type_count(state::O,grid)) {
        return 'O';
    }
   }
   
    return '-';
}

//////////////// DA NON USARE //////////////////
void Board::setgrid (std::vector<state> state) {
    grid = state;

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


/*
// da aggiungere la parte di fluttuazione nel caso di mancanza di mosse minimizzanti
    fake_energy=pow(init_energy,2);
    energy_record.clear();
if (type=='R') {
// opzione ricorsiva
    if(energy_min.size()==1){return static_cast<int>(energy_min[0]);}
    for(size_t i=1; i<energy_min.size(); i++){
        grid[energy_min[i]]=player;
        int opponent = min_Energy(grid,cell_number,dimension,get_opponent(player),'R');
        grid[opponent] = get_opponent(player);
        energy_record.push_back(Energy(grid,cell_number,dimension,player,1.0,'f'));
        grid[energy_min[i]]=state::N;
        grid[opponent] = state::N;
    }
    std::vector<long double> energy_opponent_max;
    for(size_t i=1; i<energy_min.size(); i++){
            if (energy_record[i-1]<energy_min[0] && fake_energy>=energy_record[i-1]) {
            fake_energy =energy_record[i-1];
           if (energy_opponent_max.size()==0){
            energy_opponent_max.push_back(fake_energy);
            energy_opponent_max.push_back(static_cast<long double>(i));
           }
           else if (energy_opponent_max[0]==fake_energy){
            energy_opponent_max.push_back(static_cast<long double>(i));
           }
           else {
            energy_opponent_max.clear();
            energy_opponent_max.push_back(fake_energy);
            energy_opponent_max.push_back(static_cast<long double>(i));
           }
        }
        }
    //ora se energy_min è vuoto prendo fake_enery come riferimento e returno l'int associato
    if (energy_opponent_max.size()==0){
    for (size_t i=1; i<energy_min.size(); i++){
        if (fake_energy==energy_record[i-1]){
            return static_cast<int>(energy_min[i]);
        }
        else return -1;
    } 
    }
    // altrimenti uso energy_opponent_max
    else {
        std::random_device rd;  // Genera un seed casuale
        std::mt19937 gen(rd()); // Generatore di numeri casuali Mersenne Twister
        std::uniform_int_distribution<size_t> dist(1,(energy_opponent_max.size()-1)); // Distribuzione uniforme
        size_t random_number = dist(gen);
        return static_cast<int>(energy_opponent_max[random_number]);
    }

    
}
// parte di min_energy
// opzione non ricorsiva
else {
    fake_energy=-pow(init_energy,2);
    energy_record.clear();
    std::vector<std::vector<long double>> energy_opponent_max;
    std::vector<long double> energy_opponent;

    for (size_t k=1; k<energy_min.size(); k++){
        for(int i=0; i<tot_dim; i++){
                grid[energy_min[k]]=player;

            if (grid[i]==state::N) {
                grid[i]=get_opponent(player);
                energy_record.push_back(Energy(grid,cell_number,dimension,get_opponent(player),1.0,'f'));
                grid[i]=state::N;
            }
            else {energy_record.push_back(-100);}}
            cout<<"Alla possibilità k="<<k<<" otteniamo che se l'avversario gioca la casella"<<endl;
            for(int i=0; i<tot_dim; i++){
                cout<<i<<" si ha energia dell'avversario pari a:"<<energy_record[i]<<endl;
            }
            for (size_t j=0; j<energy_record.size(); j++){
                if (energy_record[j]!=-100){
                    if (fake_energy<=energy_record[j]) {
                        fake_energy =energy_record[j];
                        cout<<"nel caso del blocco "<<j<<" fake energy diventa: "<<fake_energy<<endl;
                        if (energy_opponent.size()==0){
                             energy_opponent.push_back(fake_energy);
                             energy_opponent.push_back(static_cast<long double>(j));
                            }
                            else if (energy_opponent[0]==fake_energy){
                                cout<<"ho aggiunto al vettore energy opponent di energia: "<<energy_opponent[0]<<" la posizione: "<<j<<endl;
                             energy_opponent.push_back(static_cast<long double>(j));
                            }
                            else {
                             energy_opponent.clear();
                             energy_opponent.push_back(fake_energy);
                             energy_opponent.push_back(static_cast<long double>(j));
                            }
                    }
                }
              }
        
    energy_opponent_max.push_back(energy_opponent);
    energy_opponent.clear();
    grid[energy_min[k]]=state::N;
    }
    fake_energy=-pow(init_energy,2);

        for (size_t k=0; k<energy_opponent_max.size(); k++){
            if (energy_opponent_max[k].size()!=0 && energy_opponent_max[k][0]>=fake_energy){fake_energy=energy_opponent_max[k][0];}
        }
cout<<"FINO A QUI OK"<<endl;
cout<<"Abbiamo "<<energy_opponent_max.size()<<" possibilità che minimizzano, le quali ci danno rispettivamente energie:"<<endl;
for (size_t k=0; k<energy_opponent_max.size(); k++){
     cout<<energy_opponent_max[k][0]<<" di mosse ";
    for (size_t j=1; j<energy_opponent_max[k].size(); j++){
    cout<<energy_opponent_max[k][j]<<" ,";
}}
cout<<endl;
    if (fake_energy !=-pow(init_energy,2)){
        std::vector<int> positions;
        for (size_t k=1; k<energy_min.size(); k++){
            if(energy_opponent_max[k-1][0]==fake_energy){
                    positions.push_back(static_cast<int>(energy_min[k]));
            }
        }
        remove_repetitions(positions);
        if (positions.size()==1){return positions[0];}
        else{
        std::random_device rd;  // Genera un seed casuale
        std::mt19937 gen(rd()); // Generatore di numeri casuali Mersenne Twister
        std::uniform_int_distribution<int> dist(0,(positions.size()-1)); // Distribuzione uniforme
        size_t random_number = dist(gen);
        return positions[random_number];
        }
    }
    else{// da aggiungere la parte di fluttuazione nel caso di mancanza di mosse massimizzanti

    }

}*/