# video-query

Analyzing system performance and effects on Video query with ML

For running the individual tests
 - Run "./build.sh" to compile the program without IMSHOW. Run "./build.sh ENABLE_IMSHOW" to compile the program to display each frame retrieved (waitkey is 1 ms)
 - When the compilation finishes, call "./captureVid <video_file_name> <log_file_name> <frame_skip_rate>". The first parameter is the location to the AVI video file to analyze. The second parameter is the location of the log file (TXT) to be updated. The thirs parameter is the frame skip rate.

For complete benchmark testing, call 'sudo make'. This runs either one or all of the 4 tests
 - Image retrieve 
 - Image retrieve with IMSHOW
 - Image retrieve with system cache cleared
 - Image retrieve with IMSHOW and system cache cleared

Each test generates a log file inside logs/ 

The time reported in the log files are in millisecons (ms)
