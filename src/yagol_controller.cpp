// File: yagol_controller.cpp
#include "yagol_controller.hpp"

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////

YAGoLController::YAGoLController(YAGoLModel& model, YAGoLView& view)
    : model_( model )
    , view_( view )
{
    randomize(5, 1);            // 1/5: 20% alive, 80% dead

    redraw();
}

//////////////////////////////////////////////////////////////////////

int YAGoLController::event_loop()
{
    YAGoLEvent event;

    while (( event = view_.get_event() ) != YAGoLEvent::quit) {
        try {
            switch (event) {
                case YAGoLEvent::redraw:
                    redraw();
                    break;
                case YAGoLEvent::randomize:
                    randomize();
                    redraw();
                    break;
                case YAGoLEvent::stop:
                    stop();
                    break;
                case YAGoLEvent::start:
                    start();
                    break;
                case YAGoLEvent::toggle:
                    toggle();
                    break;
                case YAGoLEvent::null:
                case YAGoLEvent::step:
                    step();
                    break;
                case YAGoLEvent::unknown:
                    break;
                default:
                    throw unhandled_event(event);
                    break;
            }
        } catch (const unhandled_event& e) {
            view_.close();
            return e.event_id();
        } catch (const std::invalid_argument& e) {
            notify("error");
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::redraw()
{
    view_.clear();
    size_t x = 0;
    size_t y = 0;
    const size_t width = model_.width();
    for (auto it : model_) {
        view_.set_state(x++,y, it);

        if (x >= width) {
            x = 0;
            ++y;
        }
    }

    view_.show();
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::stop()
{
    view_.stop();
}
void YAGoLController::start()
{
    view_.start();
}
void YAGoLController::toggle()
{
    view_.toggle();
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::step()
{
    for (auto it : model_.next_generation()) {
        size_t x,y;
        bool val;

        std::tie(x,y, val) = YAGoLModel::unpack_diff_iterator(it);

        view_.set_state(x,y, val);
    }

    view_.show();
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::randomize(int range, int density)
{
    for (auto it : model_) {
        it = (std::rand() % range) < density;
    }
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::randomize()
{
    // TODO
    int range = view_.prompt_for_number("range");
    redraw();
    int density = view_.prompt_for_number("density");
    redraw();

    randomize(range, density);
}

//////////////////////////////////////////////////////////////////////

void YAGoLController::notify(const std::string& message)
{
    redraw();
    view_.notify(message);
    redraw();
}
