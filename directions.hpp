#ifndef DIRECTIONS
#define DIRECTIONS



#include <iostream>
#include <vector>

// Parte extra

// Funzione che calcola iul fattoriale
unsigned long long factorial(int n);

// Funzione per rendere int pow
int int_pow (int a, int b);

// Funzione per valutare un intero in Z/k
int k_module(int k, int evaluate);

// Funzione per stampare le direzioni
void print( std::vector<std::vector<int>>& combinazioni);
void print( std::vector<int>& combinazioni);

// Funzione prodotto scalare vettoriale
int scalar_p(const std::vector<int>& vector1, const std::vector<int>& vector2);

// Booleano che verifica se un dato int appartiene a un dato vector<int> o vector<vector<int>>
bool element_verify (int element, const std::vector<int>& row);
//generalizzazione per vector<vector<int>>
bool element_verify (int i, const std::vector<std::vector<int>>& limit,int cell_number, int dimension);
//---------------------------------------------------------------------------------
// Parte di selezione

// Funzione per rimuovere gli elementi uguali in un vector
void remove_repetitions(std::vector<int>& verify);
// Funzione per verificare se due vettori rappresentano la stessa direzione
bool same_directions(const std::vector<int>& first, const std::vector<int>& second);

// Funzione per verificare lo zero
bool zero_check (const std::vector<int>& check_d);

// Funzione per togliere lo zero
std::vector<std::vector<int>> zero_cut(std::vector<std::vector<int>> directions);

// Funzione di verifica angoli
bool is_corner(int cell_number, int dimension, int i);

//----------------------------------------------------------------------------------------
//Parte di generazione

// Funzione ricorsiva per generare tutte le combinazioni di +1, -1 e 0 in k scatole
void comb_gen(std::vector<int>& combination, int k, std::vector<std::vector<int>>& risultato, int posizione); 

// Funzione che genera il vettore i-esimo canonico di dim cell_number 
std::vector<int> canonic(int dimension, int i);
std::vector<int> inv_canonic(int dimension, int i);

//Funzione per generare gli angoli
std::vector<int> corner_generator (int cell_number, int dimension, int first_corner,std::vector<int>& collector);
//-----------USO ESTERNO------------------------------------------------------------------

// Funzione per trasformare le direzioni nei numeri equivalenti
std::vector<int> numeric_directions (int cell_number, std::vector<std::vector<int>> source);

// Funzione per generare e inscatolare in un vector le possibili direzioni
std::vector<std::vector<int>> k_comb_gen(int k);

// Funzione per selezionare le direzioni uniche da un insieme di vettori
std::vector<std::vector<int>> select_directions(const std::vector<std::vector<int>>& all_directions);

// Funzione che rimuove le direzioni che hanno componente j-esima pari ad a
std::vector<std::vector<int>> j_a_remover(std::vector<std::vector<int>>& source, int j, int a);

/* Funzione che restituisce, data dimensione e cell-number i costrains, ovvero i numeri limitatori,
date le n dimensioni indicheranno rispetto la k-esima i numeri tali per cui 
if (k_module(int_pow(cell_number,k+1),j+int_pow(cell_number,i))==costrains[k] && k>=i)) {directions = j_a_remover(directions,k,1);}
else if (k_module(int_pow(cell_number,k+1),j+int_pow(cell_number,i))==costrains[k] && k<i)){directions = j_a_remover(directions,k,-1);}*/
std::vector<std::vector<int>> constrains (int cell_number, int dimension);

// Funzione restituisce i numeri delle celle non angolari e non interne
std::vector<std::vector<std::vector<int>>> hyper_constrains (std::vector<std::vector<int>> constrains,int cell_number, int dimension);

#endif