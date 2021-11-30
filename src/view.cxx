#include "view.hxx"

using Sprite_set = ge211::Sprite_set;

static int const grid_size = 80;
static int const circle_radius_l = grid_size / 3;
static int const circle_radius_s = grid_size / 8;

// Colors
enum Color
{
    red, blue, green, yellow, orange, teal, pink, purple, white
};
static ge211::Color const tile_color {15, 15, 0};

View::View(Model const& model)
        : moves {},
          model_(model),
          tile_sprite_({grid_size, grid_size}, tile_color)
{
    auto* colors = new ge211::Color[9];
    colors[red] = {255,0,0};
    colors[blue] = {0,0,255};
    colors[green] = {0,255,0};
    colors[yellow] = {255,255,0};
    colors[orange] = {255,128,0};
    colors[teal] = {0,128,128};
    colors[pink] = {255,192,203};
    colors[purple] = {128,0,128};
    colors[white] = {255,255,255};

    for(int i = red; i <= white; ++i) {
        corners_.push_back(ge211::Circle_sprite(circle_radius_s, colors[i]));
        horiz_conns_.push_back(ge211::Rectangle_sprite({grid_size, grid_size
        / 4}, colors[i]));
        vert_conns_.push_back(ge211::Rectangle_sprite({grid_size / 4,
                                                       grid_size}, colors[i]));
        endpts_.push_back(ge211::Circle_sprite(circle_radius_l, colors[i]));
    }

    auto sans52 = ge211::Font("sans.ttf", 52);
    auto builder = ge211::Text_sprite::Builder(sans52);
    builder.color(ge211::Color::black());
    builder << "Moves";
    moves.reconfigure(builder);
}

void
View::draw(Sprite_set &set)
{
    for (int i = 0; i < model_.dims().width; i++) {
        for (int j = 0; j < model_.dims().height; j++) {
            set.add_sprite(tile_sprite_, board_to_screen({i, j}), 0);
        }
    }
    for (int i = 0; i < model_.dims().width; i++) {
        for (int j = 0; j < model_.dims().height; j++) {
            if(model_.endpts[i][j])
                set.add_sprite(endpts_[model_.endpts[i][j] - 1], mid_bts({i, j}),
                           2);
        }
    }

    for (int i = 0; i < model_.vert_conns.size(); i++) {
        for (int j = 0; j < model_.vert_conns[0].size(); j++) {
            if (model_.vert_conns[i][j] != 0) {
                size_t c = model_.vert_conns[i][j] - 1;
                set.add_sprite(vert_conns_[c], vert_conn_mid_bts({i, j}), 1);
                set.add_sprite(corners_[c], vert_conn_mid_bts({i, j})
                .up_by(grid_size/8), 1);
                set.add_sprite(corners_[c], vert_conn_mid_bts({i, j+1}).up_by
                (grid_size/8), 1);
            }
        }
    }
    for (int i = 0; i < model_.horiz_conns.size(); ++i) {
        for (int j = 0; j < model_.horiz_conns[0].size(); ++j) {
            if (model_.horiz_conns[i][j] != 0) {
                size_t c = model_.horiz_conns[i][j] - 1;
                set.add_sprite(horiz_conns_[c], horiz_conn_mid_bts({i, j}),
                               1);
                set.add_sprite(corners_[c], horiz_conn_mid_bts({i, j})
                .left_by(grid_size/8), 1);
                set.add_sprite(corners_[c], horiz_conn_mid_bts({i+1, j})
                .left_by(grid_size/8), 1);
            }
        }
    }

    set.add_sprite(moves, {0, grid_size * model_.dims().width}, 3);
}

View::Dimensions
View::initial_window_dimensions() const
{
    return {(grid_size + 1) * (int) model_.endpts[0].size() + 1,
            (grid_size + 1) * (int) model_.endpts.size() + 70};
}

std::string
View::initial_window_title()
{
    return "Flow Free";
}

View::Position
View::board_to_screen(Model::Position logical)
{
    return View::Position {(1 + logical.x) + grid_size * logical.x,
                           (1 + logical.y) + grid_size * logical.y};
}

View::Position
View::mid_bts(Model::Position logical)
{
    return board_to_screen(logical).down_right_by({grid_size / 6,
                                                   grid_size / 6});
}

View::Position
View::vert_conn_mid_bts(Model::Position logical)
{
    return board_to_screen(logical).down_right_by({grid_size * 3 / 8,
                                                   grid_size / 2});
}

View::Position
View::horiz_conn_mid_bts(Model::Position logical)
{
    return board_to_screen(logical).down_right_by({grid_size / 2,
                                                   grid_size * 3 / 8});
}

Model::Position
View::screen_to_board(View::Position physical)
{
    return Model::Position {int(std::floor(physical.x / grid_size)),
                            int(std::floor(physical.y / grid_size))};
}