#include <ege/ecs/systems/project_explorer_system.hpp>
#include <misc/cpp/imgui_stdlib.h>

namespace ege {

project_explorer_system::project_explorer_system() {
    m_folder_icon = ere::texture2d_api::create_texture2d_api("res/textures/folder_icons/mac-folder-96.png", false);
    m_file_icon = ere::texture2d_api::create_texture2d_api("res/textures/folder_icons/file.png", false);
}

bool project_explorer_system::on_gui_draw(gui_draw_event& t_e) {
    ImGui::Begin("Project Explorer");

    draw_cwd();

    int i = 1;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(m_padding.x, m_padding.y));
    if (ImGui::BeginTable("filetables", 10)) {

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

    if (ImGui::BeginPopupContextWindow("Explorer Menu")) {
        if (ImGui::BeginMenu("Create New")) {
            if (ImGui::MenuItem("Folder")) {
                // create a new folder with a new name if allready taken
                std::filesystem::create_directory(m_project_root / "New Folder");
            }
            if (ImGui::MenuItem("File")) {
                std::ofstream(m_project_root / "New File");
            }

            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    m_selected =  ImGui::IsWindowFocused();

    ImGui::End();

    ImGui::ShowDemoWindow();

    return false;
}

bool project_explorer_system::on_open_project(open_project_event& t_e) {
    m_project_root = project_manager::get_project_root();
    return false;
}

void project_explorer_system::draw_icon(const std::filesystem::path& t_path, const std::string& t_display) {
    ImGui::PushID(t_path.string().c_str());
    ImGui::BeginGroup();
    {
        ere::ref<ere::texture2d_api> tex;
        if (std::filesystem::is_directory(t_path)) { 
            tex = m_folder_icon;
        } else {
            tex = m_file_icon;
        }

        if (ImGui::ImageButton((ImTextureID)(size_t)tex->get_texture_id(), ImVec2(m_icon_size.x, m_icon_size.y)) && std::filesystem::is_directory(t_path)) {
            EGE_INFO("Changing directory to: {}", t_path.string());
            m_project_root = t_path;
        }

        int t = ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x;

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
            std::filesystem::copy(path, m_project_root / std::filesystem::path(path).filename(), std::filesystem::copy_options::recursive);
        }
    }

    return false;
}

}
