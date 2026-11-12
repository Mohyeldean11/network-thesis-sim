
/**Inclusions**/
#include "./../include/placement_node.hpp"

int main(int argc,char *argv[])
{
    //initializing the ROS2 INFRASTRUCTURE
    rclcpp::init(argc, argv);
    //CREATING THE INSTANCE SHARED PTR FROM THE NODE FOR CALLBACKS
    auto node = std::make_shared<thesis_placement::PlacementNode>();
    //START PROCESSING CALLBACKS AND INCOMING MESSAGES
    rclcpp::spin(node);
    //SHUTING DOWN THE INFRASTRUCTURE
    rclcpp::shutdown();
    return 0;
}

