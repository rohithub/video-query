#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <vector>
#include "opencv2/imgcodecs.hpp"
#include <string>   // for strings
//#include <opencv2/highgui/highgui.hpp>  // Video write

//Pre-processor directives
//#define ENABLE_RGB2GRAY
//------------------------------------------------------

#define IM_RESIZE_W	400
#define IM_RESIZE_H	400

using namespace std;
using namespace cv;

static void help()
{
    cout
        << "------------------------------------------------------------------------------" << endl
        << "This program shows how to write video files."                                   << endl
        << "You can extract the R or G or B color channel of the input video."              << endl
        << "Usage:"                                                                         << endl
        << "./VidConvert inputvideoName                      "                              << endl
        << "------------------------------------------------------------------------------" << endl
        << endl;
}

int main(int argc, char *argv[])
{
    help();

    if (argc != 2)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    const string source      = argv[1];           // the source file name

    VideoCapture inputVideo(source);              // Open input
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << source << endl;
        return -1;
    }

    string::size_type pAt = source.find_last_of('.');                  // Find extension point
    const string NAME = source.substr(0, pAt) + "conv" + ".avi";   // Form the new name with container
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

   // Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
   //               (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    Size S = Size((int)IM_RESIZE_W,    // Acquire input size
                  (int)IM_RESIZE_H);

    VideoWriter outputVideo;                                        // Open the output
    //if (askOutputType)
    //    outputVideo.open(NAME, ex=-1, inputVideo.get(CV_CAP_PROP_FPS), S, true);
    //else
        outputVideo.open(NAME, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << source << endl;
        return -1;
    }

    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
         << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << EXT << endl;

    Mat src, res;
    vector<Mat> spl;

    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read
        if (src.empty()) break;         // check if at end

	resize(src,src, Size(IM_RESIZE_W, IM_RESIZE_H));
	#ifdef ENABLE_RGB2GRAY
		cvtColor(src, src, CV_RGB2GRAY);
	#endif

       //outputVideo.write(res); //save or
       outputVideo << src;
    }
	inputVideo.release();
	outputVideo.release();
    cout << "Finished writing" << endl;
    return 0;
}

