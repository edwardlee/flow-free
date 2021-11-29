
#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          moving_{false},
          mouse_posn_{0, 0},
          cur_posn_{-1, -1}
{ }

void
Controller::on_mouse_move(ge211::Posn<int> mp)
{
    ge211::Posn<int> p = view_.screen_to_board(mp);
    if (moving_ && p.x >= 0 && p.x < model_.dims().width && p.y >= 0 && p.y <
            model_.dims().height && (!model_.endpts_[p.x][p.y] || model_
            .endpts_[p.x][p.y] == orig_)) {
        if(p.x - cur_posn_.x == 1 && p.y == cur_posn_.y) {
            model_.horiz_conns_[cur_posn_.x][p.y] = orig_;
            cur_posn_ = p;
        } else if(p.y - cur_posn_.y == 1 && p.x == cur_posn_.x) {
            model_.vert_conns_[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if(p.x - cur_posn_.x == -1 && p.y == cur_posn_.y) {
            model_.horiz_conns_[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if(p.y - cur_posn_.y == -1 && p.x == cur_posn_.x) {
            model_.vert_conns_[cur_posn_.x][p.y] = orig_;
            cur_posn_ = p;
        }
    }
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> p)
{
    ge211::Posn<int> bp = view_.screen_to_board(p);
    if (model_.endpts_[bp.x][bp.y] != 0) {
        moving_ = true;
        cur_posn_ = bp;
        orig_ = model_.endpts_[bp.x][bp.y];
        for(auto &row : model_.horiz_conns_)
            for(auto &elem : row)
                if(elem == model_.endpts_[bp.x][bp.y])
                    elem = 0;
        for(auto &row : model_.vert_conns_)
            for(auto &elem : row)
                if(elem == model_.endpts_[bp.x][bp.y])
                    elem = 0;
    }
}
void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    moving_ = false;
    cur_posn_ = {-1, -1};
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

void Controller::on_key(ge211::events::Key key)
{
    if(key == ge211::Key::code('r')) {
        moving_ = false;
        cur_posn_ = {-1, -1};
        for(auto &elem : model_.horiz_conns_) std::fill(elem.begin(), elem
        .end(), 0);
        for(auto &elem : model_.vert_conns_) std::fill(elem.begin(), elem
        .end(), 0);
    }
}

void Controller::on_start()
{
    ge211::Abstract_game::background_color = ge211::Color::white();
    model_.endpts_[0][0] = 1;
    model_.endpts_[1][1] = 2;
    model_.endpts_[2][1] = 2;
    model_.endpts_[3][3] = 1;
    model_.endpts_[0][1] = 3;
    model_.endpts_[1][2] = 3;
    model_.endpts_[0][3] = 5;
    model_.endpts_[2][2] = 5;
}
