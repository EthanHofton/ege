#ifndef __EGE_PROJECT_EXPLORER_SYSTEM_HPP__
#define __EGE_PROJECT_EXPLORER_SYSTEM_HPP__

#include <imgui.h>
#include <ege/ecs/system.hpp>
#include <ege/ecs/project_manger.hpp>

namespace ege {

class project_explorer_system : public system {
public:

    bool on_gui_draw(gui_draw_event& t_e) override;
};

}

#endif // __EGE_PROJECT_EXPLORER_SYSTEM_HPP__
