#include "board.hxx"
#include <algorithm>

using namespace ge211;

Board::Board(Dimensions dims)
        : dims_(dims)
{
    if (dims_.width < 4 || dims_.height < 4) {
        throw Client_logic_error("Board::Board: dims too small");
    }

    if (dims_.width > 15 || dims_.height > 15) {
        throw Client_logic_error("Board::Board: dims too large");
    }
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

int
Board::operator[](Position pos) const
{
    bounds_check_(pos);
    return get_(pos);
}

static std::vector<Board::Dimensions>
build_directions()
{
    std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx || dy) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::all_directions()
{
    static std::vector<Dimensions> result = build_directions();
    return result;
}

// Board::Rectangle
// Board::all_positions() const
// {
//     return Rectangle::from_top_left(the_origin, dims_);
// }

bool
operator==(Board const& b1, Board const& b2)
{
    return b1.dims_ == b2.dims_;
}

int
Board::get_(Position pos) const
{
    // TODO
    return -1;
}

void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator!=(Board const& b1, Board const& b2)
{
    return !(b1 == b2);
}

std::ostream&
operator<<(std::ostream& os, Board const& board)
{
    Board::Dimensions dims = board.dimensions();

    for (int y = 0; y < dims.height; ++y) {
        for (int x = 0; x < dims.width; ++x) {
            os << board[{x, y}];
        }
        os << "\n";
    }

    return os;
}