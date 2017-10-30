#!/bin/bash

if [ "$#" -eq 1 ]
then
g++ -I /usr/local/include/opencv -I /usr/local/include/opencv2 -L /usr/local/ -g -o captureVid captureVid.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -D $1
fi 

if [ "$#" -eq 0 ]
then
g++ -I /usr/local/include/opencv -I /usr/local/include/opencv2 -L /usr/local/ -g -o captureVid captureVid.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching
fi
