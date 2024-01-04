#include "directions.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::cin;

int main() {
    int dimension;
    int cell_number;
    int i;
    
    cout<< "Qual è la dimensione? -> : "; cin>>dimension;
    cout<< "Qual è il numero di celle? -> : "; cin>>cell_number;

    std::vector<std::vector<int>> directions = k_comb_gen(dimension);
    directions = zero_cut(directions); 
    std::vector<std::vector<int>> limit = constrains(cell_number,dimension);

    cout<< "Quale cella si vuole verificare -> : "; cin>>i; cout<<endl;

    for (int k=0; k<static_cast<int>(limit.size()); k++) {
                        for (size_t f=0; f<limit[k].size(); f++) {
                    if (k_module(int_pow(cell_number,k+1-dimension),i)==limit[k][f] && k>=dimension) {directions = j_a_remover(directions,k-dimension,1);}
                    else if (k_module(int_pow(cell_number,k+1),i)==limit[k][f] && k<dimension){directions = j_a_remover(directions,k,-1);}
                    }}


    std::ofstream file1("vectors.csv");
        
    if (file1.is_open()) {
        for (int k=0; k<static_cast<int>(directions.size()); k++) {
            for (size_t f=0; f<directions[k].size(); f++) {
                if (f!=directions[k].size()-1) {file1<<directions[k][f]<<",";}
                else {file1<<directions[k][f]<<endl;}
        }}
        file1.close();
        std::cout << "Dati scritti con successo nel file CSV." << std::endl;
    }
    else {
            std::cerr << "Impossibile aprire il file." << std::endl;
        }

return 0;
}