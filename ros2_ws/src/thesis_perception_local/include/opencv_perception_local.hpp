#ifndef OPENCV_PERCEPTION_LOCAL_HPP_
#define OPENCV_PERCEPTION_LOCAL_HPP_

#include "perception_algorithm_local.hpp"
namespace thesis_perception_local
{
    class OpenCV_Perception_Local : public Perception_Algorithm
    {

    public:
        Perception_Result Process_Image(const cv::Mat & input_image)override;
    };
    
}




#endif