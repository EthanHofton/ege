include(FetchContent)

# ================= EGE =================
FetchContent_Declare(
    ere
    GIT_REPOSITORY https://github.com/EthanHofton/ere.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(ere)
# ========================================


# ================= EnTT =================
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
)
FetchContent_MakeAvailable(EnTT)
# ========================================

# ================= nolhmann_json =================
FetchContent_Declare(
    nolhmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(nolhmann_json)
# ========================================



# ================= SOURCES =================
set(EXE_NAME "ege")

set(SOURCES
    main_layer.cpp
    main.cpp
)
# ============================================


# ================= EXECUTABLE =================
add_executable(${EXE_NAME} ${SOURCES})
# ===============================================


# ================= INCLUDES =================
target_include_directories(${EXE_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include}
)
# =============================================


# ================= LINKS =================
target_link_libraries(${EXE_NAME} PUBLIC
    ere
    EnTT::EnTT
    nlohmann_json::nlohmann_json
)
# =========================================
