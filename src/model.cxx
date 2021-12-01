#include <algorithm>
#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : endpts(height, std::vector<int>(width, 0)),
          horiz_conns(width - 1, std::vector<int>(height, 0)),
          vert_conns(width, std::vector<int>(height - 1, 0)),
          orig{},
          completed{},
          die(0, 9),
          dims_(width, height)
{ }

Model::Dimensions
Model::dims() const
{
    return dims_;
}

void
Model::reset()
{
    for (auto& elem: horiz_conns) {
        std::fill(elem.begin(), elem.end(), 0);
    }
    for (auto& elem: vert_conns) {
        std::fill(elem.begin(), elem.end(), 0);
    }
    for(auto& p : matched)
        p.second = false;
}



void
Model::cleanup(int c)
{
    if(c != orig) {
        for (auto& row: horiz_conns)
            for (auto& elem: row)
                if (elem == c)
                    elem = 0;
        for (auto& row: vert_conns)
            for (auto& elem: row)
                if (elem == c)
                    elem = 0;

        if(matched[c]) {
            --completed;
            if(completed < 0) completed = 0;
            matched[c] = false;
        }
    }
}

void
Model::overlap(ge211::Posn<int> p, bool dir)
{
    if (dir) {
        if (p.x < dims_.width - 1) {
            if(horiz_conns[p.x][p.y] != 0) {
                cleanup(horiz_conns[p.x][p.y]);
            } else if(p.x > 0 && horiz_conns[p.x - 1][p.y] != 0) {
                cleanup(horiz_conns[p.x - 1][p.y]);
            }
        }
    } else {
        if (vert_conns[p.x][p.y] != 0) {
            cleanup(vert_conns[p.x][p.y]);
        } else if(p.y > 0 && vert_conns[p.x][p.y - 1] != 0) {
            cleanup(vert_conns[p.x][p.y - 1]);
        }
    }
}

bool
Model::solved() const
{
//    return completed == matched.size();
    return std::all_of(matched.begin(), matched.end(), [](std::pair<int, bool> p){
        return p.second || p.first < 1 || p.first > 9;
    });
}

void Model::instantiate() {
    matched.clear();
    int d = dims_.height;

    std::ifstream file("../Resources/levels" + std::to_string(d)
                       + std::to_string(d) + ".txt");
    int level = die.next();
    std::string row;
    for (int _ = 0; _ < level * (d + 1); ++_) {
        getline(file, row);
    }

    for (int i = 0; i < d; ++i) {
        getline(file, row);
        std::stringstream ss(row);
        int c;
        for (int j = 0; j < d; ++j) {
            ss >> c;
            endpts[i][j] = c;
            if(c > 0 && c < 10) matched[c] = false;
        }
    }
    file.close();
}