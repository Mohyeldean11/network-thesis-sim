#ifndef PERCEPTION_LOCAL_NODE_HPP_
#define PERCEPTION_LOCAL_NODE_HPP_


#include <rclcpp/rclcpp.hpp>  //BASE ROS 2 NODE FUNCTIONALITY
#include <rclcpp_action/rclcpp_action.hpp> //ROS 2 ACTIONS

#include <sensor_msgs/msg/image.hpp> //IMAGE MESSAGE TYPE
#include <cv_bridge/cv_bridge.hpp>  //CONVERTS IMAGE MSSG TO CV MESSAGE

#include <mutex>                //PROTECT SHARED STATE

#include "perception_algorithm/include/opencv_perception.hpp" //CONTAINS ALGORITHM FOR PERCEPTION
#include <thesis_interfaces/action/run_perception.hpp> //HAS THE CUSTOM ACTION INTERFACE





namespace thesis_perception_local
{
    class PerceptionLocalNode : public rclcpp::Node
    {
    public :
        using RunPerception = thesis_interfaces::action::RunPerception;     //AN ALIAS TO USE THE RUNPERCEPTION.ACTION WITHOUT HAVING INCLUDING THE LONG FORMAT FROM THESIS_INTERFACE PKG

        using GoalHandleRunPerception = rclcpp_action::ServerGoalHandle<RunPerception> ;  // AN ALIAS TO REUSE THE SERVER GOAL HANDLE WHEN WE RECEIVE MULTIPLE GOALS FROM THE PLACEMENT NODE
        
    private:
        /*Data*/

        //receive requests from the placement node
        rclcpp_action::Server<RunPerception>::SharedPtr local_action_server;
        //run the opencv processing algorithm
        perception_algorithm::OpenCV_Perception image_Processing_Algorithm;
        //protects shared state while being processed
        std::mutex processing_mutex;
        //a flag to check if the processing is still running or finished
        bool processing_active{false};
        
       //ACTION CALLBACKS
        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const RunPerception::Goal> goal); //handle goal callback function
        
        rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleRunPerception> goal_handle); //call back from handle_cancel
        
        void handle_accepted(const std::shared_ptr<GoalHandleRunPerception> goal_handle);
        void execute_goal(const std::shared_ptr<GoalHandleRunPerception> goal_handle);
    public:


        PerceptionLocalNode();
        ~PerceptionLocalNode()=default;
    };
};

#endif