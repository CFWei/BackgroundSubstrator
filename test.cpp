#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main(int argc,char **argv)
{
	Mat img=imread("lena.jpg",CV_LOAD_IMAGE_UNCHANGED);
	if(img.empty())
	{
		cout<<"Error:Can not load image"<<endl;
		return -1;
	}
	cv:nameWindows("MyWindow",CV_WINDOW_AUTOSIZE);

	waitKey(0);

	destroyWindows("Windows");
	
}
