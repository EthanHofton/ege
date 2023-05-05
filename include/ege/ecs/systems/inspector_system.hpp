#ifndef __EGE_INSPECTOR_SYSTEM_HPP__
#define __EGE_INSPECTOR_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <functional>
#include <imgui.h>

namespace ege {

class inspector_system : public system {
public:

    using window_draw_fn = std::function<void()>;

    template<class T>
    static void inspector_draw(void* t_user_data) {}

    template<class T>
    void set_inspector_focus(void* t_user_data = nullptr) {
        m_window_draw_fn = std::bind(&inspector_system::inspector_draw<T>, t_user_data);
    }

    void unfocus() {
        m_window_draw_fn = []() {};
    }

    bool on_gui_draw(gui_draw_event& t_event) override {
        ImGui::Begin("Inspector");

        ImGui::PushID("Inspector");

        m_window_draw_fn();

        ImGui::PopID();

        ImGui::End();

        return true;
    }

private:

    window_draw_fn m_window_draw_fn = []() {};
};

}

#endif // __EGE_INSPECTOR_SYSTEM_HPP__
