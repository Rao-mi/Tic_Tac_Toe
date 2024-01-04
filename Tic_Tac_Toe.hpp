#ifndef TIC_TAC_TOE
#define TIC_TAC_TOE

#include <vector>


enum class state {X,O,N};

class Board {
    private:
    int cell_number; // di default sarebbe 3 
    int dimension; // proviamo a generalizzare un po' (3x3 opp. 3x3x3 ecc)
    std::vector<state> grid;
    public:
    Board(int set_cell_n, int set_dim);
    bool Move(state player);
    bool AI_Move();
    bool AI_Ising_Move();
    state Arena(char first, char second);
    state Arena(char first, char second, double b);
    void print2D();
    std::vector<std::vector<std::vector<state>>> all_plays();   
    char End();
    void setgrid(std::vector<state> source);
};
bool areVectorsEquivalent(const std::vector<state>& vec1, const std::vector<state>& vec2);
#endif