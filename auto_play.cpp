#include "Tic_Tac_Toe.hpp"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
    int a,b;
    char id;
    bool game=true;
    state player=state::N;
    cout<<"In quale dimensione vuoi giocare?"<<endl;
    cin>>a;
    cout<<"Quanti blocchi vuoi per lato?"<<endl;
    cin>>b;
    Board tavola(b,a);

    
    while (game) {
    cout<<"Chi gioca [X/O]?"<<endl;
    cin>> id;
    if (id=='X') {player=state::X;}
    else if (id=='O') {player=state::O;}
    if (player!=state::N) {
    game=tavola.Move(player);
    tavola.print2D();
    }
    else {
        cout<<"Errore, inserisci correttamente il giocatore"<<endl;
        cin.get();
        game=true;
    }
    }


    return 0;
}