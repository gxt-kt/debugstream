find_package(GTest REQUIRED)
add_executable(test_main
               test_main.cc
               test_time.cc
               test_type.cc
               test_thread.cc
               test_filename.cc
)
target_include_directories(test_main PRIVATE
                            ${${PROJECT_NAME}_SOURCE_DIR}/include
)
target_link_libraries(test_main PRIVATE ${MY_LIB_NAME})
target_link_libraries(test_main PRIVATE GTest::gtest GTest::gtest_main)
