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

static int cb_var = 0;

void streamerCallback(const std_msgs::UInt8::ConstPtr& msg)
{
    cb_var = msg->data;

}


int main(int argc, char **argv) {

    ros::init(argc, argv, "video_streamer");

    ros::NodeHandle n("~");

    image_transport::ImageTransport it(n);

    int camera_id;

    Mat pos_feed = imread(ros::package::getPath("robofit")+"/media/positive_big.png", CV_LOAD_IMAGE_COLOR);
    Mat neg_feed = imread(ros::package::getPath("robofit")+"/media/negative_big.png", CV_LOAD_IMAGE_COLOR);

    n.param(n.getNamespace() + "/camera_id", camera_id, 0);

    string image_topic, feedback_topic;
    n.param(n.getNamespace() + "/camera_image", image_topic, n.getNamespace() + "/image");
    n.param(n.getNamespace() + "feedback", feedback_topic, string("/feedback"));

    image_transport::Publisher stream_pub = it.advertise(n.getNamespace() + image_topic, 1000);

    ros::Rate loop_rate(30);
    ros::Subscriber sub = n.subscribe<std_msgs::UInt8>(feedback_topic, 1000, streamerCallback);

    VideoCapture cap(camera_id);
    if(!cap.isOpened())
    {
        ROS_ERROR("Unable to receive video stream from camera device with ID %i.", camera_id);
    }

    cv_bridge::CvImage frame_bridge;
    frame_bridge.encoding = "bgr8";
    sensor_msgs::Image img_out;

    while(ros::ok())
    {
        cout << "cb var: " << cb_var << endl;
        Mat frame;
        if(cb_var == 0) // Nothing
        {

            cap >> frame_bridge.image;


        }
        else if(cb_var == 1) // Positive
        {
            frame_bridge.image = pos_feed;
        }    
        else if(cb_var == 2) // Negative
        {
            frame_bridge.image = neg_feed;
        }    
        stream_pub.publish(frame_bridge.toImageMsg());
        ros::spinOnce();
        loop_rate.sleep();
    }

}
