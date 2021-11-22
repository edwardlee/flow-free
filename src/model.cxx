
#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : endpts_(height, std::vector<int>(width, 0)),
          horiz_conns_(width - 1, std::vector<int>(height, 0)),
          vert_conns_(width, std::vector<int>(height - 1, 0)),
          dims_(width, height)
{
}

Board::Rectangle
Model::board() const
{
    return Board::Rectangle::from_top_left(the_origin, dims_);
}

// bool
// Model::evaluate_position_(Position pos) const
// {
//     if(board_[pos] != -1)
//         return true;
//     return board_[pos] == -1;
// }