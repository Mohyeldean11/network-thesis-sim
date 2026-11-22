#ifndef PERCEPTION_ALGORITHM_LOCAL_HPP_
#define PERCEPTION_ALGORITHM_LOCAL_HPP_
/* A GENERIC CLASS ABSTRACT TO BE INHERITED FROM :
                                                     A.CLASSIC OPENCV
                                                     B.YOLO OPENCV
*/
/** CPP Libs **/
#include <string>
/**OPENCV LIBS **/
#include <opencv2/core.hpp>

namespace perception_algorithm
{
    //OUTPUT FROM THE PERCEPTION MODEL
    struct Perception_Result
    {
        /* data */
        int image_width{0};
        int image_height{0};
        int feature_count{0};
        double processing_time_ms{0.0};
        std::string message{};
        double edge_density{0.0};
    };
    //PERCEPTION ALGORITHM CLASS
    class Perception_Algorithm
    {

    public:
        virtual ~Perception_Algorithm() = default;
        virtual Perception_Result Process_Image(const cv::Mat & input_image) = 0;
    };
    
}



#endif