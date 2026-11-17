#include "./../include/opencv_perception_local.hpp"


#include <opencv2/imgproc.hpp> //NEEDED FOR RESIZING GRAYSCALING AND GAUSSIAN BLURING AND CANNY ALGO

#include <stdexcept>       //HANDLING THE INVALID IMGS

#include <chrono>     //NEEDED TO MEASURE THE EXECUTION TIME OF THE PERCEPTION ALGO

using namespace thesis_perception_local ;

Perception_Result OpenCV_Perception_Local::Process_Image(const cv::Mat &input_image)
{
    // CREATING A RETURN FOR THE FUNCTION OF TYPE OF PERCEPTION_RESULT STRUCT
    Perception_Result perception_result;

    //CHEKING IF THE INPUT IMAGE IS VALID 
    if(input_image.empty())
    {
        // image_result.message = "Invalid picture";

        //IF NOT THROW AN ERROR
        throw std::invalid_argument("Invalid picture");
        // return image_result;
    }
    //START OF THE PROCESSING TIME
    auto start_time = std::chrono::steady_clock::now();
    //CREATING A DESTINATION TO RESIZE IMAGE AS THE INPUT IMAGE IS CONSTANT AND NON CHANGABLE (CREATING AN EMPTY MATRIX OBJECT)
    cv::Mat resized_image;
    //RESIZING THE IMAGE TO BE READY FOR NEXT STEP WHICH IS GRAYSCALING
    cv::resize(input_image,resized_image,cv::Size(640,480));
    //CREATING A GRAY IMAGE MATRIX TO CHANGE THE REZISED BGR IMAGE TO GRAY SCALE READY FOR GAUSSIAN NOISE REDUCTION AND THEN READY TO BE PROCESSED BY CANNY ALGO
    cv::Mat gray_image ;
    cv::cvtColor(resized_image,gray_image,cv::COLOR_BGR2GRAY);

    //NOW BLURING THE IMAGE USING GAUSSIAN BLUR WITH SIGMA 0 AND KERNEL OF SIZE 5X5
    cv::Mat blurred_image;
    cv::GaussianBlur(gray_image,blurred_image,cv::Size(5,5),0);

    //APPLYING CANNY ALGORITHM ON THE REFINED GAUSSIAN PICTURE
    cv::Mat edge_image;
    //150 THRESHOLD IS WHERE ANYTHING ABOVE IT IS DEFINETLY INCLUDED AS AN EDGE.
    //50 THRESHOLD IS WHERE ANYTHING BELOW IT IS DEFINETLY NOT INCLUDED.
    //ANYTHING IN BETWEEN MIGHT BE CONNECTED OR NOT SO WE KEEP IT FOR FURTHER ANALYSIS
    //THIS CALLED HYSTERESIS THRESHOLD
    cv::Canny(blurred_image,edge_image,50,150);
    
    //NOW GETTING THE NON ZERO PIXELS IN THE IMAGE
    perception_result.feature_count = cv::countNonZero(edge_image);

    //NOW CALCULATING THE MEASUREMENTS OF THE RESULTED IMAGE
    perception_result.image_width = edge_image.cols;
    perception_result.image_height = edge_image.rows;
    perception_result.edge_density = static_cast<double>(perception_result.feature_count) / ( static_cast<double>(perception_result.image_height) * static_cast<double>(perception_result.image_width) ) * 100;
    //NOW WE SEE THE END TIME OF PROCESSING
    auto end_time = std::chrono::steady_clock::now();

    //WE CALCULATE THE ELAPSED PROCESSING TIME NOW AND ASSIGN IT TO THE RESULT MESSAGE
    perception_result.processing_time_ms = 
                                        std::chrono::duration<double,std::milli>(
                                            end_time - start_time
                                        ).count();
    perception_result.message = "Classical OpenCV perception completed";
    return perception_result;

}