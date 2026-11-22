#ifndef PLACEMENT_NODE_HPP_
#define PLACEMENT_NODE_HPP_
/** ROS 2 Libs **/
#include <rclcpp/rclcpp.hpp>           //FOR THE ROS NODE
#include <sensor_msgs/msg/image.hpp>    //FOR THE SUBSCRIBER TO RECEIVE THE IMAGE MESSAGE
#include <std_msgs/msg/string.hpp>     //FOR THE PUBLISHER TO SEND THE DECISION (EDGE COMPUTING/ LOCAL COMPUTING)
#include <rclcpp_action/rclcpp_action.hpp>
#include <thesis_interfaces/action/run_perception.hpp>
#include <cstdint>

/**DEIGNED  ALGORITHM **/
#include "placement_algorithm.hpp"

/** TIME MONITOR LIB **/
#include <chrono>

namespace thesis_placement 
{
    class PlacementNode : public rclcpp::Node
    {
    public:
        using RunPerception = thesis_interfaces::action::RunPerception;
        using GoalHandleRunPerception = rclcpp_action::ClientGoalHandle<RunPerception>;
    private:
        /* data */
        bool processing_goal{false};
        std::uint64_t task_id_counter{0};
        std::chrono::steady_clock::time_point task_start_time;
        std::chrono::steady_clock::time_point task_end_time;
        
        //TOPIC RECEIVED
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription;
        rclcpp_action::Client<RunPerception>::SharedPtr perception_action_client;
        //CALLBACKS
        void image_callback(sensor_msgs::msg::Image::ConstSharedPtr image_msg);
        void goal_response_callback(GoalHandleRunPerception::SharedPtr goal_handle);
        void result_callback(const GoalHandleRunPerception::WrappedResult &result);
        public:
        PlacementNode ();
        ~PlacementNode ()=default;
    };
    
};

#endif