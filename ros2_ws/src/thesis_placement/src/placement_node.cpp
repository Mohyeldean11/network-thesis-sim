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
    3.image_subscription : retains the ownership of the subscription
    4.Camera publishes the image : ROS2 EXECUTOR PROCESSES THE INCOMING MESSAGE
    5.image_callback() executes : Receives and inspects the image
    */
    RCLCPP_INFO(this->get_logger(),"PLACEMENT NODE has been initialized");
    
    //storing the subscription from the image sensor
    image_subscription = this->create_subscription<sensor_msgs::msg::Image>
    (
        "/camera/image_raw",
        rclcpp::SensorDataQoS().keep_last(1),
        std::bind
        (
            &PlacementNode::image_callback,
            this,
            std::placeholders::_1
        )
    );
    perception_action_client = rclcpp_action::create_client<RunPerception>
    (
        this,
        "/run_perception"
    );
    RCLCPP_INFO
    (
        this->get_logger(),
        "Perception Action Client has been initialized"
    );
}

void PlacementNode::image_callback(sensor_msgs::msg::Image::ConstSharedPtr image_msg)
{
    if(processing_goal)
    {
        return;
    }
    processing_goal = true;
   
    RunPerception::Goal goal;
    goal.task_id = task_id_counter++;
    goal.image = *image_msg;
    goal.target_execution = "LOCAL";
    
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
    
    // Check if the Action Server is available
    if(!perception_action_client->action_server_is_ready())
    {
        RCLCPP_WARN
        (
            this->get_logger(),
            "Perception Action Server is not available"
        );
        processing_goal = false;
        return;
    }
    // Configure callbacks
    rclcpp_action::Client<RunPerception>::SendGoalOptions send_goal_options;
    send_goal_options.goal_response_callback = std::bind
                        (
                            &PlacementNode::goal_response_callback,
                            this,
                            std::placeholders::_1
                        );
    send_goal_options.result_callback = std::bind
                        (
                            &PlacementNode::result_callback,
                            this,
                            std::placeholders::_1
                        );
    
    // Send the goal asynchronously
    perception_action_client->async_send_goal
    (
        goal,
        send_goal_options
    );

    RCLCPP_INFO
    (
        this->get_logger(),
        "Perception Goal Sent. Task ID: %lu",
        goal.task_id
    );                    
}

void PlacementNode::goal_response_callback(GoalHandleRunPerception::SharedPtr goal_handle)
{
    if (!goal_handle)
    {
        RCLCPP_WARN
        (
            this->get_logger(),
            "Perception Goal was rejected"
        );

        processing_goal = false;
        return;
    }

    RCLCPP_INFO
    (
        this->get_logger(),
        "Perception Goal was accepted"
    );
}

void PlacementNode::result_callback(const GoalHandleRunPerception::WrappedResult &result)
{
    switch (result.code)
    {
        case rclcpp_action::ResultCode::SUCCEEDED:
        {
            RCLCPP_INFO
            (
                this->get_logger(),
                "Task %lu completed successfully",
                result.result->task_id
            );

            RCLCPP_INFO
            (
                this->get_logger(),
                "Processing time: %.2f ms",
                result.result->processing_time_ms
            );

            RCLCPP_INFO
            (
                this->get_logger(),
                "Result message: %s",
                result.result->message.c_str()
            );

            break;
        }

        case rclcpp_action::ResultCode::ABORTED:
        {
            RCLCPP_ERROR
            (
                this->get_logger(),
                "Task was aborted"
            );

            break;
        }

        case rclcpp_action::ResultCode::CANCELED:
        {
            RCLCPP_WARN
            (
                this->get_logger(),
                "Task was canceled"
            );

            break;
        }

        default:
        {
            RCLCPP_ERROR
            (
                this->get_logger(),
                "Unknown Action Result"
            );

            break;
        }
    }

    // Allow the next camera image to be processed
    processing_goal = false;
}