
#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          moving{false},
          mouse_posn_{0, 0}
{ }

void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    mouse_posn_ = position;
    if (moving) {
        // model_.evaluate_position_(position);
    }
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> p)
{
    ge211::Posn<int> bp = view_.screen_to_board(mouse_posn_);
    if (model_.board_[bp.x][bp.y] == 1) {
        moving = true;
        ge211::Abstract_game::background_color = {255, 200, 200};
    }
}
void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    moving = false;
    ge211::Abstract_game::background_color = {255, 255, 255};
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_posn_);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void Controller::on_start()
{
    ge211::Abstract_game::background_color = {255, 255, 255};
    model_.board_[0][0] = 1;
    model_.board_[3][3] = 1;
}
