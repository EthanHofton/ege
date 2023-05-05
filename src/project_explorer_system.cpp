#include <ege/ecs/systems/project_explorer_system.hpp>
#include <ege/ecs/systems/inspector_system.hpp>
#include <ege/ecs/system_manager.hpp>
#include <misc/cpp/imgui_stdlib.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace ege {

template<>
inline void inspector_system::inspector_draw<project_explorer_system>(void* t_user_data) {
    ImGui::SeparatorText("File Inspector");

    std::filesystem::path& p = *(std::filesystem::path*)t_user_data;

    static std::string file_name = "";
    ImGui::Text("File Name: %s", p.filename().c_str());
    ImGui::InputText("##Rename", &file_name);
    ImGui::SameLine();
    if (ImGui::Button("Rename")) {
        if (!std::filesystem::exists(p.parent_path() / file_name) && !file_name.empty() && file_name != p.filename().string()) {
            std::filesystem::rename(p, p.parent_path() / file_name);
            p = p.parent_path() / file_name;
        }
    }

    if (ImGui::Button("Delete")) {
        std::filesystem::remove(p);
        system_manager::get<inspector_system>()->unfocus();
    }
}

project_explorer_system::project_explorer_system() {
    m_folder_icon = ere::texture2d_api::create_texture2d_api("res/textures/folder_icons/mac-folder-96.png", false);
    m_file_icon = ere::texture2d_api::create_texture2d_api("res/textures/folder_icons/file.png", false);
}

bool project_explorer_system::on_gui_draw(gui_draw_event& t_e) {
    ImGui::Begin("Project Explorer");

    draw_cwd();

    int i = 1;
    int cols = ImGui::GetContentRegionAvail().x / (m_icon_size.x + m_padding.x);
    cols = std::max(1, cols);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(m_padding.x, m_padding.y));
    if (ImGui::BeginTable("filetables", cols)) {

        ImGui::PushID(i++);
        if (project_manager::get_project_root() != m_project_root) {
            ImGui::TableNextColumn();

            draw_icon(m_project_root.parent_path(), "..");
        }
        
        for (auto& path : std::filesystem::directory_iterator(m_project_root)) {
            ImGui::TableNextColumn();

            draw_icon(path, path.path().filename());
        }

        ImGui::PopID();

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    draw_menu();

    m_selected = ImGui::IsWindowFocused();

    ImGui::End();

    if (m_show_import_window) {
        draw_import_window();
    }

    return false;
}

bool project_explorer_system::on_open_project(open_project_event& t_e) {
    m_project_root = project_manager::get_project_root();
    return false;
}

void project_explorer_system::draw_import_window() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal("Importer Window", nullptr, flags)) {
        ImGui::PushID("Importer Window");
        ImGui::SeparatorText("Import File");

        static std::string path = "";
        ImGui::LabelText(path.c_str(), "File Path");
        ImGui::SameLine();
        
        if (ImGui::Button("...")) {
            ImGuiFileDialogFlags flags = ImGuiFileDialogFlags_DontShowHiddenFiles;
            ImGuiFileDialog::Instance()->OpenDialog("ChooseProjectRoot", "Choose File", "{*.*}", path, 1, nullptr, flags);
        }

        float display_w = ImGui::GetIO().DisplaySize.x;
        float display_h = ImGui::GetIO().DisplaySize.y;
        ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);  // The full display area
        ImVec2 minSize = {maxSize.x * 0.5f, maxSize.y *0.5f};  // Half the display area
        ImGuiWindowFlags dialog_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGuiFileDialog::Instance()->Display("ChooseProjectRoot", dialog_flags, minSize, maxSize)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                path = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Import") && !path.empty()) {
            import(path);
            ImGui::CloseCurrentPopup();
            m_show_import_window = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            m_show_import_window = false;
        }

        ImGui::PopID();
        ImGui::EndPopup();
    } else {
        ImGui::OpenPopup("Importer Window");
    }
}

void project_explorer_system::draw_menu() {
    if (ImGui::BeginPopupContextWindow("Explorer Menu")) {
        if (ImGui::BeginMenu("New")) {
            if (ImGui::MenuItem("Folder")) {
                // create a new folder with a new name if allready taken
                std::filesystem::create_directory(m_project_root / "New Folder");
            }
            if (ImGui::MenuItem("File")) {
                std::ofstream(m_project_root / "New File");
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Import")) {
            m_show_import_window = true;
        }

        ImGui::EndPopup();
    }
}

void project_explorer_system::import(std::filesystem::path t_path) {
    std::filesystem::copy(t_path, m_project_root / std::filesystem::path(t_path).filename(), std::filesystem::copy_options::recursive);
}

void project_explorer_system::draw_icon(const std::filesystem::path& t_path, const std::string& t_display) {
    ImGuiButtonFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick;
    ImGui::PushID(t_path.string().c_str());
    ImGui::BeginGroup();
    {
        ere::ref<ere::texture2d_api> tex;
        if (std::filesystem::is_directory(t_path)) { 
            tex = m_folder_icon;
        } else {
            tex = m_file_icon;
        }

        ImGui::ImageButton((ImTextureID)(size_t)tex->get_texture_id(), ImVec2(m_icon_size.x, m_icon_size.y));

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && std::filesystem::is_directory(t_path)) {
            EGE_INFO("Changing directory to: {}", t_path.string());
            m_project_root = t_path;
            m_selected_path = std::filesystem::path();
            system_manager::get<inspector_system>()->unfocus();
        } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
            m_selected_path = t_path;
            system_manager::get<inspector_system>()->set_inspector_focus<project_explorer_system>(&m_selected_path);
        } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered() && ImGui::IsWindowHovered()) {
            system_manager::get<inspector_system>()->unfocus();
        }

        std::string name = t_path.filename();
        ImGui::TextWrapped("%s", t_display.c_str());

        ImGui::EndGroup();
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", t_path.filename().c_str());
    }

    ImGui::PopID();
}

void project_explorer_system::draw_cwd() {
    ImGui::PushID("cwd");

    auto base_path = std::filesystem::relative(m_project_root, std::filesystem::path(project_manager::get_project_root()).parent_path());
    auto fullpath = std::filesystem::path(project_manager::get_project_root()).parent_path();

    for (auto &p : base_path) {
        fullpath /= p;
        ImGui::PushID(fullpath.c_str());

        if (ImGui::Button(p.string().c_str())) {
            m_project_root = fullpath;
            system_manager::get<inspector_system>()->unfocus();
        }

        if (fullpath != m_project_root) {
            ImGui::SameLine();
            ImGui::Text("/");
            ImGui::SameLine();
        }

        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::PopID();
}

bool project_explorer_system::on_file_drop(file_drop_event& t_e) {
    if (m_selected) {
        for (auto& path : t_e.get_paths()) {
            import(path);
        }
    }

    return false;
}



}
