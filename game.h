#ifndef ANGRY_BIRDS__GAME_H
#define ANGRY_BIRDS__GAME_H

#include "stage.h"

#define STAGE_HEIGHT 20

class Game {
  public:
    Game(int width, int height);

    ~Game();

    /// Load and draw the stage
    void load_stage(int level);

    /// Draw the status window
    void draw_status() const noexcept;


  private:
    /// Handle user input in the game
    void _user_input() noexcept;

    /// Show the game win screen
    void _game_win() const noexcept;

    /// Show the game over screen
    void _game_over() const noexcept;

    int _width;
    int _height;

    Stage *_stage;

    WINDOW *_win_stage;
    WINDOW *_win_status;
};

#endif //ANGRY_BIRDS__GAME_H
