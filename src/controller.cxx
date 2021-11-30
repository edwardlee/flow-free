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
          orig_ {0},
          completed_ {0},
          moves_ {0}
{ }

void Controller::cleanup(int c) {
    if(c != orig_) {
        for (auto& row: model_.horiz_conns)
            for (auto& elem: row)
                if (elem == c)
                    elem = 0;
        for (auto& row: model_.vert_conns)
            for (auto& elem: row)
                if (elem == c)
                    elem = 0;
    }
}

void
Controller::overlap(ge211::Posn<int> p, bool dir)
{
    if (dir) {
        if (p.x < model_.dims().width - 1) {
            if(model_.horiz_conns[p.x][p.y] != 0) {
                cleanup(model_.horiz_conns[p.x][p.y]);
            } else if(p.x > 0 && model_.horiz_conns[p.x - 1][p.y] != 0) {
                cleanup(model_.horiz_conns[p.x - 1][p.y]);
            }
        }
    } else {
        if (model_.vert_conns[p.x][p.y] != 0) {
            cleanup(model_.vert_conns[p.x][p.y]);
        } else if(p.y > 0 && model_.vert_conns[p.x][p.y - 1] != 0) {
            cleanup(model_.vert_conns[p.x][p.y - 1]);
        }
    }
}


void
Controller::on_mouse_move(ge211::Posn<int> mp)
{
    ge211::Posn<int> p = View::screen_to_board(mp);
    if (moving_ && p.x >= 0 && p.x < model_.dims().width && p.y >= 0 && p.y <
        model_.dims().height && (!model_.endpts[p.x][p.y] || (model_
                                .endpts[p.x][p.y] == orig_ && p != ori_posn_))) {
        if (p.x - cur_posn_.x == 1 && p.y == cur_posn_.y) {
            overlap(p, false);
            model_.horiz_conns[cur_posn_.x][p.y] = orig_;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == 1 && p.x == cur_posn_.x) {
            overlap(p, true);
            model_.vert_conns[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if (p.x - cur_posn_.x == -1 && p.y == cur_posn_.y) {
            overlap(p, false);
            model_.horiz_conns[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == -1 && p.x == cur_posn_.x) {
            overlap(p, true);
            model_.vert_conns[cur_posn_.x][p.y] = orig_;
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
        cleanup(model_.endpts[bp.x][bp.y]);
        orig_ = model_.endpts[bp.x][bp.y];
        ori_posn_ = bp;
    }
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    moving_ = false;
    if(model_.endpts[cur_posn_.x][cur_posn_.y] == orig_ && cur_posn_ != ori_posn_) {
        ge211::audio::Sound_effect s1("pitch" + std::to_string
        (++completed_) + ".mp3",
                                      mixer());
        mixer().play_effect(s1);
    }
    orig_ = -1;
    cur_posn_ = {-1, -1};
    if(completed_ > 8)
        completed_ = 0;
    auto sans52 = ge211::Font("sans.ttf", 52);
    auto builder = ge211::Text_sprite::Builder(sans52);
    builder.color(ge211::Color::black());
    builder << "Moves  " << ++moves_;
    view_.moves.reconfigure(builder);
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
        completed_ = 0;
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
    }
}

void
Controller::on_start()
{
    ge211::Abstract_game::background_color = ge211::Color::white();

    int d = model_.dims().height;

    std::ifstream file("../Resources/levels" + std::to_string(d)
                       + std::to_string(d) + ""
                                             ".txt");
    ge211::Random_source<int> die(0, 9);
    int level = die.next();
    std::string row;
    for (int _ = 0; _ < level * (d + 1); ++_) {
        getline(file, row);
    }

    for (int i = 0; i < d; ++i) {
        getline(file, row);
        std::stringstream ss(row);
        int c;
        for (int j = 0; j < d; ++j) {
            ss >> c;
            model_.endpts[i][j] = c;
        }
    }
    file.close();
}
