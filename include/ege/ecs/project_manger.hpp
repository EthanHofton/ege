#ifndef __EGE_PROJECT_MANAGER_HPP__
#define __EGE_PROJECT_MANAGER_HPP__

#include <string>
#include <fstream>

#include <ege/core/logger.hpp>
#include <ege/events/app_events.hpp>
#include "project_config.hpp"


namespace ege {

class project_manager {
public:

    static inline bool is_project_loaded() { return m_project_loaded; }
    static inline const config& get_project_config() { return m_project_config; }
    static inline const std::string& get_project_root() { return m_project_root; }

    static bool open_project(const std::string& project_root);
    static bool new_project(const config& project_config, const std::string& project_root);
    static bool save_project();
    static bool save_project_as(const config& new_project_config, const std::string& project_root);

private:

    static bool m_project_loaded;
    static config m_project_config;
    static std::string m_project_root;
};


}


#endif // __EGE_PROJECT_MANAGER_HPP__
