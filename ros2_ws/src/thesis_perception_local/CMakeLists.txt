cmake_minimum_required(VERSION 3.8)
project(thesis_perception_local)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(thesis_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

# Build perception node
add_executable(
    perception_local_node
    src/main.cpp
    src/perception_local_node.cpp
    ../perception_algorithm/src/opencv_perception.cpp
)
# Include package and OpenCV headers
target_include_directories(
    perception_local_node PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../perception_algorithm/include
    ${OpenCV_INCLUDE_DIRS}
)
# Link OpenCV libraries
target_link_libraries(
    perception_local_node
    ${OpenCV_LIBS}
)

# ROS 2 dependencies
ament_target_dependencies(
    perception_local_node
    rclcpp
    rclcpp_action
    sensor_msgs
    cv_bridge
    thesis_interfaces
)

# Install executable
install(
    TARGETS perception_local_node
    DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(
    DIRECTORY include/
    DESTINATION include
)

if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)

    set(ament_cmake_copyright_FOUND TRUE)
    set(ament_cmake_cpplint_FOUND TRUE)

    ament_lint_auto_find_test_dependencies()
endif()

ament_package()