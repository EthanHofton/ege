#ifndef __EGE_PROJECT_SELECTOR_SYSTEM_HPP__
#define __EGE_PROJECT_SELECTOR_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace ege {

class project_selector_system : public system {
public:

    bool on_gui_draw(gui_draw_event& t_e) override;

    inline void show_new_project_window() { m_show_new_project_window = true; }
    inline void show_open_project_window() { m_show_open_project_window = true; }
    inline void show_save_project_as_window() { m_show_save_project_as_window = true; }
    void save_project();

private:

    void draw_new_project_window();
    void draw_open_project_window();
    void draw_save_project_as_window();

    bool m_show_new_project_window = false;
    bool m_show_open_project_window = false;
    bool m_show_save_project_as_window = false;
    
    static std::string s_homedir;

};

}

#endif // __EGE_PROJECT_SELECTOR_SYSTEM_HPP__
