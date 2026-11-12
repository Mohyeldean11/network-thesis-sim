#ifndef PLACEMENT_NODE_HPP_
#define PLACEMENT_NODE_HPP_
/** ROS 2 Libs **/
#include <rclcpp/rclcpp.hpp>           //FOR THE ROS NODE
#include <sensor_msgs/msg/image.hpp>    //FOR THE SUBSCRIBER TO RECEIVE THE IMAGE MESSAGE
#include <std_msgs/msg/string.hpp>     //FOR THE PUBLISHER TO SEND THE DECISION (EDGE COMPUTING/ LOCAL COMPUTING)

/**DEIGNED  ALGORITHM **/
#include "placement_algorithm.hpp"

namespace thesis_placement 
{
    class PlacementNode : public rclcpp::Node
    {
    private:
        /* data */
        //TOPIC RECEIVED
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
        void image_callback(sensor_msgs::msg::Image::ConstSharedPtr image_msg);
    public:
        PlacementNode ();
        ~PlacementNode ()=default;
    };
    
};

#endif