#ifndef __EGE_APP_EVENTS_HPP__
#define __EGE_APP_EVENTS_HPP__

#include "event.hpp"

namespace ege {

#define EMPTY_EVENT(name) class name##_event : public ege_event { public: EVENT_CLASS_TYPE(ege::ege_events, name); };

EMPTY_EVENT(attach);
EMPTY_EVENT(detach);
EMPTY_EVENT(draw_begin);
EMPTY_EVENT(pre_draw);
EMPTY_EVENT(draw);
EMPTY_EVENT(draw_end);
EMPTY_EVENT(post_draw);
EMPTY_EVENT(gui_draw);

EMPTY_EVENT(update);

EMPTY_EVENT(simulation_begin);
EMPTY_EVENT(simulation_start);
EMPTY_EVENT(simulation_step);
EMPTY_EVENT(simulation_finish);
EMPTY_EVENT(simulation_end);

class file_drop_event : public ege_event {
public:

    EVENT_CLASS_TYPE(ege::ege_events, file_drop);

    file_drop_event(const std::vector<std::string>& paths) : paths(paths) {}

    const std::vector<std::string>& get_paths() const { return paths; }

private:

    std::vector<std::string> paths;
};

}

#endif
