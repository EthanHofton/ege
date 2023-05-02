#include <ege/ecs/project_manger.hpp>
#include <ege/ecs/system_manager.hpp>

namespace ege {

bool project_manager::m_temp_project = false;
std::string project_manager::m_temp_project_root = "";
config project_manager::m_project_config;
std::string project_manager::m_project_root;


bool project_manager::open_project(const std::string& project_root) {
    // check validity of project root
    if (!std::filesystem::exists(project_root)) {
        return false;
    }

    if (!std::filesystem::is_directory(project_root)) {
        return false;
    }

    if (!std::filesystem::exists(project_root + "/" + m_project_file_filter)) {
        return false;
    }

    // load project config
    std::ifstream config_file(project_root + "/" + m_project_file_filter);
    nlohmann::json config_json = nlohmann::json::parse(config_file);
    config_file.close();

    m_project_config = config_json.get<ege::config>();
    m_project_root = project_root;
    m_temp_project = false;

    EGE_INFO("Loaded project '{}' from directory '{}'", m_project_config.project_name, project_root);

    open_project_event e;
    system_manager::on_event(e);

    return true;
}

bool project_manager::new_project(const config& project_config, const std::string& project_root) {
    // check validity of project root
    if (!std::filesystem::exists(project_root)) {
        return false;
    }

    if (!std::filesystem::is_directory(project_root)) {
        return false;
    }

    // create project root
    m_project_root = project_root;
    m_project_root += "/";
    m_project_root += project_config.project_name;

    EGE_INFO("Creating project '{}' in directory '{}'", project_config.project_name, m_project_root);
    std::filesystem::create_directory(m_project_root);

    nlohmann::json config_json = project_config;
    std::ofstream config_file(m_project_root + "/" + m_project_file_filter);
    config_file << config_json.dump(4);
    config_file.close();

    // create project directories
    // TODO: create directories

    m_temp_project = false;
    m_project_config = project_config;

    open_project_event e_open;
    system_manager::on_event(e_open);

    new_project_event e_new;
    system_manager::on_event(e_new);

    return true;
}

bool project_manager::save_project() {
    save_project_event e;
    system_manager::on_event(e);

    return true;
}

bool project_manager::save_project_as(const config& new_project_config, const std::string& project_root) {
    // check validity of project root
    if (!std::filesystem::exists(project_root)) {
        return false;
    }

    if (!std::filesystem::is_directory(project_root)) {
        return false;
    }

    std::string old_project_root = m_project_root;

    // create project root
    m_project_root = project_root;
    m_project_root += "/";
    m_project_root += new_project_config.project_name;
    m_project_root = std::filesystem::path(m_project_root).string();

    EGE_INFO("Saving Project As '{}' in directory '{}'", new_project_config.project_name, m_project_root);
    std::filesystem::copy(old_project_root, m_project_root, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);

    nlohmann::json config_json = new_project_config;
    std::ofstream config_file(m_project_root + "/" + m_project_file_filter, std::ios::out | std::ios::trunc);
    config_file << config_json.dump(4);
    config_file.close();

    // create project directories
    // TODO: create directories

    m_temp_project = true;
    m_project_config = new_project_config;

    open_project_event e_open;
    system_manager::on_event(e_open);

    new_project_event e_new;
    system_manager::on_event(e_new);

    save_project();

    return true;
}

bool project_manager::create_temp_project() {
    config project_config;

    new_project(project_config, std::filesystem::current_path());

    m_temp_project = true;
    m_temp_project_root = m_project_root;

    return true;
}

void project_manager::delete_temp_project() {
    if (m_temp_project_root.empty()) {
        return;
    }

    if (!std::filesystem::exists(m_temp_project_root)) {
        return;
    }

    if (!std::filesystem::is_directory(m_temp_project_root)) {
        return;
    }

    std::filesystem::remove_all(m_temp_project_root);
}

}
