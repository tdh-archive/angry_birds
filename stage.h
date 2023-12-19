#ifndef ANGRY_BIRDS__STAGE_H
#define ANGRY_BIRDS__STAGE_H

#include <ncurses.h>
#include <vector>

#include "object.h"

class Stage {
  public:
    Stage(WINDOW *win, int level);
    ~Stage();

    /// Draw all objects in the stage
    void draw() const noexcept;

    /// Draw the slingshot
    void draw_sling() const noexcept;

    /// Fire the bird
    void fire_bird(int o_x, int o_y, double v0, double rad) noexcept;

    /// Get collision object with the given coordinates
    Object *collision(int x, int y) noexcept;

    /// Get remaining birds
    int remaining_birds() const noexcept { return this->_remaining_birds; }
    /// Get remaining pigs
    int remaining_pigs() const noexcept { return this->_remaining_pigs; }
    /// Get the level number
    int level() const noexcept { return this->_level; }

    /// Get the bird
    Bird *bird() noexcept { return this->_bird; }
    /// Set the bird
    void set_bird(Bird *bird) noexcept { this->_bird = bird; }

  private:
    int _remaining_birds;
    int _remaining_pigs;
    int _level;

    Bird *_bird;
    std::vector<Object*> _objects;
    WINDOW *_win;
};

#endif //ANGRY_BIRDS__STAGE_H
