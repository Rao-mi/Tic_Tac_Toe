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
    void setgrid(std::vector<state> source);
    bool Move(state player);
    bool AI_Move();
    std::vector<std::vector<std::vector<state>>> all_plays();
    std::vector<double> Entropy ();
    void print2D();
};
bool areVectorsEquivalent(const std::vector<state>& vec1, const std::vector<state>& vec2);
#endif