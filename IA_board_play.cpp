#include <iostream>
#include "directions.hpp"
#include "Tic_Tac_Toe.hpp"

using std::cout;
using std::cin;
using std::endl;


int main() {
Board tavola(3,2);
	cout << "********************************\n\n\tTic Tac Toe AI\n\n********************************" << endl << endl;
	cout << "Player = X\t AI Computer = O" << endl << endl;
    char IA;
    cout << "Con quale IA si vuole giocare (I = Ising, M = minimax)?"<<endl;
    cin>>IA;
    while (IA!='I' && IA!='M') {
        cout<<"INSERIMENTO SCORRETTO !"<<endl;
        cout<<"Ripetere l'inserimento (I = Ising, M = minimax):"<<endl;
        cin>>IA;
    }
    tavola.print2D();
    bool game=true;
    while (game)
    {
     if (IA=='I'){game=tavola.AI_Ising_Move();}
        else {game=tavola.AI_Ising_Move();}
        tavola.print2D();
    }
    
    return 0;
}