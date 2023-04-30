#include <ege/core/main_layer.hpp>
#include <imgui.h>

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
    setup_dockspace();

    ImGui::ShowDemoWindow();

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

void main_layer::draw_menu_bar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save As");

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
