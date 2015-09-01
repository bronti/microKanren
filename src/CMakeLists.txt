set (MK_SRCS
        micro_kanren.cpp
        entity.cpp
        state_stream.cpp
        main.cpp)

include_directories("${PROJECT_SOURCE_DIR}/h/")

add_executable(${PROJECT_NAME} "${MK_SRCS}")