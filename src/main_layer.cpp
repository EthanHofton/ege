#include <ege/core/main_layer.hpp>

namespace ege {

bool main_layer::on_attach(ere::attach_event& e) {
    return false;
}

bool main_layer::on_detach(ere::detach_event& e) {
    return false;
}

bool main_layer::on_update(ere::update_event& e) {
    return false;
}

bool main_layer::on_imgui_update(ere::imgui_update_event& e) {
    return false;
}

bool main_layer::on_key_pressed(ere::key_pressed_event& e) {
    return false;
}

bool main_layer::on_key_released(ere::key_released_event& e) {
    return false;
}

bool main_layer::on_key_typed(ere::key_typed_event& e) {
    return false;
}

bool main_layer::on_mouse_button_pressed(ere::mouse_button_pressed_event& e) {
    return false;
}

bool main_layer::on_mouse_button_released(ere::mouse_button_released_event& e) {
    return false;
}

bool main_layer::on_mouse_moved(ere::mouse_moved_event& e) {
    return false;
}

bool main_layer::on_mouse_scrolled(ere::mouse_scrolled_event& e) {
    return false;
}    

}
