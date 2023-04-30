#include <ege/ecs/system.hpp>

namespace ege {

void system::on_event(ege_event &t_e) {
    util::event_dispatcher<ege_events> dispatcher(t_e);

    dispatcher.dispatch<draw_begin_event>(std::bind(&system::on_draw_begin, this, std::placeholders::_1));
    dispatcher.dispatch<pre_draw_event>(std::bind(&system::on_pre_draw, this, std::placeholders::_1));
    dispatcher.dispatch<draw_event>(std::bind(&system::on_draw, this, std::placeholders::_1));
    dispatcher.dispatch<draw_end_event>(std::bind(&system::on_draw_end, this, std::placeholders::_1));
    dispatcher.dispatch<post_draw_event>(std::bind(&system::on_post_draw, this, std::placeholders::_1));

    dispatcher.dispatch<gui_draw_event>(std::bind(&system::on_gui_draw, this, std::placeholders::_1));

    dispatcher.dispatch<update_event>(std::bind(&system::on_update, this, std::placeholders::_1));
    dispatcher.dispatch<attach_event>(std::bind(&system::on_attach, this, std::placeholders::_1));
    dispatcher.dispatch<detach_event>(std::bind(&system::on_detach, this, std::placeholders::_1));

    dispatcher.dispatch<simulation_begin_event>(std::bind(&system::on_simulation_begin, this, std::placeholders::_1));

    dispatcher.dispatch<simulation_start_event>(std::bind(&system::on_simulation_start, this, std::placeholders::_1));
    dispatcher.dispatch<simulation_step_event>(std::bind(&system::on_simulation_step, this, std::placeholders::_1));
    dispatcher.dispatch<simulation_finish_event>(std::bind(&system::on_simulation_finish, this, std::placeholders::_1));

    dispatcher.dispatch<simulation_end_event>(std::bind(&system::on_simulation_end, this, std::placeholders::_1));

    dispatcher.dispatch<key_pressed_event>(std::bind(&system::on_key_pressed, this, std::placeholders::_1));
    dispatcher.dispatch<key_released_event>(std::bind(&system::on_key_released, this, std::placeholders::_1));
    dispatcher.dispatch<key_typed_event>(std::bind(&system::on_key_typed, this, std::placeholders::_1));

    dispatcher.dispatch<mouse_button_pressed_event>(std::bind(&system::on_mouse_button_pressed, this, std::placeholders::_1));
    dispatcher.dispatch<mouse_button_released_event>(std::bind(&system::on_mouse_button_released, this, std::placeholders::_1));
    dispatcher.dispatch<mouse_moved_event>(std::bind(&system::on_mouse_moved, this, std::placeholders::_1));
    dispatcher.dispatch<mouse_scrolled_event>(std::bind(&system::on_mouse_scrolled, this, std::placeholders::_1));
    
    dispatcher.dispatch<file_drop_event>(std::bind(&system::on_file_drop, this, std::placeholders::_1));
}

}
