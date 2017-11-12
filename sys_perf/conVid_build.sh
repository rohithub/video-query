#!/bin/bash

if [ "$#" -eq 1 ]
then
g++ -I /usr/local/include -I /usr/local/include -L /usr/local/ -g -o VidConvert VidConvert.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_imgcodecs -D $1
fi 

if [ "$#" -eq 0 ]
then
g++ -I /usr/local/include -I /usr/local/include -L /usr/local/ -g -o VidConvert VidConvert.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_imgcodecs
fi
