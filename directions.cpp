#include <iostream>
#include <vector>
#include <cmath>


//(0)//////////////////////////////////////// FUNZIONI EXTRA ///////////////////////////////////////////

//(0)[0]---------------------------------- FUNZIONI MATEMATICHE ----------------------------------------

//(0)[0][0] Funzione che calcola il fattoriale
unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

//(0)[0][1] Funzione che calcola il prodotto scalare
int scalar_p(const std::vector<int>& vector1, const std::vector<int>& vector2){
    // Assicurati che entrambi i vettori abbiano la stessa dimensione
    if (vector1.size() != vector2.size()) {
        std::cerr << "I vettori hanno dimensioni diverse." << std::endl;
        return 0; // In questo caso, il prodotto scalare non è definito
    }

    int result = 0;

    // Calcola il prodotto scalare sommando i prodotti dei corrispondenti elementi
    for (size_t i = 0; i < vector1.size(); ++i) {
        result += vector1[i] * vector2[i];
    }

    return result;
}

//(0)[0][2] Funzione che rende int la funzione pow
int int_pow (int a, int b) {
    return static_cast<int>(pow(a,b));
}

//(0)[0][3] Funzione che valuta il valore di un intero in Z/k
int k_module(int k, int evaluate) {
    if (k <= 0) {
        // Gestione errore: k deve essere positivo
        return -1; // O un altro valore per indicare un errore
    }
    int result = evaluate % k;
    if (result < 0) {
        // Se il risultato è negativo, aggiungiamo k per portarlo in Z/k positivo
        result += k;
    }

    return result;
}

//(0)[0][4] Funzione di conversione di vettori direzione in direzioni numeriche
std::vector<int> numeric_directions (int cell_number, std::vector<std::vector<int>> source) {
    std::vector<int> result;
    
    for (size_t i=0; i<source.size(); i++) {
        int sum=0;
        for (size_t j=0; j<source[i].size(); j++) {
            sum += source[i][j]*int_pow(cell_number,j);
        }
        result.push_back(sum);
    }
    return result;
}
//(0)[1]----------------------------------- FUNZIONI DI OUTPUT -----------------------------------------

//(0)[1][0] Funzione che stampa le direzioni (tutti i vettori contenutiin un vector<vector<int>>)
    void print( std::vector<std::vector<int>>& combinazioni) {
        for (const std::vector<int>& combinazione : combinazioni) {
        for (int elemento : combinazione) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    } 
}

//(0)[1][1] Funzione che stampa un vettore di int 
    void print( std::vector<int>& combinazioni) {
        for (int elemento : combinazioni) {
            std::cout << elemento << std::endl;
        }
       
}

//(1)//////////////////////////////////// FUNZIONI DI GENERAZIONE //////////////////////////////////////

//(1)[0]--------------------------------- FUNZIONI DI GENERAZIONE --------------------------------------

//(1)[0][0] Funzione ricorsiva per generare tutte le combinazioni di +1, -1 e 0 in k scatole
void comb_gen(std::vector<int>& combination, int k, std::vector<std::vector<int>>& result, int posizione) {
    if (posizione == k) {
        result.push_back(combination);
        return;
    }

    // Genera le combinazioni ricorsivamente
    for (int valore : {-1, 0, 1}) {
        combination[posizione] = valore;
        comb_gen(combination, k, result, posizione + 1);
    }
}

//(1)[0][1] Funzione per returnare tutte le combinazioni di +1, -1 e 0 in k scatole
std::vector<std::vector<int>> k_comb_gen(int k) {
    std::vector<std::vector<int>> result;
    std::vector<int> combination(k, 0);

    comb_gen(combination, k, result, 0);

    return result;
}

//(1)[0][2] Funzione che genera un vettore canonico i-esimo di dimensione dimension
std::vector<int> canonic(int dimension, int i) {

    std::vector<int> vettore(dimension, 0);

    // Imposta l'elemento in posizione i a 1 (assicurati che i sia valido)
    if (i >= 0 && i < dimension) {
        vettore[i] = 1;}

    // Restituisci il vettore risultante
    return vettore;
}

