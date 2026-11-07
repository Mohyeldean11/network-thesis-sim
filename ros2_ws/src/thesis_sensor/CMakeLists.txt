cmake_minimum_required(VERSION 3.8)
project(thesis_sensor)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(camera_node src/camera_node.cpp)

ament_target_dependencies(camera_node
  rclcpp
  sensor_msgs
)

install(TARGETS
  camera_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()