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

    View::Position board_to_screen(Model::Position logical) const;
    View::Position mid_bts(Model::Position logical) const;

    Model::Position screen_to_board(View::Position physical) const;

private:
    Model const& model_;
    ge211::Rectangle_sprite tile_sprite_;
    ge211::Circle_sprite dark_sprite_;
    ge211::Circle_sprite light_sprite_;
};