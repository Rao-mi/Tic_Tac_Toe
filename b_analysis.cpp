#include <iostream>
#include <fstream>
#include <sstream>
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
    double passo;
    bool flag=true;
    std::vector<state> record;
    std::vector<std::vector<int>> big_record;
    cout<<"------------- SET DELL'AMBIENTE -------------"<<endl;
    cout<<"Quale numero di celle per dimensione si vogliono avere?"<<endl;
    cin>>cell_number;
    cout<<"Con quale dimensione ?"<<endl;
    cin>>dimension;
    cout<<"------------- SET DEL TRAINING -------------"<<endl;
    cout<<"Quanti giochi di verifica vuoi fare?"<<endl;
    cin>>games_number;

    while(flag) {
    cout<<"Chi vuoi fare giocare? ('R' per random, 'M' per minimax, 'I' per ising)"<<endl;
    cin>>first;
    cout<<"Contro chi? ('R' per random, 'M' per minimax, 'I' per ising)"<<endl;
    cin>>second;
    if (first=='I' || second=='I') flag=false;
    else cout<<"ERRORE!! Questo eseguibile server per determinare valori ottimali di b, pertanto DEVE esserci almeno un giocatore Ising-like"<<endl;
    }

    flag=true;
    while(flag) {
    cout<<"Quale passo vuoi settare per l'incremento del bias?"<<endl;
    cout<<"(un valore tra 0 e 1): ";
    cin>>passo;
    if (passo<1 && passo>0) flag=false;
    else cout<<"ERRORE!! Il passo DEVE essere compreso tra 0 e 1"<<endl;
    }
    int contapassi=1;
    double bias=0;
    while(bias!=1) {
        record.clear();
    for (int i=0; i<games_number; i++) {
        Board tavola(cell_number,dimension);
        record.push_back(tavola.Arena(first,second,bias));
    }
    int count1=0;
    int count2=0;
    int count3=0;
    
    for (size_t j=0; j<record.size(); j++){
        if (record[j]==state::N) {count1++;}
        if (record[j]==state::X) {count2++;}
        if (record[j]==state::O) {count3++;}
    }
    std::vector<int> collector;
    collector.push_back(count2);
    collector.push_back(count1);   
    collector.push_back(count3); 
    big_record.push_back(collector);
    if (bias+passo>=1) bias=1;
    else bias=bias+passo;
    cout<<"Siamo al passo n: "<<contapassi<<"; il bias totale è: "<<bias<<endl;
    contapassi++;
    }

    // Costruisci il nome del file in base alle variabili
    std::ostringstream oss;
    for (int i=0; i<dimension; i++){
        if(i!=dimension) oss<< cell_number <<"x";
        else oss<<cell_number;
    }
    oss << "_datas_bin_" << passo << "_games_" << games_number<< ".csv";
    std::string nome_file = oss.str();

    std::ofstream file1(nome_file);
    if (file1.is_open()) {
        file1<<"bias, X, O, N"<<endl;
    for (size_t i=0; i<big_record.size(); i++) {
        double current_passo;
        if (i==big_record.size()) current_passo=1;
        else current_passo=passo*(1+i);
        file1<<current_passo<<" ,"<<big_record[i][0]<<" ,"<<big_record[i][1]<<" ,"<<big_record[i][2]<<endl;

    }
    file1.close();
    std::cout << "Dati scritti con successo nel file CSV." << std::endl;
    } else {
    std::cerr << "Impossibile aprire il file." << std::endl;
    }
}