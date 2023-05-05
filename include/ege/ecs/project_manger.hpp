#ifndef __EGE_PROJECT_MANAGER_HPP__
#define __EGE_PROJECT_MANAGER_HPP__

#include <string>
#include <fstream>
#include <cstdlib>

#include <ege/core/logger.hpp>
#include <ege/events/app_events.hpp>
#include "project_config.hpp"


namespace ege {

class project_manager {
public:

    static inline bool is_temp_project() { return m_temp_project; }
    static inline const config& get_project_config() { return m_project_config; }
    static inline const std::string& get_project_root() { return m_project_root; }

    static bool open_project(const std::string& project_root);
    static bool new_project(const config& project_config, const std::string& project_root);
    static bool save_project();
    static bool save_project_as(const config& new_project_config, const std::string& project_root);

    static bool create_temp_project();
    static void delete_temp_project();

private:

    static std::string m_temp_project_root;
    static bool m_temp_project;

    static config m_project_config;
    static std::string m_project_root;
};


}


#endif // __EGE_PROJECT_MANAGER_HPP__
