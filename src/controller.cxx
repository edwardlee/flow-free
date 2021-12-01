#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          moving_ {false},
          ori_posn_ {-1,-1},
          cur_posn_ {-1, -1},
          moves_ {0}
{ }


void
Controller::on_mouse_move(ge211::Posn<int> mp)
{
    ge211::Posn<int> p = View::screen_to_board(mp);
    if (moving_ && p.x >= 0 && p.x < model_.dims().width && p.y >= 0 && p.y <
        model_.dims().height && (!model_.endpts[p.x][p.y] || (model_
                                .endpts[p.x][p.y] == model_.orig && p != ori_posn_))) {
        if (p.x - cur_posn_.x == 1 && p.y == cur_posn_.y) {
            model_.overlap(p, false);
            model_.horiz_conns[cur_posn_.x][p.y] = model_.orig;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == 1 && p.x == cur_posn_.x) {
            model_.overlap(p, true);
            model_.vert_conns[p.x][cur_posn_.y] = model_.orig;
            cur_posn_ = p;
        } else if (p.x - cur_posn_.x == -1 && p.y == cur_posn_.y) {
            model_.overlap(p, false);
            model_.horiz_conns[p.x][cur_posn_.y] = model_.orig;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == -1 && p.x == cur_posn_.x) {
            model_.overlap(p, true);
            model_.vert_conns[cur_posn_.x][p.y] = model_.orig;
            cur_posn_ = p;
        }
    }
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> p)
{
    ge211::Posn<int> bp = View::screen_to_board(p);
    cur_posn_ = bp;
    if (model_.endpts[bp.x][bp.y] != 0) {
        moving_ = true;
        model_.cleanup(model_.endpts[bp.x][bp.y]);
        model_.orig = model_.endpts[bp.x][bp.y];
        ori_posn_ = bp;
    }
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    moving_ = false;
    if(model_.endpts[cur_posn_.x][cur_posn_.y] == model_.orig && cur_posn_ != ori_posn_) {
        ge211::audio::Sound_effect s1("pitch" + std::to_string
        (++model_.completed) + ".mp3",
                                      mixer());
        mixer().play_effect(s1);
        model_.matched[model_.orig] = true;
    }
    model_.orig = -1;
    cur_posn_ = {-1, -1};

    auto sans42 = ge211::Font("sans.ttf", 42);
    auto builder = ge211::Text_sprite::Builder(sans42);
    builder.color(ge211::Color::black());
    builder << "Moves  " << ++moves_;
    view_.moves.reconfigure(builder);


    if(model_.solved()) {
        ge211::Abstract_game::background_color = ge211::Color::black();
        builder.color(ge211::Color::white());
        builder << "   Time  " << ttc_.elapsed_time().seconds();
        view_.moves.reconfigure(builder);
    }

    if(model_.completed > 9)
        model_.completed = 0;
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return View::initial_window_title();
}

void
Controller::on_key(ge211::events::Key key)
{
    if (key == ge211::Key::code('r')) {
        moving_ = false;
        cur_posn_ = {-1, -1};
        model_.completed = 0;
        moves_ = 0;
        for (auto& elem: model_.horiz_conns) {
            std::fill(elem.begin(), elem
                    .end(), 0);
        }
        for (auto& elem: model_.vert_conns) {
            std::fill(elem.begin(), elem
                    .end(), 0);
        }

        auto sans52 = ge211::Font("sans.ttf", 52);
        auto builder = ge211::Text_sprite::Builder(sans52);
        builder.color(ge211::Color::black());
        builder << "Moves";
        view_.moves.reconfigure(builder);
        ge211::Abstract_game::background_color = ge211::Color::white();
        ttc_ = ge211::Timer();
    }
}

void
Controller::on_start()
{
    ge211::Abstract_game::background_color = ge211::Color::white();
    model_.instantiate();
    ttc_ = ge211::Timer();
}