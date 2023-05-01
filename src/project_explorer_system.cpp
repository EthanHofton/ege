#include <ege/ecs/systems/project_explorer_system.hpp>

namespace ege {

bool project_explorer_system::on_gui_draw(gui_draw_event& t_e) {
    ImGui::Begin("Project Explorer");

    if (project_manager::is_project_loaded()) {
        ImGui::Text("Project Root: %s", project_manager::get_project_root().c_str());
        ImGui::Text("Project Name: %s", project_manager::get_project_config().project_name.c_str());
    }

    ImGui::End();

    return false;
}

}
