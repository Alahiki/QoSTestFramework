#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/dnn.hpp>

using namespace cv;
using namespace cv::ml;
using namespace cv::dnn;
using namespace std;

#include <map>
#include <unistd.h>
#include <stdio.h>

#define ND 4

void help()
{
    cout << endl;
    cout << "/////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << "This program saves frames received" << endl;
    cout << "If you want to run it in terminal instead of basicServer, please cd to mcu-bench_cpp folder and use ./native/xxx" << endl;
    cout << "USAGE: ./saveImage rawdata" << endl;
    cout << "For example: ./native/saveImage ./native/Data/localARGB.txt " << endl;
    cout << "The output files will be saved in ./native/output" << endl;
    cout << "/////////////////////////////////////////////////////////////////////////////////" << endl << endl;
}

int main(int argc, char *argv[])
{

    ifstream received_video(argv[1]);

    int v(0);
    unsigned int r, g, b;

    long t(0);//get timestamp from file "mixRawFile"
    int a1, r1, g1, b1;//get ARGB from file "mixRawFile"
    int framecount(0);

    int width(1280);
    int height(720);
    int overFlag(0);

    char c;//get ',' from file "mixRawFile"

//////////////////////////////////////////////////////////Load data ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int isFirstData = 0;


    for(int f = 0;;f++)
    {               
        if(overFlag == 1)//the whole file is over 
        {
            overFlag = 0;
            //cout<<"over!"<<endl;
            break;
        }
        if(overFlag == 2)//one frame over
        {
            overFlag = 0;
        }
        Mat image(height, width, CV_8UC3);

        if (isFirstData == 0)
        {
            isFirstData = 1;
            received_video >> c;//get first ','
        }
        received_video >> t;//get a frame's timestamp
        received_video >> c;

        for(int i = 0;i < height;i++)
        {
            for(int j = 0;j < width;j++)
            {
                framecount++;
                received_video >> v;
                b1 = v;
                received_video >> c;
                received_video >> v;
                g1 = v;
                received_video >> c;
                received_video >> v;
                r1 = v;
                received_video >> c;
                received_video >> v;
                a1 = v;
                received_video >> c;
                //cout << a1 << " " << r1 << " " << g1 << " " << b1 << " " << endl;

                image.at<Vec3b>(i, j)[0] = b1;
                image.at<Vec3b>(i, j)[1] = g1;
                image.at<Vec3b>(i, j)[2] = r1;

                if ((c == 'f'))
                {
                    //cout<<"occ1"<<endl;
                    framecount = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        received_video >> c;
                    }
                    received_video >> c;//get next char,such as ',' or 'e'.
                    if (c == ' ')
                    {
                        //cout<<"occ2"<<endl;
                        overFlag = 1;//file over
                        break;
                    }
                    if (received_video.eof())//the situation:the file over at 'frame'
                    {
                        //cout<<"occ3"<<endl;
                        overFlag = 1;//file over
                        break;
                    }
                    else
                    {
                        //cout<<"occ4"<<endl;
                        overFlag = 2;//one frame over
                        break;
                    }
                }
                if (received_video.eof())//the situation:the file over at ' '
                {
                    //cout<<"occ5"<<endl;
                    overFlag = 1;//file over
                    break;
                }
            }
            if (overFlag >= 1)
            {
                //cout<<"occ6"<<endl;
                break;//break to do iq task when a frame over or the whole file over.(maybe a img didn't trans completely,so must make a force break.)
            }
        }
               // imshow("frame", image);
               // waitKey(3000);
        char path[255];
        sprintf(path,"./output/%d.bmp", f);
        imwrite(path,image);
        if(received_video.eof())
        {
            break;
        }
    }
    return 0;
}