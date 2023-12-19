#include <cmath>
#include <csignal>
#include <ncurses.h>

#include "game.h"

Game::Game(int width, int height) : _width(width), _height(height) {
    // Create the stage window
    this->_win_stage = newwin(STAGE_HEIGHT, this->_width, 0, 0);
    box(this->_win_stage, 0, 0);
    refresh();

    // Create the status window
    this->_win_status = newwin(
        this->_height - STAGE_HEIGHT,
        this->_width,
        STAGE_HEIGHT,
        0);
    box(this->_win_status, 0, 0);
    refresh();

    // Draw the windows
    wrefresh(this->_win_stage);
    wrefresh(this->_win_status);
}

Game::~Game() {
    delwin(this->_win_stage);
    delwin(this->_win_status);
}

void Game::load_stage(int level) {
    // Create the stage
    auto *stage = new Stage(this->_win_stage, level);
    // Draw the stage
    stage->draw();

    // Draw the slingshot
    stage->draw_sling();

    // Set the stage bird
    auto *bird = new Bird(this->_win_stage, 15, 2);
    stage->set_bird(bird);
    bird->draw();
    wrefresh(this->_win_stage);

    this->_stage = stage;

    // Draw the status
    this->draw_status();

    // Handle user input
    this->_user_input();
}

void Game::_user_input() noexcept {
    int c;

    const int bird_o_x = this->_stage->bird()->x();
    const int bird_o_y = this->_stage->bird()->y();

    int force_x = 0;
    int force_y = 0;

    while ((c = getch()) != 'q') {
        // Draw the slingshot
        this->_stage->draw_sling();
        this->_stage->bird()->draw();

        if (c == 'a') {
            if (force_x < 14) {
                ++force_x;
                this->_stage->bird()->move(-1, 0);
            }
        } else if (c == 'd') {
            if (force_x > -14) {
                --force_x;
                this->_stage->bird()->move(1, 0);
            }
        } else if (c == 'w') {
            if (force_y > -14) {
                --force_y;
                this->_stage->bird()->set_position(
                    this->_stage->bird()->x(),
                    bird_o_y - (int) (force_y / 5));
            }
        } else if (c == 's') {
            if (force_y < 14) {
                ++force_y;
                this->_stage->bird()->set_position(
                    this->_stage->bird()->x(),
                    bird_o_y - (int) (force_y / 5));
            }
        } else if (c == ' ') {
            // Fire the bird
            double angle = atan2(force_y, force_x);
            this->_stage->fire_bird(
                bird_o_x,
                bird_o_y,
                sqrt(force_x * force_x + force_y * force_y),
                angle);

            // Reset the force
            force_x = 0;
            force_y = 0;

            // Update the status
            this->draw_status();

            // Check if the game is over
            if (this->_stage->remaining_pigs() == 0) {
                this->_game_win();
                return;
            } else if (this->_stage->remaining_birds() == 0) {
                this->_game_over();
                return;
            }
        }

        // DEBUG
        mvwprintw(this->_win_stage, 1, 1, "Vx=%d, Vy=%d     ", force_x, force_y);

        // Draw the stage
        this->_stage->draw();
        wrefresh(this->_win_stage);
    }
}

void Game::draw_status() const noexcept {
    // Clear the status
    werase(this->_win_status);
    refresh();

    box(this->_win_status, 0, 0);
    refresh();

    // Draw the status
    mvwprintw(this->_win_status, 1, 1, "Level: %d", this->_stage->level());
    mvwprintw(this->_win_status,
              2,
              1,
              "Birds: %d | Pigs: %d",
              this->_stage->remaining_birds(),
              this->_stage->remaining_pigs());
    wrefresh(this->_win_status);
}

void Game::_game_over() const noexcept {
    // Clear the stage
    werase(this->_win_stage);

    // Draw the game over message
    mvwprintw(this->_win_stage,
              STAGE_HEIGHT / 2,
              (this->_width - 10) / 2,
              "You lose!");
    wrefresh(this->_win_stage);

    // Wait for the user to press a key
    getch();
}

void Game::_game_win() const noexcept {
    // Clear the stage
    werase(this->_win_stage);

    // Draw the game win message
    mvwprintw(this->_win_stage,
              STAGE_HEIGHT / 2,
              (this->_width - 10) / 2,
              "You Win!");
    wrefresh(this->_win_stage);

    // Wait for the user to press a key
    getch();
}
