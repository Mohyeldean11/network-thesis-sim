#include "./../include/perception_local_node.hpp"
#include <thread>
#include <memory>
#include <exception>
#include <functional>
using namespace thesis_perception_local ;


PerceptionLocalNode::PerceptionLocalNode() : rclcpp::Node("perception_local_node")
{
    /*
    1.constructer executes : perception local node instance is created and logging
    2.create a server action() : creating an action server with all callbacks

    */
    RCLCPP_INFO
    (
        this->get_logger(),
        "Local PERCEPTION NODE has been initialized"
    );
    local_action_server = rclcpp_action::create_server<RunPerception>
                                    (
                                        this,
                                        "run_perception",
                                        std::bind
                                        (
                                            &PerceptionLocalNode::handle_goal,
                                            this,
                                            std::placeholders::_1,
                                            std::placeholders::_2
                                        ),
                                        std::bind
                                        (
                                            &PerceptionLocalNode::handle_cancel,
                                            this,
                                            std::placeholders::_1
                                        ),
                                          std::bind
                                        (
                                            &PerceptionLocalNode::handle_accepted,
                                            this,
                                            std::placeholders::_1
                                        )
                                    );

    RCLCPP_INFO
    (
        this->get_logger(),
        "RunPerception action server is ready"
    );
    }

rclcpp_action::GoalResponse PerceptionLocalNode::handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const RunPerception::Goal> goal)
{
    RCLCPP_INFO(this->get_logger(),"Receiving goal with taskID= %lu",goal->task_id);
    if(goal->target_execution != "LOCAL")
    {
        RCLCPP_WARN(this->get_logger(),"Rejecting goal : Target is not LOCAL");
        
        return rclcpp_action::GoalResponse::REJECT;
    }
    
    // {
    //     // ensuring only one callback at a time can inspect and modify the state of the  processing active
    //     std::lock_guard<std::mutex> lock(processing_mutex);

    //     if(processing_active)
    //     {
    //         RCLCPP_INFO(this->get_logger(),"Perception node is busy. Deffering goal.");
    //         return rclcpp_action::GoalResponse::ACCEPT_AND_DEFER;
    //     }
    //     processing_active = true;
    // }
    RCLCPP_INFO(this->get_logger(),"Accepting perception goal");
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}


rclcpp_action::CancelResponse PerceptionLocalNode::handle_cancel(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    // if(goal_handle->is_active())
    // {
    //     RCLCPP_INFO(this->get_logger(),"CANCEL REQUEST received for task id %lu and the task is still active ",goal_handle->get_goal()->task_id);
    //     RCLCPP_INFO(this->get_logger(),"CANCEL REQUEST is executed for task id %lu",goal_handle->get_goal()->task_id));
    //     goal_handle->abort();
    //     return rclcpp_action::CancelResponse::ACCEPT;
    // }
    RCLCPP_INFO(this->get_logger(),"CANCEL REQUEST received for task id %lu and rejected",goal_handle->get_goal()->task_id);
    
    return rclcpp_action::CancelResponse::REJECT;
}
void PerceptionLocalNode::handle_accepted(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    RCLCPP_INFO(this->get_logger(),"task id %lu is accepted and being executed",goal_handle->get_goal()->task_id);
    std::thread
    (
        [this,goal_handle]()
        {
            execute_goal(goal_handle);
        }
    ).detach();
    
}
void PerceptionLocalNode::execute_goal(const std::shared_ptr<GoalHandleRunPerception> goal_handle)
{
    auto result = std::make_shared<RunPerception::Result>();

    result->task_id = goal_handle->get_goal()->task_id;

    try
    {
        // 1. Extract the ROS image from the action goal
        const sensor_msgs::msg::Image &raw_image = goal_handle->get_goal()->image;

        // 2. Convert ROS image to OpenCV image
        cv_bridge::CvImagePtr cv_image = cv_bridge::toCvCopy(raw_image, "bgr8");

        // 3. Run the perception algorithm
        perception_algorithm::Perception_Result perception_result = image_Processing_Algorithm.Process_Image(cv_image->image);

        // 4. Populate the action result
        result->success = true;
        result->message = perception_result.message;
        result->processing_time_ms = perception_result.processing_time_ms;

        // 5. Send successful result to the action client
        goal_handle->succeed(result);

        RCLCPP_INFO
        (
            this->get_logger(),
            "Task %lu completed successfully in %.2f ms",
            result->task_id,
            result->processing_time_ms
        );
    }
    catch (const cv_bridge::Exception &exception)
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
    catch (const std::exception &exception)
    {
        result->success = false;
        result->message = exception.what();

        goal_handle->abort(result);

        RCLCPP_ERROR
        (
            this->get_logger(),
            "Perception failed for task %lu: %s",
            result->task_id,
            exception.what()
        );
    }
}