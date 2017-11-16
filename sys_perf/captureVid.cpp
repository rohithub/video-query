#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <vector>
//#include "highgui.h"
#include "opencv2/imgcodecs.hpp"

// Pre-processor directives, controlled from g++ -D inputs
//#define ENABLE_IMSHOW		1
//#define ENABLE_IMAGE_RESIZE	1
#define ENABLE_RGB2GRAY		1
#define ENABLE_DIFF_WRITE_FILE	0
//#define LOG_TYPE_INFO		1
//#define ENABLE_WRITE_FRAMES	1
#define DEBUG_ENABLED

/********************************************************************/
#define FRAME_SKIP_RATE		10
#define MAX_INPUT_ARG		4 //Includes the name of the executable
#define MAX_STRING_LEN		255
#define IM_RESIZE_W		400
#define IM_RESIZE_H		400

using namespace cv;

long double convert_to_usec(struct timeval time)
{
	return((time.tv_sec*1000000) + (time.tv_usec));
}

long double convert_to_msec(struct timeval time)
{
	return((time.tv_sec*1000) + (time.tv_usec/1000));
}

int main(int argn, char** argv)
{
	struct timeval start_time, time_1, time_2, time_3, time_4, time_5, time_6, time_7, time_8, time_9, end_time;
	double elapsed_time = 0;

	int frame_skip_rate = FRAME_SKIP_RATE;
	char* vid_file_name;
	char* log_file_loc;
	Scalar tot_sum, max_sum;
	unsigned long long num_of_frames_grab = 0, num_of_frames_retrieve = 0;
	char im_write_loc[200] = "frames/";
	char im_wr_type[6] = ".png";
	char im_wr_num[8] = "";
	char final_string[250] = "";
	bool func_retn = 0;
	unsigned long long tot_grab_time, tot_retrieve_time, tot_color_time, tot_resize_time;
	std::vector<int> compression_param;

	Mat prev_image, next_image;
	Mat prev_resized_im, next_resized_im;
	Mat diff_image;

	int count = 0;
	std::ofstream out_fp;

	if(argn != MAX_INPUT_ARG)	
	{
		std::cout << "ERROR: Command format is: ./captureVid <video file name> <log file name> <frame_skip_rate> " << std::endl;
		return -1;
	}

	log_file_loc = (char*)malloc(sizeof(char) * MAX_STRING_LEN);
	strcpy(log_file_loc, argv[2]);
	out_fp.open(log_file_loc, std::ofstream::app); //use app for appending data

	frame_skip_rate = atoi(argv[3]);
	vid_file_name = (char*)malloc(sizeof(char) * MAX_STRING_LEN);

	strcpy(vid_file_name, argv[1]);
	#if (LOG_TYPE_INFO == 1)
	out_fp << "Video file = " << vid_file_name << ", Frame skip rate = " << frame_skip_rate << std::endl;
	#endif
	out_fp << "New_iter " << std::endl;	//MATLAB will use these flags to determine start of the new iteration
	std::cout << "Frame skip rate = " << frame_skip_rate << std::endl;

	gettimeofday(&start_time, NULL);
	VideoCapture cap(vid_file_name);
	gettimeofday(&time_1, NULL);

	if(!cap.isOpened())
	{
		std::cout << "ERROR: Video cannot be opened\n";
		return -1;
	}
	
	tot_grab_time = 0;
	tot_retrieve_time = 0;
	tot_color_time = 0;
	tot_resize_time = 0;

	for(;;)
	{
		count++;
		num_of_frames_grab++;
		
		gettimeofday(&time_2, NULL);
		func_retn = cap.grab();
		gettimeofday(&time_3, NULL);
		
		tot_grab_time = tot_grab_time + (convert_to_usec(time_3) - convert_to_usec(time_2));
		if(!func_retn)
		{
			break;
		}	
		else
		{	
			if(count >= frame_skip_rate)
			{
				
				num_of_frames_retrieve++;
				gettimeofday(&time_4, NULL);
				func_retn = cap.retrieve(next_image, 0);
				gettimeofday(&time_5, NULL);

				tot_retrieve_time = tot_retrieve_time + (convert_to_usec(time_5) - convert_to_usec(time_4));

				if(!func_retn)
				{
					break;
				}
				else
				{
					count = 0;
					gettimeofday(&time_6, NULL);
					#ifdef ENABLE_RGB2GRAY
					cvtColor(next_image, next_image, CV_RGB2GRAY);
					#endif
					gettimeofday(&time_7, NULL);

					tot_color_time = tot_color_time + (convert_to_usec(time_7) - convert_to_usec(time_6));

					#ifdef ENABLE_IMAGE_RESIZE
						gettimeofday(&time_8, NULL);
						resize(next_image, next_resized_im, Size(IM_RESIZE_W,IM_RESIZE_H));
						gettimeofday(&time_9, NULL);

						tot_resize_time = tot_resize_time + (convert_to_usec(time_9) - convert_to_usec(time_8));
						
						#ifdef ENABLE_IMSHOW
							imshow("Resized images",diff_image);
							waitKey(1);
						#endif
						#ifdef ENABLE_WRITE_FRAMES
							sprintf(final_string, "frames/%llu.png",num_of_frames_retrieve);
							cv::imwrite(final_string, next_resized_im);
						#endif
						#if (ENABLE_DIFF_WRITE_FILE == 1)
							out_fp << tot_sum[0] << std::endl;
						#endif
						prev_resized_im = next_resized_im.clone();
					#else

						#ifdef ENABLE_IMSHOW
							imshow("Diff images",diff_image);
							waitKey(1);
						#endif
						#ifdef ENABLE_WRITE_FRAMES
							sprintf(final_string, "frames/%llu.png",num_of_frames_retrieve);
							cv::imwrite(final_string, next_resized_im);
						#endif
						#ifdef ENABLE_WRITE_FRAMES
							imwrite("frames/0.png", prev_resized_im);
						#endif
						#if (ENABLE_DIFF_WRITE_FILE == 1)
							out_fp << tot_sum[0] << std::endl;
						#endif
						prev_image = next_image.clone();
					#endif
				}
			}
		}
	//Collect all the final times here
		#ifdef DEBUG_ENABLED
		out_fp << (convert_to_usec(time_3) - convert_to_usec(time_2)) << " ";
		out_fp << (convert_to_usec(time_5) - convert_to_usec(time_4)) << " ";
		out_fp << (convert_to_usec(time_7) - convert_to_usec(time_6)) << " ";
		#ifdef ENABLE_IMAGE_RESIZE
		out_fp << (convert_to_usec(time_9) - convert_to_usec(time_8)) << " " << std::endl;
		#else
		out_fp << "0 " << std::endl; //This only ensures consistency in collected log when Resize is disabled
		#endif
		#endif
		
	}
	gettimeofday(&end_time, NULL);
	cap.release();

	#if (LOG_TYPE_INFO == 1)
	out_fp << "Num of Frames = " << num_of_frames_retrieve << std::endl;
	out_fp << "Total time = " << convert_to_msec(end_time) - convert_to_msec(start_time) ;
	out_fp << ", Video load = " << convert_to_usec(time_1) - convert_to_usec(start_time);
	out_fp << ", Im grab = " << tot_grab_time/*/num_of_frames_grab*/;
	out_fp << ", Im retrieve = " << tot_retrieve_time/*/num_of_frames_retrieve*/;
	out_fp << ", RGB 2 Gray = " << tot_color_time/*/num_of_frames_retrieve*/; 
	out_fp << ", Resize = " << tot_resize_time/*/num_of_frames_retrieve*/ << std::endl;
	#else

	out_fp << "Final Values" << std::endl;
	out_fp << convert_to_msec(end_time) - convert_to_msec(start_time) << " ";
	out_fp << convert_to_usec(time_1) - convert_to_usec(start_time) << " ";
	out_fp << tot_grab_time/num_of_frames_grab << " " ;
	out_fp << tot_retrieve_time/num_of_frames_retrieve << " " ;
	out_fp << tot_color_time/num_of_frames_retrieve  << " " ;
	out_fp << tot_resize_time/num_of_frames_retrieve << std::endl;
	#endif

	out_fp.close();
	return 0;
}

