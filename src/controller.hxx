#pragma once

#include "model.hxx"
#include "view.hxx"

/// The main game class for Flow Free.
class Controller : public ge211::Abstract_game
{
public:

    /// Constructs a game with `size` as both its width and height.
    ///
    /// ERRORS:
    ///  - Throws `ge211::Client_logic_error` if `size` is less than 4
    ///    or greater than 8.
    explicit Controller(int size = 8);

    /// Constructs a game with the given width and height.
    ///
    /// ERRORS:
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    Controller(int width, int height);

    /// `protected` means that GE211 (via base class `ge211::Abstract_game`)
    /// can access these members, but arbitrary other code cannot:
protected:
    ///
    /// Controller operations called by GE211
    ///
    void on_mouse_move(ge211::Posn<int>) override;
    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_key(ge211::events::Key key) override;
    void on_start() override;

    /// These three delegate to the view:
    void draw(ge211::Sprite_set&) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

private:
    Model model_;
    View  view_;
    bool moving_;
    View::Position ori_posn_;
    View::Position cur_posn_;
    int moves_;
    ge211::Timer ttc_;
};