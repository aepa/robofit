robofit
=========

This package provides (add a later stage) functionality for activity recognition as well as feedback based on its execution.

### Usage:

roslaunch robofit robofit_bringup.launch camera_id:=<ID of your RGB camera>

Subscribes to UInt8 topic via which it takes in the next activity and the difficulty level.

Output is video and audio feedback


### TODO: 

Myo package to be added later.
