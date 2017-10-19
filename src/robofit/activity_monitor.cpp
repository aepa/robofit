#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/UInt8.h>
#include <ros/package.h>


using namespace std;

//enum

class ActivityMonitor
{
    public:
        ActivityMonitor()
        {
            idle_ = 1;
            count_ = 0;
            ros::NodeHandle np("~");
            ros::NodeHandle n;
            timer_ = n.createTimer(ros::Duration(20), &ActivityMonitor::timerCallback, this, 0, 0);

            np.param("feedback_topic", feedback_topic_, string("/feedback"));
            np.param("exercise_topic", exercise_topic_, string("/exercise"));

            sub_ = n.subscribe<std_msgs::UInt8>(exercise_topic_, 1000, &ActivityMonitor::activityCallback, this);
            pub_ = n.advertise<std_msgs::UInt8>(feedback_topic_, 1000);
        }

        void timerCallback(const ros::TimerEvent& event)
        {
            cout << "Timeout." << endl;
            idle_ = 1;
            feedback_.data = 12;
            pub_.publish(feedback_);
            timer_.stop(); 
    
        }
    
        void activityCallback(const std_msgs::UInt8::ConstPtr& msg)
        {
            if(idle_)
            {
                if(msg->data == 1)
                {
                    cout << "Doing Squats." << endl;
                    idle_ = !idle_;
                    count_ = 3;
                    timer_.setPeriod(ros::Duration(60));
                    timer_.start();
                }
                else if(msg->data == 2)
                {
                    cout << "Doing Jumping Jacks." << endl;
                    idle_ = !idle_;
                    count_ = 3;
                    timer_.setPeriod(ros::Duration(60));
                    timer_.start();
                }
            }
            else
            {
                if(msg->data == 4)
                {

                    count_--;
                    if(count_ <= 0)
                    {
                        timer_.stop();
                        idle_ = 1;
                        feedback_.data = 11;
                        pub_.publish(feedback_);
                        cout << "Success!" << endl;
                    }
                    else 
                    {
                        cout << "Need " << count_ << " more." << endl;
                    }
                }
            }
    
        }
    private:
        bool idle_;
        int count_;
        string feedback_topic_;
        string exercise_topic_;
        std_msgs::UInt8 feedback_;
        ros::Subscriber sub_;
        ros::Publisher pub_;
        ros::Timer timer_;

}; // End ActivityMonitor Class

int main(int argc, char **argv) {

    ros::init(argc, argv, "activity_monitor");

    ActivityMonitor actmony;

    ros::spin();

}
