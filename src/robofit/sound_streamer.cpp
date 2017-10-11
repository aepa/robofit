#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/UInt8.h>
#include <sound_play/SoundRequest.h>
#include <ros/package.h>


using namespace std;

static int cb_var = 0;
static string crowd_cheer_01 = "crowd-cheer.ogg";
static string crowd_boo_02 = "crowd-boo.ogg";
class SoundStreamer
{
    public:
        SoundStreamer()
        {
            ros::NodeHandle n;

            media_path_ = ros::package::getPath("robofit")+"/media/";
            output_.volume=1;
            output_.command=1;
            output_.sound=-2;

            n.param("feedback", feedback_topic_, string("/feedback"));
            n.param("robot_sound_topic", sound_topic_, string("/robotsound"));

            sub_ = n.subscribe<std_msgs::UInt8>(feedback_topic_, 1000, &SoundStreamer::streamerCallback, this);
            stream_pub_ = n.advertise<sound_play::SoundRequest>(n.getNamespace() + sound_topic_, 1000);
        }

    void streamerCallback(const std_msgs::UInt8::ConstPtr& msg)
    {
        if(msg->data == 11) 
        {
            output_.arg = media_path_ + crowd_cheer_01;
            stream_pub_.publish(output_);
        
        }
        else if(msg->data == 12) 
        {
            output_.arg = media_path_ + crowd_boo_02;
            stream_pub_.publish(output_);
        }


    }
    private:
        string sound_topic_; 
        string feedback_topic_;
        ros::Subscriber sub_;
        ros::Publisher stream_pub_;
        string media_path_;
        sound_play::SoundRequest output_;

}; // End SoundStreamer Class

int main(int argc, char **argv) {

    ros::init(argc, argv, "sound_streamer");

    SoundStreamer streamy;

    ros::spin();

}
