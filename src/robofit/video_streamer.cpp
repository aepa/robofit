#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt8.h>
#include <ros/package.h>


using namespace cv;
using namespace std;

class VideoStreamer
{
    public:
        VideoStreamer()
        {
            ros::NodeHandle n;
            ros::NodeHandle np("~");
            ros::Rate loop_rate(30);
            image_transport::ImageTransport it(n);
            timer_ = n.createTimer(ros::Duration(2), &VideoStreamer::timerCallback, this, 0, 0);
        

            np.param("camera_image", image_topic_, string("image"));
            np.param("feedback_topic", feedback_topic_, string("/feedback"));
            np.param("exercise_topic", exercise_topic_, string("/gesure"));
            np.param("camera_id", camera_id_, 0);

            stream_pub_ = it.advertise(image_topic_, 1000);
            sub_ = n.subscribe<std_msgs::UInt8>(feedback_topic_, 1000, &VideoStreamer::feedbackCallback, this);
            subg_ = n.subscribe<std_msgs::UInt8>(exercise_topic_, 1000, &VideoStreamer::exerciseCallback, this);
        
            VideoCapture cap(camera_id_);
            if(!cap.isOpened())
            {
                ROS_ERROR("Unable to receive video stream from camera device with ID %i.", camera_id_);
            }
            cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
            cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
        
            cv_bridge::CvImage frame_bridge;
            frame_bridge.encoding = "bgr8";
            sensor_msgs::Image img_out;
            pos_feed_ = imread(ros::package::getPath("robofit")+"/media/positive_big.png", CV_LOAD_IMAGE_COLOR);
            neg_feed_ = imread(ros::package::getPath("robofit")+"/media/negative_big.png", CV_LOAD_IMAGE_COLOR);
            squats_start_ = imread(ros::package::getPath("robofit")+"/media/squats.jpg", CV_LOAD_IMAGE_COLOR);
            jjacks_start_ = imread(ros::package::getPath("robofit")+"/media/jumping_jacks.jpg", CV_LOAD_IMAGE_COLOR);

            cb_var_ = 0;
            while(ros::ok())
            {
                if(cb_var_ == 0) // Nothing
                {
        
                    cap >> frame_bridge.image;
        
                }
                else if(cb_var_ == 1) // Show that Squats were chosen
                {
                    frame_bridge.image = squats_start_;
                    timer_.start();
                }    
                else if(cb_var_ == 2) // Show that Jumping Jacks were chosen
                {
                    frame_bridge.image = jjacks_start_;
                    timer_.start();
                }    
                else if(cb_var_ == 11) // Positive
                {
                    frame_bridge.image = pos_feed_;
                    timer_.start();
                }    
                else if(cb_var_ == 12) // Negative
                {
                    frame_bridge.image = neg_feed_;
                    timer_.start();
                }    
                stream_pub_.publish(frame_bridge.toImageMsg());
                
                ros::spinOnce();
                loop_rate.sleep();
            }
        }        
        void timerCallback(const ros::TimerEvent& event)
        {
            cb_var_ = 0;
            timer_.stop();
        }
        

        void feedbackCallback(const std_msgs::UInt8::ConstPtr& msg)
        {
            cb_var_ = msg->data;
        
        }
        
        void exerciseCallback(const std_msgs::UInt8::ConstPtr& msg)
        {
            cb_var_ = msg->data;
        
        }

    private:

        int camera_id_;
        int cb_var_;
        Mat pos_feed_, neg_feed_, squats_start_, jjacks_start_;
        string image_topic_, feedback_topic_, exercise_topic_;
        image_transport::Publisher stream_pub_;
        ros::Subscriber sub_;
        ros::Subscriber subg_;
        ros::Timer timer_;

}; // End VideoStreamer Class    

int main(int argc, char **argv) {

    ros::init(argc, argv, "video_streamer");
    VideoStreamer vidstreamy;


}
