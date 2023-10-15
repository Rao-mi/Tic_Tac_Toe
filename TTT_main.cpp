#include "Tic_Tac_Toe.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;



int main() {
    Board tavola(3,2);
    std::vector<std::vector<std::vector<state>>> history;
    history=tavola.all_plays();
    for (size_t j=0; j<history.size(); j++) {
        std::cout<<"Alla mossa "<<j<<" abbiamo le seguenti possibilità"<<std::endl;
        for (size_t k=0; k<history[j].size(); k++) {
            std::cout<<"Possibilità n "<<k<<std::endl;
        for (size_t i=0; i<history[j][k].size(); i++) {
           // for (size_t l=0; l<history[j][k][i].size(); l++) {
        if (i==2 || i==5 || i==8) {
            if (history[j][k][i]==state::X) {cout<<" X "<<endl;}
            else if (history[j][k][i]==state::O) {cout<<" O "<<endl;}
            else if (history[j][k][i]==state::N) {cout<<"[ ]"<<endl;}
        }
        else {
            if (history[j][k][i]==state::X) {cout<<" X ";}
            else if (history[j][k][i]==state::O) {cout<<" O ";}
            else if (history[j][k][i]==state::N) {cout<<"[ ]";}
        }
    }    
        }
        std::cout<<"--------------------------------------------------------------------"<<std::endl;
        }
   /* std::ofstream file("TTT_3^2.csv");
        if (file.is_open()) {
        file << "Nome,Età,Città\n";
        file << "Alice,25,New York\n";
        file << "Bob,30,San Francisco\n";
        file << "Charlie,22,Los Angeles\n";

        file.close();
        std::cout << "Dati scritti con successo nel file CSV." << std::endl;
    } else {
        std::cerr << "Impossibile aprire il file." << std::endl;
    }*/

    return 0;
    
    }
