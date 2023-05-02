#ifndef __EGE_PROJECT_EXPLORER_SYSTEM_HPP__
#define __EGE_PROJECT_EXPLORER_SYSTEM_HPP__

#include <imgui.h>
#include <ege/ecs/system.hpp>
#include <ege/ecs/project_manger.hpp>
#include <ere/core/core.hpp>
#include <ere/api/texture2d_api.hpp>

namespace ege {

class project_explorer_system : public system {
public:

    project_explorer_system();
    bool on_gui_draw(gui_draw_event& t_e) override;
    bool on_open_project(open_project_event& t_e) override;

private:

    void draw_icon(const std::filesystem::path& t_path, const std::string& t_display_name);

    std::filesystem::path m_project_root;
    ere::ref<ere::texture2d_api> m_folder_icon;
    ere::ref<ere::texture2d_api> m_file_icon;
    glm::vec2 m_icon_size = {96, 96};
    glm::vec2 m_padding = {24, 5};
};



}

#endif // __EGE_PROJECT_EXPLORER_SYSTEM_HPP__
