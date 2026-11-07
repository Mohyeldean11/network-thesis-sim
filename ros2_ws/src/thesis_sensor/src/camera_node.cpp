#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

using namespace std::chrono_literals;

class CameraNode : public rclcpp::Node
{
public:
    CameraNode()
    : Node("camera_node")
    {
        publisher_ =
            this->create_publisher<sensor_msgs::msg::Image>(
                "/camera/image_raw", 10);

        timer_ = this->create_wall_timer(
            100ms,
            std::bind(&CameraNode::publish_image, this));

        RCLCPP_INFO(this->get_logger(), "Simulated camera started");
    }

private:
    void publish_image()
    {
        auto image = sensor_msgs::msg::Image();

        image.header.stamp = this->now();
        image.header.frame_id = "camera_link";

        image.height = 480;
        image.width = 640;
        image.encoding = "rgb8";
        image.is_bigendian = 0;
        image.step = image.width * 3;

        image.data.resize(image.height * image.step);

        // Generate a simple synthetic RGB image.
        for (uint32_t y = 0; y < image.height; ++y)
        {
            for (uint32_t x = 0; x < image.width; ++x)
            {
                const size_t index = y * image.step + x * 3;

                image.data[index]     = x % 256;       // Red
                image.data[index + 1] = y % 256;       // Green
                image.data[index + 2] = (x + y) % 256; // Blue
            }
        }

        publisher_->publish(image);

        RCLCPP_INFO(
            this->get_logger(),
            "Published image: %ux%u",
            image.width,
            image.height);
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<CameraNode>());

    rclcpp::shutdown();
    return 0;
}
