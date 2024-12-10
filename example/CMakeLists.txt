# add_executable(example example_main.cc)
# add_executable(example_leetcode example_leetcode.cc)
# message(STATUS "string:${${PROJECT_NAME}_SOURCE_DIR}")
# target_include_directories(example PRIVATE
#                             ${${PROJECT_NAME}_SOURCE_DIR}/include
# )
# target_include_directories(example_leetcode PRIVATE
#                             ${${PROJECT_NAME}_SOURCE_DIR}/include
# )
# target_link_libraries(example PRIVATE ${MY_LIB_NAME})
# target_link_libraries(example_leetcode PRIVATE ${MY_LIB_NAME})

set(SOURCE_EXAMPLE_FILES
  example_leetcode.cc
  example_main.cc
  example_type.cc
  example_singleton.cc
  example_debugwait.cc
  example_filename.cc
  example_logfile.cc
)

# 使用 foreach 遍历源文件并将它们添加到可执行目标中
foreach(SOURCE_EXAMPLE ${SOURCE_EXAMPLE_FILES})
                            get_filename_component(EXEC ${SOURCE_EXAMPLE} NAME_WE)
    add_executable(${EXEC} ${SOURCE_EXAMPLE})
    target_include_directories(${EXEC} PRIVATE
                            ${${PROJECT_NAME}_SOURCE_DIR}/include
    )
    target_link_libraries(${EXEC} PRIVATE ${MY_LIB_NAME})
    target_link_libraries(${EXEC} PRIVATE pthread)
endforeach()
