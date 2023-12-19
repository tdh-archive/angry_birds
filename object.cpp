#include <cmath>
#include <csignal>
#include <ncurses.h>

#include "object.h"

void Object::draw() {
    if (this->_destroyed) {
        return;
    }

    // Get the height of the window
    int height = getmaxy(this->_win) - 1;

    // Draw the object
    mvwaddwstr(this->_win, height - (this->_y + 1), this->_x, this->_asset);
}

void Object::destroy() {
    this->_destroyed = true;

    int height = getmaxy(this->_win) - 1;

    mvwaddwstr(this->_win, height - (this->_y + 1), this->_x, L"💥");
    wrefresh(this->_win);
    sleep(1);
    mvwaddwstr(this->_win, height - (this->_y + 1), this->_x, L"  ");
    wrefresh(this->_win);
}

void Object::move(int x, int y) {
    // Get the height of the window
    int height = getmaxy(this->_win) - 1;

    // Clear the previous position
    mvwaddwstr(this->_win, height - (this->_y + 1), this->_x, L" ");

    // Update the position
    this->_x += x;
    this->_y += y;

    // Draw the object
    this->draw();
}

void Object::set_position(int x, int y) {
    // Get the height of the window
    int height = getmaxy(this->_win) - 1;

    // Clear the previous position
    mvwaddwstr(this->_win, height - (this->_y + 1), this->_x, L" ");

    // Update the position
    this->_x = x;
    this->_y = y;

    // Draw the object
    this->draw();
}

double Object::distance(int x, int y) const noexcept {
    if (this->_destroyed) {
        return false;
    }

    int x_diff = abs(x - this->x());
    int y_diff = abs(y - this->y());

    double distance = sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));

    return distance;
}
