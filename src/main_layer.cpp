#include <ege/core/main_layer.hpp>
#include <ege/ecs/system_manager.hpp>
#include <ege/ecs/systems/project_selector_system.hpp>
#include <ege/ecs/systems/project_explorer_system.hpp>

#include <imgui.h>

namespace ege {

bool main_layer::on_attach(ere::attach_event& e) {
    // SYSTEMS
    system_manager::get<project_explorer_system>();
    system_manager::get<project_selector_system>();

    return false;
}

bool main_layer::on_detach(ere::detach_event& e) {
    project_manager::delete_temp_project();
    return false;
}

bool main_layer::on_update(ere::update_event& e) {
    // SIMULATION EVENTS HANDLED BY THE SIMULATION MANAGER

    // UPDATE EVENT
    ege::update_event update_e;
    system_manager::on_event(update_e);

    // DRAW BEGIN
    ege::draw_begin_event draw_begin_e;
    system_manager::on_event(draw_begin_e);

    // PRE DRAW
    ege::pre_draw_event pre_draw_e;
    system_manager::on_event(pre_draw_e);

    // DRAW
    ege::draw_event draw_e;
    system_manager::on_event(draw_e);

    // DRAW END
    ege::draw_end_event draw_end_e;
    system_manager::on_event(draw_end_e);

    // POST DRAW
    ege::post_draw_event post_draw_e;
    system_manager::on_event(post_draw_e);

    return false;
}

bool main_layer::on_imgui_update(ere::imgui_update_event& e) {
    setup_dockspace();

    // GUI DRAW
    ege::gui_draw_event gui_draw_e;
    system_manager::on_event(gui_draw_e);

    return false;
}

bool main_layer::on_key_pressed(ere::key_pressed_event& e) {
    ege::key_pressed_event key_pressed_e(e.get_key_code(), e.get_repete_count());
    system_manager::on_event(key_pressed_e);

    return false;
}

bool main_layer::on_key_released(ere::key_released_event& e) {
    ege::key_released_event key_released_e(e.get_key_code());
    system_manager::on_event(key_released_e);

    return false;
}

bool main_layer::on_key_typed(ere::key_typed_event& e) {
    ege::key_typed_event key_typed_e(e.get_key_code());
    system_manager::on_event(key_typed_e);

    return false;
}

bool main_layer::on_mouse_button_pressed(ere::mouse_button_pressed_event& e) {
    ege::mouse_button_pressed_event mouse_button_pressed_e(e.get_mouse_button());
    system_manager::on_event(mouse_button_pressed_e);

    return false;
}

bool main_layer::on_mouse_button_released(ere::mouse_button_released_event& e) {
    ege::mouse_button_released_event mouse_button_released_e(e.get_mouse_button());
    system_manager::on_event(mouse_button_released_e);

    return false;
}

bool main_layer::on_mouse_moved(ere::mouse_moved_event& e) {
    ege::mouse_moved_event mouse_moved_e(e.get_pos(), e.get_delta_pos());
    system_manager::on_event(mouse_moved_e);

    return false;
}

bool main_layer::on_mouse_scrolled(ere::mouse_scrolled_event& e) {
    ege::mouse_scrolled_event mouse_scrolled_e(e.get_offset());
    system_manager::on_event(mouse_scrolled_e);

    return false;
}

bool main_layer::on_file_dropped(ere::file_dropped_event& e) {
    ege::file_drop_event file_dropped_e(e.get_file_paths());
    system_manager::on_event(file_dropped_e);

    return false;
}


void main_layer::draw_menu_bar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                system_manager::get<project_selector_system>()->show_new_project_window();
            }

            if (ImGui::MenuItem("Open")) {
                system_manager::get<project_selector_system>()->show_open_project_window();
            }

            if (ImGui::MenuItem("Save")) {
                system_manager::get<project_selector_system>()->save_project();
            }

            if (ImGui::MenuItem("Save As")) {
                system_manager::get<project_selector_system>()->show_save_project_as_window();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void main_layer::setup_dockspace() {
    static bool dockspace_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    draw_menu_bar();

    ImGui::End();
}

}
