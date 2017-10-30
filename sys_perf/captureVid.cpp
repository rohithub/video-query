#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Pre-processor directives, controlled from g++ -D inputs
//#define ENABLE_IMSHOW		0

#define FRAME_SKIP_RATE		10
#define MAX_INPUT_ARG		4 //Includes the name of the executable
#define MAX_STRING_LEN		30

using namespace cv;

double get_process_time(void) 
{
	return (double(std::clock()) * 1000/CLOCKS_PER_SEC);
}

int main(int argn, char** argv)
{
	double start_time, end_time;
	int frame_skip_rate = FRAME_SKIP_RATE;
	char* vid_file_name;
	char* log_file_loc;

	Mat images;
	int count = 0;
	std::ofstream out_fp;

	if(argn != MAX_INPUT_ARG)	
	{
		std::cout << "ERROR: Command format is: ./captureVid <video file name> <log file name> <frame_skip_rate> " << std::endl;
		return -1;
	}

	log_file_loc = (char*)malloc(sizeof(char) * MAX_STRING_LEN);
	strcpy(log_file_loc, argv[2]);
	out_fp.open(log_file_loc, std::ofstream::app);

	frame_skip_rate = atoi(argv[3]);
	vid_file_name = (char*)malloc(sizeof(char) * MAX_STRING_LEN);

	strcpy(vid_file_name, argv[1]);
	out_fp << ", Video file = " << vid_file_name << "Frame skip rate = " << frame_skip_rate << std::endl;
	
	start_time = get_process_time();

	VideoCapture cap(vid_file_name);
	if(!cap.isOpened())
	{
		std::cout << "ERROR: Video cannot be opened\n";
		return -1;
	}
	
	for(;;)
	{
		count++;
		if(!cap.read(images))
		{
			break;
		}	
		else
		{			
			if(count > frame_skip_rate)
			{
				count = 0;
				#ifdef ENABLE_IMSHOW
					imshow("Captured images",images);
					if(waitKey(30)>=0) break;
				#endif
			}
		}
	}
	end_time = get_process_time();
	cap.release();

	out_fp << "Elapsed time " << (end_time - start_time) << std::endl;
	out_fp.close();
	return 0;
}
