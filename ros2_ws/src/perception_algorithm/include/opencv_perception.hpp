#ifndef OpenCV_Perception_HPP_
#define OpenCV_Perception_HPP_

#include "perception_algorithm.hpp"

namespace perception_algorithm
{
    class OpenCV_Perception : public Perception_Algorithm
    {

    public:
        Perception_Result Process_Image(const cv::Mat & input_image)override;
    };
    
}




#endif