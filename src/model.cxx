#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : endpts(height, std::vector<int>(width, 0)),
          horiz_conns(width - 1, std::vector<int>(height, 0)),
          vert_conns(width, std::vector<int>(height - 1, 0)),
          dims_(width, height)
{ }

Model::Dimensions
Model::dims() const
{
    return dims_;
}