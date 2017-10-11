robofit
=========

This package provides (in parts) functionality for activity recognition as well as feedback based on its execution.

### Prerequisites:
A Myo. If no Myo is available, remove the relevant launch file and publish another way.


### Usage:

roslaunch robofit robofit_bringup.launch camera_id:= <ID of your RGB camera>

Subscribes to UInt8 topic via which it takes in the next activity and the difficulty level.

Output is video and audio feedback

=========
Input: std_msgs/UInt8 at topic /exercise

Coding:
Do 10 squats: 1
Do 10 jumping jacks: 2

Do 20 squats: 101
Do 20 jumping jacks: 102   


Output: std_msgs/UInt8 at topic /feedback

Coding:
GOOD: 1 (if set is completed within as yet hardcoded time)
BAD: 2  (if set is not completed in time)


### Constraints:

The myo is not reliable. Possible integration with visual monitoring yet to come.
An own message type for the activities might be beneficial after all to encode exercise type, number of repetitions and timeout duration.
