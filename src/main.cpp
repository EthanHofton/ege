#include <ere/core/entry_point.hpp>
#include <ege/core/main_layer.hpp>
#include <ege/core/colors.hpp>
#include <ege/ecs/project_manger.hpp>

using namespace ere;

ref<ere::application> ere::ere_create_application(int argc, char** argv) {
    ref<application> app = application::get_application();

    app->push_layer(createRef<ege::main_layer>());

    app->set_fps(120);
    app->set_window_title("Ethans Game Engine");
    app->set_background_color(ege::BASE_COLOR);
    app->set_window_size({1600, 900});
    app->set_window_pos({50, 50});

    if (argc > 1) {
        std::string project_root = argv[1];
        ege::project_manager::open_project(project_root);
    } else if (argc == 1) {
        ege::project_manager::create_temp_project();
    }

    return app;
}
