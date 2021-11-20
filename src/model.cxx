
#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    // TODO
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

// bool
// Model::evaluate_position_(Position pos) const
// {
//     if(board_[pos] != -1)
//         return true;
//     return board_[pos] == -1;
// }