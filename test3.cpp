#include <iostream>
#include <chrono>
#include "directions.hpp"
#include "Tic_Tac_Toe.hpp"

using std::cout;
using std::cin;
using std::endl;

int main() {
    int games_number=0;
    char first;
    char second;
    int cell_number;
    int dimension;
    std::vector<state> record;
    cout<<"Quanti giochi vuoi fare?"<<endl;
    cin>>games_number;
    cout<<"Chi vuoi fare giocare? ('R' per random, 'M' per minimax, 'I' per ising)"<<endl;
    cin>>first;
    cout<<"Contro chi? ('R' per random, 'M' per minimax, 'I' per ising)"<<endl;
    cin>>second;
    cout<<"Con quale numero di celle ?"<<endl;
    cin>>cell_number;
    cout<<"Con quale dimensione ?"<<endl;
    cin>>dimension;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<games_number; i++) {
        cout<<"Si svolge la partita n "<<i+1<<endl;
        Board tavola(cell_number,dimension);
        record.push_back(tavola.Arena(first,second));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)/games_number);
    games_number=0;
    cell_number=0;
    dimension=0;
    for (size_t j=0; j<record.size(); j++){
        if (record[j]==state::N) {games_number++;}
        if (record[j]==state::X) {cell_number++;}
        if (record[j]==state::O) {dimension++;}
    }
    cout<<" Si ha che il primo giocatore ("<<first<<") :"<<endl;
    cout<<"vince "<<cell_number<<" volte"<<endl;
    cout<<"perde "<<dimension<<" volte"<<endl;
    cout<<"pareggia "<<games_number<<" volte"<<endl;
    cout << "Il tempo di esecuzione medio per partita è di " << duration.count() << " millisecondi." << endl;
    return 0;
}