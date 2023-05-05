#ifndef __EGE_PROJECT_CONFIG_HPP__
#define __EGE_PROJECT_CONFIG_HPP__

#include <nlohmann/json.hpp>

namespace ege {

struct config {
    std::string project_name = "New Project";

    int project_version_major = 0;
    int project_version_minor = 1;
    int project_version_patch = 0;
   
    std::string project_author = "No author provided.";
    std::string project_description = "No description provided.";

    std::string project_license = "MIT";
    std::string project_license_url = "https://choosealicense.com/licenses/mit/";

    std::string project_url = "";
};

inline static const std::string m_project_file_filter = "ege.json";

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config, 
                                   project_name,
                                   project_version_major,
                                   project_version_minor,
                                   project_version_patch,
                                   project_author,
                                   project_description,
                                   project_license,
                                   project_license_url,
                                   project_url)

}

#endif // __EGE_PROJECT_CONFIG_HPP__

