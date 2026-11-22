#include "./../include/perception_edge_node.hpp"
#include <thread>
#include <memory>
#include <exception>
#include <functional>

using namespace thesis_perception_edge ;

PerceptionEdgeNode::PerceptionEdgeNode() :rclcpp::Node("perception_edge_node")
{
    RCLCPP_INFO
    (
        this->get_logger(),
        "Edge Perception Node has been initialized"
    );
    edge_action_server = rclcpp_action::create_server<RunPerception>
                                (
                                    this,
                                    "run_perception",
                                    std::bind
                                    (
                                        &PerceptionEdgeNode::handle_goal,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2
                                    ),
                                    std::bind
                                    (
                                        &PerceptionEdgeNode::handle_cancel,
                                        this,
                                        std::placeholders::_1
                                    ),
                                    std::bind
                                    (
                                        &PerceptionEdgeNode::handle_accepted,
                                        this,
                                        std::placeholders::_1
                                    )
                                );
    
    RCLCPP_INFO
    (
        this->get_logger(),
        "Edge RunPerception action server is ready"
    );
}


rclcpp_action::CancelResponse PerceptionEdgeNode::handle_cancel(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    // if(goal_handle->is_active())
    // {
    //     RCLCPP_INFO(this->get_logger(),"CANCEL REQUEST received for task id %lu and the task is still active ",goal_handle->get_goal()->task_id);
    //     RCLCPP_INFO(this->get_logger(),"CANCEL REQUEST is executed for task id %lu",goal_handle->get_goal()->task_id));
    //     goal_handle->abort();
    //     return rclcpp_action::CancelResponse::ACCEPT;
    // }
    RCLCPP_INFO
    (
        this->get_logger(),
        "CANCEL REQUEST RECEIVED FOR TASK ID %lu and rejected",
        goal_handle->get_goal()->task_id
    );
    return rclcpp_action::CancelResponse::REJECT;
}

rclcpp_action::GoalResponse PerceptionEdgeNode::handle_goal(const rclcpp_action::GoalUUID &uuid , std::shared_ptr<const RunPerception::Goal> goal)
{
    RCLCPP_INFO
    (
        this->get_logger(),
        "Goal Received for task ID : %lu ",
        goal->task_id
    );


    if(goal->target_execution != "EDGE")
    {
        RCLCPP_WARN
        (
            this->get_logger(),
            "Rejecting goal : TARGET is not EDGE"
        );
        return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO
    (
        this->get_logger(),
        "Goal Accepted"
    );
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

void PerceptionEdgeNode::handle_accepted(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    RCLCPP_INFO
    (
        this->get_logger(),
        "task id %lu is accepted and being executed",
        goal_handle->get_goal()->task_id
    );
    std::thread
    (
        [this,goal_handle]()
        {
            execute_goal(goal_handle);
        }
    ).detach();
}

void PerceptionEdgeNode::execute_goal(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    auto result = std::make_shared<RunPerception::Result>();
    result->task_id = goal_handle->get_goal()->task_id;
    
    try
    {
        const sensor_msgs::msg::Image &raw_image = goal_handle->get_goal()->image;

        cv_bridge::CvImagePtr cv_image = cv_bridge::toCvCopy(raw_image,"bgr8");
        perception_algorithm::Perception_Result perception_result = image_processing_algorithm.Process_Image(cv_image->image);
        
        result->success = true;
        result->message = perception_result.message;
        result->processing_time_ms = perception_result.processing_time_ms;

        goal_handle->succeed(result);
        
        RCLCPP_INFO
        (
            this->get_logger(),
            "Task %lu completed successfully in %.2f ms",
            result->task_id,
            result->processing_time_ms
        );
    }
    catch(const cv_bridge::Exception &exception)
    {
        result->success = false;
        result->message = exception.what();
        goal_handle->abort(result);
        RCLCPP_ERROR
        (
            this->get_logger(),
            "Image conversion failed for task %lu: %s",
            result->task_id,
            exception.what()
        );
    }
    catch(const std::exception &exception)
    {
        result->success = false;
        result->message = exception.what();
        goal_handle->abort(result);
        RCLCPP_ERROR
        (
            this->get_logger(),
            "Image conversion failed for task %lu: %s",
            result->task_id,
            exception.what()
        );   
    }
}