cmake_minimum_required(VERSION 3.8)
project(thesis_placement)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(thesis_interfaces REQUIRED)

#build placement node
add_executable(
  placement_node 
  src/main.cpp
  src/placement_node.cpp
  src/placement_algorithm.cpp
)

#Include package headers
target_include_directories(
  placement_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include()>
  $<INSTALL_INTERFACE:include>
)

# Link ROS 2 dependencies
ament_target_dependencies(
  placement_node
  rclcpp
  rclcpp_action
  sensor_msgs
  std_msgs
  thesis_interfaces
)

# Install executable
install(
  TARGETS placement_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install public headers
install(
  DIRECTORY include/
  DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
