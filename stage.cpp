#include <cmath>
#include <fcntl.h>
#include <string>
#include <unistd.h>

#include "game.h"
#include "object.h"
#include "stage.h"

Stage::Stage(WINDOW *win, int level) : _win(win), _level(level) {
    // Load the stage from a csv file
    std::string filename = "stages/stage" + std::to_string(_level) + ".csv";

    // Read the file
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        filename = "../stages/stage" + std::to_string(_level) + ".csv";
        fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            wprintf(L"Could not open file %s\n", filename.c_str());
            exit(EXIT_FAILURE);
        }
    }

    char buffer[32];
    char c;

    // Read the first line
    int i = 0;
    while (read(fd, &c, 1) > 0 && c != '\n') {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    this->_remaining_birds = (int) strtol(buffer, nullptr, 10);

    // Ignore the second line (CSV header)
    while (read(fd, &c, 1) > 0 && c != '\n') {
        // Do nothing
    }

    // Parse the stage file
    char type;
    int x, y;
    while (read(fd, &c, 1) > 0) {
        // The first character is the object type
        type = c;

        // The next character should be a comma
        if (read(fd, &c, 1) < 0 || c != ',') {
            wprintf(L"Invalid stage file %s\n", filename.c_str());
            exit(EXIT_FAILURE);
        }

        // The next characters are the x coordinate
        i = 0;
        while (read(fd, &c, 1) > 0) {
            if (c == ',') {
                break; // End of x
            }
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        x = (int) strtol(buffer, nullptr, 10);

        // The next characters are the y coordinate
        i = 0;
        while (read(fd, &c, 1) > 0) {
            if (c == '\n') {
                break; // End of y
            }
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        y = (int) strtol(buffer, nullptr, 10);

        // Create the object
        Object *obj;
        switch (type) {
        case 'p':
            obj = new PigSmall(win, x, y);
            break;
        case 'b':
            obj = new BlockH(win, x, y);
            break;
        case 'B':
            obj = new BlockV(win, x, y);
            break;
        case 'S':
            obj = new Stone(win, x, y);
            break;
        default:
            wprintf(L"Invalid stage file %s\n", filename.c_str());
            exit(EXIT_FAILURE);
        }
        this->_objects.emplace_back(obj);
    }
    this->_objects.shrink_to_fit();
    close(fd);

    this->_remaining_pigs = 0;
    for (auto obj : this->_objects) {
        ObjectType _type = obj->type();
        if (_type == ObjectType::PIG_SMALL) {
            ++this->_remaining_pigs;
        }
    }
}

Stage::~Stage() {
    for (auto obj : this->_objects) {
        delete obj;
    }
}

void Stage::draw() const noexcept {
    for (auto obj : this->_objects) {
        obj->draw();
    }
    wrefresh(this->_win);
}

void Stage::draw_sling() const noexcept {
    // Draw the slingshot
    mvwaddwstr(this->_win, STAGE_HEIGHT - 1, 15, L"||");
    mvwaddwstr(this->_win, STAGE_HEIGHT - 2, 15, L"||");
    mvwaddwstr(this->_win, STAGE_HEIGHT - 3, 15, L"||");
    mvwaddwstr(this->_win, STAGE_HEIGHT - 4, 13, L"\\\\");
    mvwaddwstr(this->_win, STAGE_HEIGHT - 4, 17, L"//");
    wrefresh(this->_win);
}

void Stage::fire_bird(const int o_x, const int o_y, const double v0, const double rad) noexcept {
    --this->_remaining_birds;

    double t = 0.0;
    double dt = 0.1;

    int x;
    int y;

    Object *collision_object = nullptr;

    while (collision_object == nullptr) {
        // Sleep for 0.1 seconds
        usleep(100000);

        // Compute the new position
        x = o_x + (int) (v0 * t * cos(rad));
        y = o_y + (int) (v0 * t * sin(rad) - 0.5 * 9.81 * t * t);

        if (x < 0 || y < 0) {
            // Get current cursor position
            int cur_y, cur_x;
            getyx(this->_win, cur_y, cur_x);
            mvwprintw(this->_win, cur_y, cur_x - 1, "X");
            wrefresh(this->_win);
            sleep(1);
            mvwprintw(this->_win, cur_y, cur_x - 1, "  ");
            wrefresh(this->_win);

            // Draw the slingshot
            this->draw_sling();

            // Set the stage bird
            this->_bird->set_position(15, 2);
            wrefresh(this->_win);
            return;
        }

        // Move the bird
        this->_bird->set_position(x, y);
        wrefresh(this->_win);

        // Update the time
        t += dt;

        // Check for collision
        collision_object = this->collision(x, y);
    }

    // Check if the bird hit a pig
    ObjectType _type = collision_object->type();
    if (_type == ObjectType::PIG_SMALL) {
        collision_object->destroy();
        --this->_remaining_pigs;
    } else if (collision_object->breakable()) {
        collision_object->destroy();
    }

    // Move the bird back to the slingshot
    this->_bird->set_position(15, 2);

    sleep(1);

    // Draw the slingshot
    this->draw_sling();
}

Object *Stage::collision(int x, int y) noexcept {
    for (auto obj : this->_objects) {
        if (obj->destroyed()) {
            continue;
        }

        if (obj->distance(x, y) <= 1.0) {
            return obj;
        }
    }
    return nullptr;
}
