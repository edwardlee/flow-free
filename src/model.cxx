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

Model::Dimensions
Model::dims() const
{
    return dims_;
}

Model::TwoVec
Model::endpts() const
{
    return endpts_;
}

Model::TwoVec
Model::horiz_conns() const
{
    return horiz_conns_;
}

Model::TwoVec
Model::vert_conns() const
{
    return vert_conns_;
}

void
Model::place_conn(Position p)
{
    // TODO
}