
#include "view.hxx"

// Convenient type aliases:
// using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 120;
static int const circle_radius_l = grid_size / 3;
static int const circle_radius_s = grid_size / 6;

// Colors
enum Color { red, green, blue };


static ge211::Color const tile_color {15, 15, 0};
static ge211::Color const dark_color {0, 0, 0};
static ge211::Color const light_color {255, 0, 0};

View::View(Model const& model)
        : model_(model),
          tile_sprite_({grid_size, grid_size}, tile_color),
          vert_conn_sprite_({grid_size/4, grid_size}, light_color),
          horiz_conn_sprite_({grid_size, grid_size/4}, light_color),
          dark_sprite_(circle_radius_l, dark_color),
          light_sprite_(circle_radius_l, light_color)
// You may want to add sprite initialization here
{ }

void View::draw(Sprite_set& set, Position posn)
{
    for (auto p : model_.board()) {
        set.add_sprite(tile_sprite_, board_to_screen(p), 0);
    }
    for (int i = 0; i < model_.dims().width; i++) {
        for (int j = 0; j < model_.dims().height; j++) {
            switch(model_.endpts_[i][j]) {
                case 1:
                    set.add_sprite(red_, mid_bts({i,j}), 2);
                    break;
                case 2:
                    set.add_sprite(green_, mid_bts({i,j}), 2);
                    break;
                case 3:
                    set.add_sprite(blue_, mid_bts({i,j}), 2);
                    break;
            }
        }
    }

    set.add_sprite(light_sprite_, mid_bts({0,0}), 1);
    set.add_sprite(light_sprite_, mid_bts({3,3}), 1);
    for(int i = 0; i < model_.vert_conns_.size(); i++) {
        for(int j = 0; j < model_.vert_conns_[0].size(); j++) {
            if(model_.vert_conns_[i][j] != 0) {
                set.add_sprite(vert_conn_sprite_, vert_conn_mid_bts({i,j}), 1);
            }
        }
    }
    for(int i = 0; i < model_.horiz_conns_.size(); ++i) {
        for(int j = 0; j < model_.horiz_conns_[0].size(); ++j) {
            if(model_.horiz_conns_[i][j] != 0) {
                set.add_sprite(horiz_conn_sprite_, horiz_conn_mid_bts({i,j}),
                               1);
            }
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return ge211::Dims<int>((grid_size + 1) * model_.endpts_[0].size() + 1,
                            (grid_size + 1) * model_.endpts_.size() + 70);
}

std::string
View::initial_window_title() const
{
    return "Flow Free";
}

View::Position
View::board_to_screen(Model::Position logical) const
{
    return View::Position{(1 + logical.x) + grid_size * logical.x,
                          (1 + logical.y) + grid_size * logical.y};
}

View::Position
View::mid_bts(Model::Position logical) const
{
    return board_to_screen(logical).down_right_by({grid_size/6, grid_size/6});
}

View::Position
View::vert_conn_mid_bts(Model::Position logical) const
{
    return board_to_screen(logical).down_right_by({grid_size*3/8, grid_size/2});
}

View::Position
View::horiz_conn_mid_bts(Model::Position logical) const
{
    return board_to_screen(logical).down_right_by({grid_size/2, grid_size*3/8});
}

Model::Position
View::screen_to_board(View::Position physical) const
{
    return Model::Position{int(std::floor(physical.x / grid_size)),
                           int(std::floor(physical.y / grid_size))};
}