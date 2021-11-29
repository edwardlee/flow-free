
#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          moving_ {false},
          mouse_posn_ {0, 0},
          cur_posn_ {-1, -1},
          completed_ {0}
{ }

void
Controller::overlap(ge211::Posn<int> p, bool dir)
{
    if (dir) {
        if (p.x < model_.dims().width - 1) {
            if(model_.horiz_conns_[p.x][p.y] != 0) {
                int c = model_.horiz_conns_[p.x][p.y];
                if(c != orig_) {
                    for (auto& row: model_.horiz_conns_) {
                        for (auto& elem: row) {
                            if (elem == c) {
                                elem = 0;
                            }
                        }
                    }
                    for (auto& row: model_.vert_conns_) {
                        for (auto& elem: row) {
                            if (elem == c) {
                                elem = 0;
                            }
                        }
                    }
                }
            }
            else if(p.x > 0 && model_.horiz_conns_[p.x - 1][p.y] != 0) {
                int c = model_.horiz_conns_[p.x - 1][p.y];
                if(c != orig_) {
                    for (auto& row: model_.horiz_conns_) {
                        for (auto& elem: row) {
                            if (elem == c) {
                                elem = 0;
                            }
                        }
                    }
                    for (auto& row: model_.vert_conns_) {
                        for (auto& elem: row) {
                            if (elem == c) {
                                elem = 0;
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (model_.vert_conns_[p.x][p.y] != 0) {
            int c = model_.vert_conns_[p.x][p.y];
            if(c != orig_) {
                for (auto& row: model_.horiz_conns_) {
                    for (auto& elem: row) {
                        if (elem == c) {
                            elem = 0;
                        }
                    }
                }
                for (auto& row: model_.vert_conns_) {
                    for (auto& elem: row) {
                        if (elem == c) {
                            elem = 0;
                        }
                    }
                }
            }
        }
        else if(p.y > 0 && model_.vert_conns_[p.x][p.y - 1] != 0) {
            int c = model_.vert_conns_[p.x][p.y - 1];
            if(c != orig_) {
                for (auto& row: model_.horiz_conns_) {
                    for (auto& elem: row) {
                        if (elem == c) {
                            elem = 0;
                        }
                    }
                }
                for (auto& row: model_.vert_conns_) {
                    for (auto& elem: row) {
                        if (elem == c) {
                            elem = 0;
                        }
                    }
                }
            }
        }
    }
}


void
Controller::on_mouse_move(ge211::Posn<int> mp)
{
    ge211::Posn<int> p = view_.screen_to_board(mp);
    if (moving_ && p.x >= 0 && p.x < model_.dims().width && p.y >= 0 && p.y <
        model_.dims().height && (!model_.endpts_[p.x][p.y] || model_
                                              .endpts_[p.x][p.y] == orig_)) {
        if (p.x - cur_posn_.x == 1 && p.y == cur_posn_.y) {
            overlap(p, false);
            model_.horiz_conns_[cur_posn_.x][p.y] = orig_;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == 1 && p.x == cur_posn_.x) {
            overlap(p, true);
            model_.vert_conns_[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if (p.x - cur_posn_.x == -1 && p.y == cur_posn_.y) {
            overlap(p, false);
            model_.horiz_conns_[p.x][cur_posn_.y] = orig_;
            cur_posn_ = p;
        } else if (p.y - cur_posn_.y == -1 && p.x == cur_posn_.x) {
            overlap(p, true);
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
        for (auto& row: model_.horiz_conns_) {
            for (auto& elem: row) {
                if (elem == model_.endpts_[bp.x][bp.y]) {
                    elem = 0;
                }
            }
        }
        for (auto& row: model_.vert_conns_) {
            for (auto& elem: row) {
                if (elem == model_.endpts_[bp.x][bp.y]) {
                    elem = 0;
                }
            }
        }
    }
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    moving_ = false;
    if(model_.endpts_[cur_posn_.x][cur_posn_.y] == orig_) {
        ge211::audio::Sound_effect s1("pitch" + std::to_string
        (++completed_) + ".mp3",
                                      mixer());
        mixer().play_effect(s1);
    }
    cur_posn_ = {-1, -1};
    if(completed_ > 8)
        completed_ = 0;
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

void
Controller::on_key(ge211::events::Key key)
{
    if (key == ge211::Key::code('r')) {
        moving_ = false;
        cur_posn_ = {-1, -1};
        for (auto& elem: model_.horiz_conns_) {
            std::fill(elem.begin(), elem
                    .end(), 0);
        }
        for (auto& elem: model_.vert_conns_) {
            std::fill(elem.begin(), elem
                    .end(), 0);
        }
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
            model_.endpts_[i][j] = c;
        }
    }
    file.close();
}
