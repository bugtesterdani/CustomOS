###################################
# Definition for executable
add_executable(${PROJECT_NAME}
    main.cpp
)

###################################
# Env
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)

###################################
# Libs

add_library(log)
add_subdirectory(log)
target_include_directories(log
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_library(parser)
add_subdirectory(parser)
target_include_directories(parser
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

###################################
# Linker definition
target_link_libraries(${PROJECT_NAME}
    log
    parser
)   