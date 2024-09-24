file(GLOB_RECURSE PROJECT_SOURCES "*.cpp" "*.hpp" "*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
