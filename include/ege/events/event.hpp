#ifndef __EGE_EVENT_HPP__
#define __EGE_EVENT_HPP__

#include <util/event.hpp>
#include <entt/entt.hpp>

namespace ege {

enum class ege_events {
    // draw
    draw_begin, pre_draw, draw, draw_end ,post_draw, gui_draw,
    // updates
    update, attach, detach,
    // simulation
    // IN THIS ORDER
    simulation_begin, simulation_start, simulation_step, simulation_finish, simulation_end,

    // key
    key_pressed, key_released, key_typed,

    // mouse
    mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled,

    // input
    file_drop,
};

struct ege_event : public util::event<ege_events> {};

}

#endif
