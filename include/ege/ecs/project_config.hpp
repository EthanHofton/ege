#ifndef __EGE_PROJECT_CONFIG_HPP__
#define __EGE_PROJECT_CONFIG_HPP__

#include <nlohmann/json.hpp>

namespace ege {

struct config {
    std::string project_name;
};

inline static const std::string m_project_file_filter = "ege.json";

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config, project_name)

}

#endif // __EGE_PROJECT_CONFIG_HPP__

