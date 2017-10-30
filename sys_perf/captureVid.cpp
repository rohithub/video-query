#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Pre-processor directives, controlled from g++ -D inputs
//#define ENABLE_IMSHOW		1
#define ENABLE_IMAGE_RESIZE	1
#define ENABLE_RGB2GRAY		1
//#define TIME_FROM_CLOCK
#define ENABLE_DIFF_WRITE_FILE	0

/********************************************************************/
#define FRAME_SKIP_RATE		10
#define MAX_INPUT_ARG		4 //Includes the name of the executable
#define MAX_STRING_LEN		30

using namespace cv;

#ifdef TIME_FROM_CLOCK
double get_process_time(void) 
{
	return (double(std::clock()) * 1000/CLOCKS_PER_SEC);
}
#else
//void get_process_time(struct timeval *t) 
//{
//	gettimeofday(t, NULL);
//}
#endif

int main(int argn, char** argv)
{
	#ifdef TIME_FROM_CLOCK 
	double start_time, vid_load_end_time, end_time;
	#else
	struct timeval start_time, vid_load_end_time, end_time;
	double elapsed_time = 0;
	//clockid_t clk_id = CLOCK_MONOTONIC;
	//struct timespec start_time, vid_load_end_time, end_time;
	#endif

	int frame_skip_rate = FRAME_SKIP_RATE;
	char* vid_file_name;
	char* log_file_loc;
	Scalar tot_sum, max_sum;
	
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
	out_fp << "Video file = " << vid_file_name << ", Frame skip rate = " << frame_skip_rate << std::endl;
	std::cout << "Frame skip rate = " << frame_skip_rate << std::endl;

	#ifdef TIME_FROM_CLOCK	
	start_time = get_process_time();
	#else
	//get_process_time(&start_time);
	gettimeofday(&start_time, NULL);
	//if(!clock_gettime(clk_id, &start_time))
	//{
	//	std::cout << "Clock cannot be recorded " << std::endl;
	//}

	#endif
	VideoCapture cap(vid_file_name);
	
	#ifdef TIME_FROM_CLOCK
	vid_load_end_time = get_process_time();
	#else
	//get_process_time(&end_time);
	gettimeofday(&vid_load_end_time, NULL);
	//if(!clock_gettime(clk_id, &vid_load_end_time))
	//{
	//	std::cout << "Clock cannot be recorded " << std::endl;
	//}
	#endif
	if(!cap.isOpened())
	{
		std::cout << "ERROR: Video cannot be opened\n";
		return -1;
	}
	
	if(!cap.read(prev_image)) // Read one image at the start
	{
		#ifdef TIME_FROM_CLOCK
		end_time = get_process_time();
		#else
		//get_process_time(&end_time);
		gettimeofday(&end_time, NULL);
		//if(!clock_gettime(clk_id, &end_time))
		//{
		//	std::cout << "Clock cannot be recorded " << std::endl;
		//}
		#endif
		cap.release();
		std::cout << "ERROR: Not even one frame \n";
		return -1;
	}	
	cvtColor(prev_image, prev_image, CV_RGB2GRAY);

	#ifdef ENABLE_IMAGE_RESIZE
	resize(prev_image, prev_resized_im, Size(800,600));
	#endif

	for(;;)
	{
		count++;
		if(!cap.read(next_image))
		{
			break;
		}	
		else
		{	
			if(count > frame_skip_rate)
			{
				count = 0;
				#ifdef ENABLE_RGB2GRAY
				cvtColor(next_image, next_image, CV_RGB2GRAY);
				#endif

				#ifdef ENABLE_IMAGE_RESIZE
					resize(next_image, next_resized_im, Size(800,600));
					diff_image = next_resized_im - prev_resized_im;
					#ifdef ENABLE_IMSHOW
					imshow("Resized images",diff_image);
					waitKey(5);
					#endif
					tot_sum = sum(diff_image)/(800*600*255);
					#if (ENABLE_DIFF_WRITE_FILE == 1)
					out_fp << tot_sum[0] << std::endl;
					#endif
					prev_resized_im = next_resized_im.clone();
				#else
					diff_image = next_image - prev_image;
					#ifdef ENABLE_IMSHOW
					imshow("Diff images",diff_image);
					waitKey(5);
					#endif
					tot_sum = sum(diff_image)/(diff_image.cols*diff_image.rows*255);
					#if (ENABLE_DIFF_WRITE_FILE == 1)
					out_fp << tot_sum[0] << std::endl;
					#endif
					prev_image = next_image.clone();
				#endif
			}
		}
	}
	#ifdef TIME_FROM_CLOCK
	end_time = get_process_time();
	#else
	//get_process_time(&end_time);
	gettimeofday(&end_time, NULL);
	//if(!clock_gettime(clk_id, &end_time))
	//{
	//	std::cout << "Clock cannot be recorded " << std::endl;
	//}
	#endif
	cap.release();

	#ifdef TIME_FROM_CLOCK
	out_fp << "Elapsed time " << (end_time - start_time) << std::endl;
	out_fp << "Video load time " << (vid_load_end_time - start_time) << std::endl;
	#else
	elapsed_time = ((end_time.tv_sec - start_time.tv_sec) * 1000) + ((end_time.tv_usec - start_time.tv_usec)/1000); 
	//elapsed_time = ((end_time.tv_sec - start_time.tv_sec) * 1000) + ((end_time.tv_sec - start_time.tv_sec)/1000); 
	out_fp << "Elapsed time " << elapsed_time  << std::endl;
	
	elapsed_time = ((vid_load_end_time.tv_sec - start_time.tv_sec) * 1000) + ((vid_load_end_time.tv_usec - start_time.tv_usec)/1000); 
	out_fp << "Video load time " << elapsed_time  << std::endl;
	#endif
	out_fp.close();
	return 0;
}

//gettimeoftheday for clock, calculate the time for each of of the items, do 10000 cycles, different API for read, grab and retrieve, collect a histogram between frames, make a graph for skip rate 1
