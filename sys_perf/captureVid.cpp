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
#define LOG_TYPE_INFO		0

/********************************************************************/
#define FRAME_SKIP_RATE		10
#define MAX_INPUT_ARG		4 //Includes the name of the executable
#define MAX_STRING_LEN		255

using namespace cv;

#ifdef TIME_FROM_CLOCK
double get_process_time(void) 
{
	return (double(std::clock()) * 1000/CLOCKS_PER_SEC);
}
#else

#endif

int main(int argn, char** argv)
{
	#ifdef TIME_FROM_CLOCK 
	double start_time, time_1, time_2, time_3, time_4, time_5, time_6, end_time;
	#else
	struct timeval start_time, time_1, time_2, time_3, time_4, time_5, time_6, end_time;
	double elapsed_time = 0;
	#endif

	int frame_skip_rate = FRAME_SKIP_RATE;
	char* vid_file_name;
	char* log_file_loc;
	Scalar tot_sum, max_sum;
	unsigned long long num_of_frames = 0;

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
	std::cout << "Frame skip rate = " << frame_skip_rate << std::endl;

	#ifdef TIME_FROM_CLOCK	
	start_time = get_process_time();
	#else
	gettimeofday(&start_time, NULL);
	#endif
	VideoCapture cap(vid_file_name);
	
	#ifdef TIME_FROM_CLOCK
	time_1 = get_process_time();
	#else
	gettimeofday(&time_1, NULL);
	#endif
	if(!cap.isOpened())
	{
		std::cout << "ERROR: Video cannot be opened\n";
		return -1;
	}
	#ifdef TIME_FROM_CLOCK	
	time_2 = get_process_time();
	#else
	gettimeofday(&time_2, NULL);
	#endif
	
	//if(!cap.read(prev_image)) // Read one image at the start
	if(!cap.grab())
	{
		#ifdef TIME_FROM_CLOCK
		end_time = get_process_time();
		#else
		gettimeofday(&end_time, NULL);
		#endif
		cap.release();
		std::cout << "ERROR: Couldn't grab the first frame \n";
		return -1;
	}

	if(!cap.retrieve(prev_image, 0))
	{
		#ifdef TIME_FROM_CLOCK
		end_time = get_process_time();
		#else
		gettimeofday(&end_time, NULL);
		#endif
		cap.release();
		std::cout << "ERROR: Couldn't retrive the first frame \n";
		return -1;

	}
	num_of_frames++;
	
	#ifdef TIME_FROM_CLOCK	
	time_3 = get_process_time();
	#else
	gettimeofday(&time_3, NULL);
	#endif
	
	cvtColor(prev_image, prev_image, CV_RGB2GRAY);
	
	#ifdef TIME_FROM_CLOCK	
	time_4 = get_process_time();
	#else
	gettimeofday(&time_4, NULL);
	#endif

	#ifdef ENABLE_IMAGE_RESIZE
	resize(prev_image, prev_resized_im, Size(800,600));
	#endif
	
	#ifdef TIME_FROM_CLOCK	
	time_5 = get_process_time();
	#else
	gettimeofday(&time_5, NULL);
	#endif

	for(;;)
	{
		count++;
		//if(!cap.read(next_image))
		if(!cap.grab())
		{
			break;
		}	
		else
		{	
			if(count >= frame_skip_rate)
			{
				if(!cap.retrieve(next_image, 0))
				{
					break;
				}
				else
				{
				count = 0;
				num_of_frames++;
				#ifdef ENABLE_RGB2GRAY
				cvtColor(next_image, next_image, CV_RGB2GRAY);
				#endif

				#ifdef ENABLE_IMAGE_RESIZE
					resize(next_image, next_resized_im, Size(800,600));
					diff_image = next_resized_im - prev_resized_im;
					#ifdef ENABLE_IMSHOW
					imshow("Resized images",diff_image);
					waitKey(1);
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
					waitKey(1);
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
	}
	#ifdef TIME_FROM_CLOCK
	end_time = get_process_time();
	#else
	gettimeofday(&end_time, NULL);
	#endif
	cap.release();

	#ifdef TIME_FROM_CLOCK
	out_fp << "Num of Frames = " << num_of_frames << std::endl;
	out_fp << " Total time = " << (end_time - start_time) << ", Video Load = "<< (time_1 - start_time) << ", Video Open Check = " << (time_2 - time_1) << ", Im read = " << (time_3 - time_2) << ", RGB 2 Gray = " << (time_4 - time_3) << ", Resize = "<< (time_5 - time_4) << std::endl;
	#else
	#if (LOG_TYPE_INFO == 1)
	out_fp << "Num of Frames = " << num_of_frames << std::endl;
	elapsed_time = ((end_time.tv_sec - start_time.tv_sec) * 1000) + ((end_time.tv_usec - start_time.tv_usec)/1000); 
	out_fp << "Total time = " << elapsed_time ;
	
	elapsed_time = ((time_1.tv_sec - start_time.tv_sec) * 1000) + ((time_1.tv_usec - start_time.tv_usec)/1000); 
	out_fp << ", Video load = " << elapsed_time ;
	
	elapsed_time = ((time_2.tv_sec - time_1.tv_sec) * 1000) + ((time_2.tv_usec - time_1.tv_usec)/1000); 
	out_fp << ", Video Open Check = " << elapsed_time;
	
	elapsed_time = ((time_3.tv_sec - time_2.tv_sec) * 1000) + ((time_3.tv_usec - time_2.tv_usec)/1000); 
	out_fp << ", Im read = " << elapsed_time;
	
	elapsed_time = ((time_4.tv_sec - time_3.tv_sec) * 1000) + ((time_4.tv_usec - time_3.tv_usec)/1000); 
	out_fp << ", RGB 2 Gray = " << elapsed_time;
	
	elapsed_time = ((time_5.tv_sec - time_4.tv_sec) * 1000) + ((time_5.tv_usec - time_4.tv_usec)/1000); 
	out_fp << ", Resize = " << elapsed_time  << std::endl;
	#else
	elapsed_time = ((end_time.tv_sec - start_time.tv_sec) * 1000) + ((end_time.tv_usec - start_time.tv_usec)/1000); 
	out_fp << elapsed_time << " ";
	
	elapsed_time = ((time_1.tv_sec - start_time.tv_sec) * 1000) + ((time_1.tv_usec - start_time.tv_usec)/1000); 
	out_fp << elapsed_time << " ";
	
	elapsed_time = ((time_2.tv_sec - time_1.tv_sec) * 1000) + ((time_2.tv_usec - time_1.tv_usec)/1000); 
	out_fp << elapsed_time << " " ;
	
	elapsed_time = ((time_3.tv_sec - time_2.tv_sec) * 1000) + ((time_3.tv_usec - time_2.tv_usec)/1000); 
	out_fp << elapsed_time << " " ;
	
	elapsed_time = ((time_4.tv_sec - time_3.tv_sec) * 1000) + ((time_4.tv_usec - time_3.tv_usec)/1000); 
	out_fp << elapsed_time << " " ;
	
	elapsed_time = ((time_5.tv_sec - time_4.tv_sec) * 1000) + ((time_5.tv_usec - time_4.tv_usec)/1000); 
	out_fp << elapsed_time  << std::endl;

	#endif
	#endif
	out_fp.close();
	return 0;
}

