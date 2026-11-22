#ifndef PERCEPTION_EDGE_NODE_HPP_
#define PERCEPTION_EDGE_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <sensor_msgs/msg/image.hpp>

#include <cv_bridge/cv_bridge.hpp>

#include <opencv_perception.hpp>
#include <thesis_interfaces/action/run_perception.hpp>


namespace thesis_perception_edge
{
    class PerceptionEdgeNode : public rclcpp::Node
    {
    public:
        using RunPerception = thesis_interfaces::action::RunPerception;
        using GoalHandleRunPerception = rclcpp_action::ServerGoalHandle<RunPerception>;
    private:
        
        rclcpp_action::Server<RunPerception>::SharedPtr edge_action_server ;
        perception_algorithm::OpenCV_Perception image_processing_algorithm;
        bool processing_active{false};
        
        void handle_accepted(const std::shared_ptr<GoalHandleRunPerception> goal_handle);
        void execute_goal(const std::shared_ptr<GoalHandleRunPerception> goal_handle);

        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid , std::shared_ptr<const RunPerception::Goal> goal );
        rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleRunPerception> goal_handle);
    
    public:
        PerceptionEdgeNode();
        ~PerceptionEdgeNode()=default;    
    };

};
#endif