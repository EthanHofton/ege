#include <ege/ecs/systems/project_selector_system.hpp>
#include <ImGuiFileDialog/ImGuiFileDialogConfig.h>

namespace ege {

#ifdef _WIN32
std::string project_selector_system::s_homedir = getenv("USERPROFILE");
#else
std::string project_selector_system::s_homedir = getenv("HOME");
#endif

bool project_selector_system::on_gui_draw(gui_draw_event& t_e) {

    if (m_show_new_project_window) {
        draw_new_project_window();
    }

    if (m_show_open_project_window) {
        draw_open_project_window();
    }

    if (m_show_save_project_as_window) {
        draw_save_project_as_window();
    }

    return false;
}

void project_selector_system::draw_new_project_window() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("New Project", nullptr, flags)) {
        ImGui::PushID("New Project");

        static config project_config = {
            .project_name = "New Project"
        };
        static std::string project_root = s_homedir;
        static std::string error_message = "";

        ImGui::InputText("Project Name", &project_config.project_name);
        ImGui::LabelText("Project Root", "%s", project_root.c_str());
        ImGui::SameLine();
        
        if (ImGui::Button("...")) {
            ImGuiFileDialogFlags flags = ImGuiFileDialogFlags_DontShowHiddenFiles;
            ImGuiFileDialog::Instance()->OpenDialog("ChooseProjectRoot", "Choose Project Root", nullptr, s_homedir, "", 0, nullptr, flags);
        }

        float display_w = ImGui::GetIO().DisplaySize.x;
        float display_h = ImGui::GetIO().DisplaySize.y;
        ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
        ImVec2 minSize = {maxSize.x * 0.5f, maxSize.y *0.5f};  // Half the display area
        ImGuiWindowFlags dialog_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGuiFileDialog::Instance()->Display("ChooseProjectRoot", dialog_flags, minSize, maxSize)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                project_root = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Create")) {
            if (project_manager::new_project(project_config, project_root)) {
                m_show_new_project_window = false;
                error_message = "";
                ImGui::CloseCurrentPopup();
            } else {
                error_message = "Project Directory Invalid!";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_show_new_project_window = false;
            error_message = "";
            ImGui::CloseCurrentPopup();
        }

        if (error_message != "") {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_message.c_str());
        }

        ImGui::PopID();
        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("New Project");
    }
}

void project_selector_system::draw_open_project_window() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("Open Project", nullptr, flags)) {
        ImGui::PushID("Open Project");

        static std::string project_root = s_homedir;
        static std::string error_message = "";

        ImGui::LabelText("Project Dir", "%s", project_root.c_str());
        ImGui::SameLine();
        
        if (ImGui::Button("...")) {
            ImGuiFileDialogFlags flags = ImGuiFileDialogFlags_DontShowHiddenFiles;
            ImGuiFileDialog::Instance()->OpenDialog("ChooseProjectRoot", "Choose Project Dir", nullptr, s_homedir, "", 0, nullptr, flags);
        }

        float display_w = ImGui::GetIO().DisplaySize.x;
        float display_h = ImGui::GetIO().DisplaySize.y;
        ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
        ImVec2 minSize = {maxSize.x * 0.5f, maxSize.y *0.5f};  // Half the display area
        ImGuiWindowFlags dialog_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGuiFileDialog::Instance()->Display("ChooseProjectRoot", dialog_flags, minSize, maxSize)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                project_root = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Open")) {
            if (project_manager::open_project(project_root)) {
                m_show_open_project_window = false;
                error_message = "";
                ImGui::CloseCurrentPopup();
            } else {
                error_message = "Project Directory Invalid!";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_show_open_project_window = false;
            error_message = "";
            ImGui::CloseCurrentPopup();
        }

        if (error_message != "") {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_message.c_str());
        }

        ImGui::PopID();
        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("Open Project");
    }

}

void project_selector_system::draw_save_project_as_window() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("Save Project As", nullptr, flags)) {
        ImGui::PushID("Save Project As");

        static config project_config = {
            .project_name = "New Project"
        };
        static std::string project_root = s_homedir;
        static std::string error_message = "";

        ImGui::InputText("Project Name", &project_config.project_name);
        ImGui::LabelText("Project Root", "%s", project_root.c_str());
        ImGui::SameLine();
        
        if (ImGui::Button("...")) {
            ImGuiFileDialogFlags flags = ImGuiFileDialogFlags_DontShowHiddenFiles;
            ImGuiFileDialog::Instance()->OpenDialog("ChooseProjectRoot", "Choose Project Root", nullptr, s_homedir, "", 0, nullptr, flags);
        }

        float display_w = ImGui::GetIO().DisplaySize.x;
        float display_h = ImGui::GetIO().DisplaySize.y;
        ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
        ImVec2 minSize = {maxSize.x * 0.5f, maxSize.y *0.5f};  // Half the display area
        ImGuiWindowFlags dialog_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGuiFileDialog::Instance()->Display("ChooseProjectRoot", dialog_flags, minSize, maxSize)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                project_root = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Create")) {
            if (project_manager::save_project_as(project_config, project_root)) {
                m_show_save_project_as_window = false;
                error_message = "";
                ImGui::CloseCurrentPopup();
            } else {
                error_message = "Project Directory Invalid!";
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_show_save_project_as_window = false;
            error_message = "";
            ImGui::CloseCurrentPopup();
        }

        if (error_message != "") {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_message.c_str());
        }

        ImGui::PopID();
        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("Save Project As");
    }
}

}
