#include "./../include/perception_edge_node.hpp"

int main(int argc , char *argv[])
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<thesis_perception_edge::PerceptionEdgeNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}