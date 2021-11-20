
#include "view.hxx"

// Convenient type aliases:
// using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;
static int const circle_radius_l = grid_size / 2;
static int const circle_radius_s = grid_size / 4;

// Colors
enum Color { red, green, blue };


static ge211::Color const tile_color {0, 255, 0};
static ge211::Color const dark_color {0, 0, 0};
static ge211::Color const light_color {255, 255, 255};

View::View(Model const& model)
        : model_(model),
          tile_sprite_({grid_size, grid_size}, tile_color),
          dark_sprite_(circle_radius_l, dark_color),
          light_sprite_(circle_radius_l, light_color)
// You may want to add sprite initialization here
{ }

void View::draw(Sprite_set& set, Position posn)
{
    for (auto p : model_.board()) {
        set.add_sprite(tile_sprite_, board_to_screen(p), 0);
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Flow";
}

View::Position
View::board_to_screen(Model::Position logical) const
{
    return View::Position{logical.x + grid_size * logical.x,
                          logical.y + grid_size * logical.y};
}

Model::Position
View::screen_to_board(View::Position physical) const
{
    return Model::Position{int(std::floor(physical.x / grid_size)),
                           int(std::floor(physical.y / grid_size))};
}