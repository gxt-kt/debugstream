add_executable(example example_main.cc)
message(STATUS "string:${${PROJECT_NAME}_SOURCE_DIR}")
target_include_directories(example PRIVATE
                            ${${PROJECT_NAME}_SOURCE_DIR}/include/debugstream
)
target_link_libraries(example PRIVATE ${MY_LIB_NAME})
