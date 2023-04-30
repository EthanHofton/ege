#ifndef __EGE_PROJECT_SYSTEM_HPP__
#define __EGE_PROJECT_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace ege {

struct config {
    std::string project_name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config, project_name)

class project_system : public system {
public:

    bool on_gui_draw(gui_draw_event& t_e) override;
    bool on_file_drop(file_drop_event& t_e) override;
    bool on_attach(attach_event& t_e) override;

    bool load_project(const std::string& project_root);
    bool create_project(const config& project_config, const std::string& project_root);

    inline bool is_project_loaded() const { return m_project_loaded; }
    inline const config& get_project_config() const { return m_project_config; }
    inline const std::string& get_project_root() const { return m_project_root; }
    inline const std::string& get_project_name() const { return m_project_config.project_name; }

    inline void open_project_dialog() { m_open_project_dialog = true; }
    inline void new_project_dialog() { m_new_project_dialog = true; }

private:

    void new_project_gui();
    void open_project_gui();
    void project_selector_gui();
    void show_project_explorer();

    void open_project_dialog_gui();
    void new_project_dialog_gui();

    bool m_project_loaded = false;
    config m_project_config;
    std::string m_project_root;

    static inline const std::string m_project_file_filter = "ege.json";

    bool m_open_project_dialog = false;
    bool m_new_project_dialog = false;
};

}

#endif // __EGE_PROJECT_SYSTEM_HPP__
