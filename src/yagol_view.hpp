// File: yagol_view.hpp
#ifndef _YAGOL_VIEW_H_
#define _YAGOL_VIEW_H_

#include <string>
#include <cstddef>
#include <stdexcept>

namespace yagol {

//////////////////////////////////////////////////////////////////////

enum class EventType : short
{
    quit = 0,
    redraw,
    resize,
    randomize,
    stop,
    start,
    toggle,
    step,
    speed,
    change_rules,
    null,
    unknown
};

struct Event
{
    EventType type;
    int arg;

    Event(const EventType init_type = EventType::null,
               const int            init_arg  = 0)
        : type( init_type )
        , arg ( init_arg  )
    {}
};

//////////////////////////////////////////////////////////////////////

class View
{
  public:
    virtual ~View();

    virtual void set_state(size_t x, size_t y, bool state) = 0;
    void make_alive(size_t x, size_t y);
    void make_dead(size_t x, size_t y);

    virtual Event get_event() = 0;

    virtual void clear() = 0;
    virtual void show()  = 0;
    virtual void close() = 0;

    virtual void notify(std::string message, const int width = 50) = 0;

    virtual std::string prompt_for_string(std::string prompt, const unsigned int width = 50) = 0;
    virtual std::pair<int, int> prompt_for_numbers_with_slash(std::string prompt, const unsigned int width = 50);
    virtual int prompt_for_number(std::string prompt, const unsigned int width = 50) throw(std::invalid_argument);

    virtual std::pair<int, int> get_size() const = 0;
};

//////////////////////////////////////////////////////////////////////

} // namespace yagol

#endif