//(1)[0][3] Funzione che genera l'opposto di un vettore i-esimo canonico di dimensione dimension
std::vector<int> inv_canonic(int dimension, int i) {

    std::vector<int> vettore(dimension, 0);

    // Imposta l'elemento in posizione i a 1 (assicurati che i sia valido)
    if (i >= 0 && i < dimension) {
        vettore[i] = -1;
    }

    // Restituisci il vettore risultante
    return vettore;
}

//(1)[0][4] Funzione di generazione di un vettore di vettori, contenenti i numeri limite, indicizzati in base alla direzione 
std::vector<std::vector<int>> constrains (int cell_number, int dimension) {
    std::vector<int> name;
    std::vector<int> surname;
    std::vector<std::vector<int>> result;
    
    for (int i=0; i<dimension+1; i++) {
    name.push_back(int_pow(cell_number,i)-1);}

    for (int i=0; i<dimension; i++) {
        surname.clear();
        for (int j=(int_pow(cell_number,i)-1); j>=0; j--) {

        surname.push_back(name[i]-j);
        }
        result.push_back(surname);
    }      

    for (int i=0; i<dimension; i++) {
         surname.clear();
            for (int j=(int_pow(cell_number,i)-1); j>=0; j--) {
        surname.push_back(name[i+1]-j);
        }
        result.push_back(surname);
    }      
    return result;
}

//(2)/////////////////////////////// FUNZIONI DI VERIFICA E SELEZIONE //////////////////////////////////

//(2)[0]---------------------------------- FUNZIONI DI VERIFICA ----------------------------------------

//(2)[0][0] Funzione booleana che verifica se un dato int appartiene a un dato vector<int> o vector<vector<int>>
bool element_verify(int element, const std::vector<int>& row) {
        for (int value : row) {
            if (value == element) {
                return true;
            }
        }
    return false;
}

//(2)[0][1] Funzione che generalizza la (1)[0][0] per vector<vector<int>>
bool element_verify(int i, const std::vector<std::vector<int>>& limit,int cell_number, int dimension) {
    for (int k=0; k<static_cast<int>(limit.size()); k++) {
                            for (size_t f=0; f<limit[k].size(); f++) {
                        if (k_module(int_pow(cell_number,k+1-dimension),i)==limit[k][f] && k>=dimension) {return true;}
                        else if (k_module(int_pow(cell_number,k+1),i)==limit[k][f] && k<dimension){return true;}

    }}
    return false;
}

//(2)[0][2] Funzione per verificare se due vettori rappresentano la stessa direzione
bool same_directions(const std::vector<int>& first, const std::vector<int>& second) {
    if (first.size() != second.size()) {
        return false;
    }

    size_t same_d = 0;
    size_t reversed_d = 0;

    for (size_t i = 0; i < first.size(); ++i) {
            if (first[i]==0 && second[i]==0) {
            same_d +=1; reversed_d +=1;
            } else if (first[i] == second[i]) {
                same_d +=1;
            } else if (first[i] == -second[i]) {
                reversed_d +=1;
            }

    }
    if (same_d==first.size() || reversed_d==first.size()) {return true;}
    else {return false;}
}

//(2)[0][3] Funzione per verificare se un vettore è presente nell'insieme delle direzioni
bool belong_direction(const std::vector<std::vector<int>>& all_directions, const std::vector<int>& check_d) {
    for (const std::vector<int>& one_direction : all_directions) {
        if (same_directions(one_direction, check_d)) {
            return true;
        }
    }
    return false;
}

//(2)[0][4] Funzione per verificare lo zero
bool zero_check (const std::vector<int>& check_d) {
    int check=0;
     for (size_t i = 0; i < check_d.size(); ++i) {check=check_d[i];}
     if (check==0) {return true;}
     else {return false;}
}

