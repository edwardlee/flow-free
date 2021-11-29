#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    void draw(ge211::Sprite_set& set, Position posn);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    /// Converts positions on the board to pixel positions.
    View::Position board_to_screen(Model::Position logical) const;

    /// Converts pixel positions to positions on the board.
    Model::Position screen_to_board(View::Position physical) const;
    View::Position vert_conn_mid_bts(Model::Position logical) const;
    View::Position horiz_conn_mid_bts(Model::Position logical) const;
    View::Position mid_bts(Model::Position logical) const;

private:
    Model const& model_;
    ge211::Rectangle_sprite tile_sprite_;
    std::vector<ge211::Rectangle_sprite> vert_conns_;
    std::vector<ge211::Rectangle_sprite> horiz_conns_;
    std::vector<ge211::Circle_sprite> endpts_;
    std::vector<ge211::Circle_sprite> corners_;
    ge211::Text_sprite moves_;
};