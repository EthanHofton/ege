#ifndef __EGE_PROJECT_SYSTEM_HPP__
#define __EGE_PROJECT_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace ege {

struct config {
    std::string project_name;
    std::string project_root;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config, project_name, project_root)

class project_system : public system {
public:

    bool on_gui_draw(gui_draw_event& t_e) override;
    bool on_file_drop(file_drop_event& t_e) override;
    bool on_attach(attach_event& t_e) override;

    bool load_project(const std::string& project_root);
    bool create_project(config project_config);

    inline bool is_project_loaded() const { return m_project_loaded; }
    inline const config& get_project_config() const { return m_project_config; }
    inline const std::string& get_project_root() const { return m_project_config.project_root; }
    inline const std::string& get_project_name() const { return m_project_config.project_name; }

private:

    void new_project_gui();
    void open_project_gui();
    void project_selector_gui();
    void show_project_explorer();

    bool m_project_loaded = false;
    config m_project_config;
};

}

#endif // __EGE_PROJECT_SYSTEM_HPP__