//(2)[0][5] Funzione per rimuovere gli elementi uguali in un vector
void remove_repetitions(std::vector<int>& verify) {
    std::vector<int> uniqueElements;
    
    for (const int& element : verify) {
        bool isUnique = true;

        for (const int& uniqueElement : uniqueElements) {
            if (element == uniqueElement) {
                isUnique = false;
                break;
            }
        }

        if (isUnique) {
            uniqueElements.push_back(element);
        }
    }

    verify = uniqueElements;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUNZIONE MISTA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//(1)[0][5]Funzione di generazione condizionale (usa funzioni di verifica) gli angoli
std::vector<int> corner_generator (int cell_number, int dimension, int first_corner,std::vector<int>& collector){
    //step 0
    if (first_corner==0) {
        collector.clear();
        collector.push_back(0);
    }

    if (!element_verify(int_pow(cell_number,dimension)-1,collector)) {

    for (const int& corner : collector) {
            int sum = corner+int_pow(cell_number,first_corner)*(cell_number-1);
            if (!element_verify(sum,collector)){
            collector.push_back(sum);}
        }
    collector=corner_generator (cell_number,dimension,first_corner+1,collector);
    }
    return collector;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//(2)[0][6] Funzione di verifica angoli
bool is_corner(int cell_number, int dimension, int i) {
  std::vector<int> corners;
  corners = corner_generator(cell_number,dimension,0,corners);
    if (element_verify(i,corners)) {return true;}
    else return false;
}
//(2)[1]---------------------------------- FUNZIONI DI SELEZIONE ---------------------------------------

//(2)[1][0] Funzione per togliere lo zero
std::vector<std::vector<int>> zero_cut(std::vector<std::vector<int>> directions) {
    std::vector<std::vector<int>> newDirections;  
    for (const std::vector<int>& direction : directions) {
        // Verifica se il vettore non è nullo
        bool isNonNull = false;
        for (int component : direction) {
            if (component != 0) {
                isNonNull = true;
                break;
            }
        }     
        // Se il vettore non è nullo, lo aggiunge al nuovo insieme
        if (isNonNull) {
            newDirections.push_back(direction);
        }
    }
    
    return newDirections;
}

//(2)[1][1] Funzione per selezionare le direzioni uniche da un insieme di vettori
std::vector<std::vector<int>> select_directions(const std::vector<std::vector<int>>& all_directions) {
    std::vector<std::vector<int>> result;

    for (const std::vector<int>& direzione : all_directions) {
        if (!belong_direction(result, direzione) && !zero_check(direzione)) {
            result.push_back(direzione);
        }
    }

    return result;
}

//(2)[1][2] Funzione che rimuove le direzioni che hanno componente j-esima pari ad a
std::vector<std::vector<int>> j_a_remover(std::vector<std::vector<int>>& source, int j, int a) {
    std::vector<std::vector<int>> result;

    for (const std::vector<int>& direction : source) {
        if ( direction[j] != a) {
            result.push_back(direction);
        }
    }

    return result;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUNZIONE MISTA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//(1)[0][6] Funzione di generazione condizionale (usa le verifiche) e selezione che fa si che ogni blocco segua SOLO le direzioni di cui è terminale
std::vector<std::vector<std::vector<int>>> hyper_constrains (std::vector<std::vector<int>> constrains,int cell_number, int dimension){
    int tot_dim=int_pow(cell_number,dimension);
    std::vector<std::vector<std::vector<int>>> result;
    std::vector<std::vector<int>> directions;
    for (int i=0; i<tot_dim; i++) {
        directions.clear();
        // escludo le celle interne
        if(!element_verify(i,constrains,cell_number,dimension)) {
             std::vector<int> empty;
            directions.push_back(empty);
        }
        //caso angoli
        else if(is_corner(cell_number,dimension,i)){
            directions = k_comb_gen(dimension); // Genero le direzioni
            directions= zero_cut(directions); // Tolgo lo zero
            for (int k=0; k<static_cast<int>(constrains.size()); k++) {
                for (size_t f=0; f<constrains[k].size(); f++) {
                    if (k_module(int_pow(cell_number,k+1-dimension),i)==constrains[k][f] && k>=dimension) {directions = j_a_remover(directions,k-dimension,1);}
                    else if (k_module(int_pow(cell_number,k+1),i)==constrains[k][f] && k<dimension){directions = j_a_remover(directions,k,-1);}
                }}
        
        }

        else {
        for (int k=0; k<static_cast<int>(constrains.size()); k++) {
                for (size_t f=0; f<constrains[k].size(); f++) {
            if (k_module(int_pow(cell_number,k+1-dimension),i)==constrains[k][f] && k>=dimension) {
            std::vector<int> direction=inv_canonic(dimension,k-dimension);
            directions.push_back(direction);}
            else if (k_module(int_pow(cell_number,k+1),i)==constrains[k][f] && k<dimension){
            std::vector<int> direction=canonic(dimension,k);
            directions.push_back(direction);} 
        }
        }}
        result.push_back(directions);
    }
    return result;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!