cmake_minimum_required(VERSION 3.8)
project(thesis_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Interface definitions will be added here.

ament_export_dependencies(rosidl_default_runtime)

ament_package()
