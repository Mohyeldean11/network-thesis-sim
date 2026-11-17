#ifndef PERCEPTION_LOCAL_NODE_HPP_
#define PERCEPTION_LOCAL_NODE_HPP_


#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>

#include "./../include/opencv_perception_local.hpp"
#include <run_perception.hpp>
namespace thesis_perception_local
{
    class PerceptionLocalNode : public rclcpp::Node
    {
    private:
        /*Data*/
    public:
        PerceptionLocalNode();
        ~PerceptionLocalNode()=default;
    };
};

#endif