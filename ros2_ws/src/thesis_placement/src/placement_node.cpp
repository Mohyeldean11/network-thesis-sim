/** CPP LIBS **/
#include <memory>                       //FOR SMART POINTERS
#include <functional>                   //FOR CALLBACKS

/**PLACEMENT NODE */
#include "./../include/placement_node.hpp"

using namespace thesis_placement;

PlacementNode::PlacementNode() : rclcpp::Node("placement_node")
{
    /*
    1.constructer executes : placement node instance is created and logging
    2.create_subscrition() : registers the subscription with ROS 2
    3.image_subscription_ : retains the ownership of the subscription
    4.Camera publishes the image : ROS2 EXECUTOR PROCESSES THE INCOMING MESSAGE
    5.image_callback() executes : Receives and inspects the image
    */
    RCLCPP_INFO(this->get_logger(),"PLACEMENT NODE has been initialized");
    
    //storing the subscription from the 
    image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/image_raw",
        rclcpp::SensorDataQoS().keep_last(1),
        std::bind(
            &PlacementNode::image_callback,
            this,
            std::placeholders::_1
        )
    );
}
void PlacementNode::image_callback(sensor_msgs::msg::Image::ConstSharedPtr image_msg)
{
    const auto image_width = image_msg->width;
    const auto image_height = image_msg->height;
    // const auto image_encoding = image_msg->encoding;
    // const auto image_step_bytes = image_msg->step;
    const auto image_size = image_msg->data.size();
    RCLCPP_INFO(
        this->get_logger(),
        "Image Received : %u x %u ,size of image : %zu", 
        image_width,
        image_height,
        image_size
    );
}