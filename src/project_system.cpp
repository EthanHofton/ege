#include <ege/ecs/systems/project_system.hpp>
#include <misc/cpp/imgui_stdlib.h>
#include <filesystem>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <nlohmann/json.hpp>
#include <ege/core/logger.hpp>

namespace ege {

bool project_system::on_gui_draw(gui_draw_event& t_e) {
    if (!m_project_loaded) {
        project_selector_gui();
    }

    if (m_open_project_dialog) {
        open_project_dialog_gui();
    }

    if (m_new_project_dialog) {
        new_project_dialog_gui();
    }

    show_project_explorer();

    return false;
}

void project_system::show_project_explorer() {
    ImGui::Begin("Project Explorer");

    if (m_project_loaded) {
        ImGui::Text("Project Name: %s", m_project_config.project_name.c_str());
        ImGui::Text("Project Root: %s", m_project_root.c_str());
    } else {
        ImGui::Text("No Project Loaded");
    }

    ImGui::End();
}

void project_system::open_project_dialog_gui() {
    // display a modal to select the project root
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("Open Project", NULL, flags)) {

        ImGui::PushID("Open Project Dialog");
        // load project
        open_project_gui();

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_open_project_dialog = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::PopID();

        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("Open Project");
    }
}

void project_system::new_project_dialog_gui() {
    // display a modal to select the project root
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("New Project", NULL, flags)) {

        ImGui::PushID("New Project Dialog");
        // new project
        new_project_gui();

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_new_project_dialog = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopID();

        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("New Project");
    }
}

void project_system::project_selector_gui() {
    // display a modal to select the project root
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("Select Project", NULL, flags)) {
        // new project
        new_project_gui();

        // load project
        open_project_gui();

        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("Select Project");
    }
}


void project_system::new_project_gui() {
    ImGui::PushID("NewProject");

    ImGui::SeparatorText("New Project");
    ImGui::Text("Create a new EGE project");

    static config new_project_config = {
        .project_name = "Untitled Project",
    };
    static std::string project_root = "No Directory Selected";
    static bool error = false;

    ImGui::Spacing();

    ImGui::InputText("Project Name", &new_project_config.project_name);
    ImGui::LabelText("Project Root", "%s", project_root.c_str());
    ImGui::SameLine();

    if (ImGui::Button("Select Directory")) {
        ImGuiFileDialog::Instance()->OpenDialog("SelectProjectRootNew", "Select Project Root", nullptr, ".");
    }

    if (ImGuiFileDialog::Instance()->Display("SelectProjectRootNew")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            project_root = ImGuiFileDialog::Instance()->GetCurrentPath();
        }

        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::Spacing();

    if (ImGui::Button("Create Project")) {
        if (!create_project(new_project_config, project_root)) {
            error = true;
        } else {
            error = false;
            m_new_project_dialog = false;
            ImGui::CloseCurrentPopup();
        }
    }

    if (error) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0));
        ImGui::Text("Failed to create project");
        ImGui::PopStyleColor();
    }

    ImGui::PopID();
}

void project_system::open_project_gui() {
    ImGui::PushID("OpenProject");
    ImGui::SeparatorText("Open Project");
    ImGui::Text("Open an existing EGE project");

    static std::string project_root = "No Directory Selected";
    static bool error = false;

    ImGui::Spacing();

    ImGui::LabelText("Project Root", "%s", project_root.c_str());
    ImGui::SameLine();

    if (ImGui::Button("Select Directory")) {
        ImGuiFileDialog::Instance()->OpenDialog("SelectProjectRootOpen", "Select Project Root", nullptr, ".");
    }

    if (ImGuiFileDialog::Instance()->Display("SelectProjectRootOpen")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            project_root = ImGuiFileDialog::Instance()->GetCurrentPath();
        }

        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::Spacing();

    if (ImGui::Button("Open Project")) {
        if (!load_project(project_root)) {
            error = true;
        } else {
            error = false;
            m_open_project_dialog = false;
            ImGui::CloseCurrentPopup();
        }
    }

    if (error) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0));
        ImGui::Text("Failed to open project");
        ImGui::PopStyleColor();
    }

    ImGui::PopID();
}

bool project_system::on_file_drop(file_drop_event& t_e) {

    return false;
}

bool project_system::on_attach(attach_event& t_e) {

    return false;
}

bool project_system::load_project(const std::string& project_root) {
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
    m_project_loaded = true;

    EGE_INFO("Loaded project '{}' from directory '{}'", m_project_config.project_name, project_root);

    return true;
}

bool project_system::create_project(const config& project_config, const std::string& project_root) {
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

    m_project_loaded = true;
    m_project_config = project_config;

    return true;
}

}
