#ifndef ANGRY_BIRDS__OBJECT_H
#define ANGRY_BIRDS__OBJECT_H

#include <map>
#include <ncurses.h>
#include <string>

// Game Object Types
enum ObjectType : char {
    PIG_SMALL = 'p', // Small pig
    // PIG_BIG = 'P',   // Big pig
    BLOCK_H = 'b', // Horizontal block (breakable)
    BLOCK_V = 'B', // Vertical block (breakable)
    STONE = 'S',   // Unbreakable stone
    /* Birds */
    BIRD = 'r', // Bird
};

// Game Object Assets. Emoji has a width of 2, so we need to use a wide string.
const std::map<ObjectType, const wchar_t *> OBJECT_ASSETS = {
    {PIG_SMALL, L"🐷"},
    // {PIG_BIG, L"🐗"},
    {BLOCK_H, L"__"},
    {BLOCK_V, L"##"},
    {STONE, L"@@"},
    /* Birds */
    {BIRD, L"🐥"},
};

class Object {
  public:
    Object(WINDOW *parent,
           ObjectType type,
           int x, int y,
           bool breakable = false)
        : _win(parent), _type(type), _x(x), _y(y), _breakable(breakable) {
        this->_asset = OBJECT_ASSETS.at(type);
    }

    virtual void draw();

    virtual void destroy();

    virtual void move(int x, int y);

    virtual void set_position(int x, int y);

    double distance(int x, int y) const noexcept;

    ObjectType type() const noexcept { return _type; }

    int x() const noexcept { return _x; }
    int y() const noexcept { return _y; }

    bool destroyed() const noexcept { return _destroyed; }
    bool breakable() const noexcept { return _breakable; }

  private:
    WINDOW *_win;
    ObjectType _type;
    const wchar_t *_asset;

    int _x;
    int _y;

    bool _destroyed;
    bool _breakable;
};

class PigSmall : public Object {
  public:
    PigSmall(WINDOW *parent, int x, int y)
        : Object(parent, ObjectType::PIG_SMALL, x, y, true) {
    }
};

class BlockH : public Object {
  public:
    BlockH(WINDOW *parent, int x, int y)
        : Object(parent, ObjectType::BLOCK_H, x, y, true) {
    }
};

class BlockV : public Object {
  public:
    BlockV(WINDOW *parent, int x, int y)
        : Object(parent, ObjectType::BLOCK_V, x, y, true) {
    }
};

class Stone : public Object {
  public:
    Stone(WINDOW *parent, int x, int y)
        : Object(parent, ObjectType::STONE, x, y, false) {
    }
};

class Bird : public Object {
  public:
    Bird(WINDOW *parent, int x, int y)
        : Object(parent, ObjectType::BIRD, x, y, false) {
    }
};

#endif //ANGRY_BIRDS__OBJECT_H
