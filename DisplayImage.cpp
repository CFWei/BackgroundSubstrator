#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <vector>


using namespace cv;
using namespace std;

Mat frame;
Mat fgMaskMOG;

BackgroundSubtractorMOG MOG;
int keyboard;

class PointClass{
	public:
		int row_num;
		int col_num;
};
class PointRange{
	public:
		int row_upperBound;
		int row_lowerBound;

		int col_upperBound;
		int col_lowerBound;


};


PointRange findRange(PointClass startPoint){
	
	int row_num=startPoint.row_num;
	int col_num=startPoint.col_num;
	
	PointRange up=findRange()



}



void foundObject(int **matValue,int nRows,int nCols){
	vector<PointClass> pointGroup;

	for(int i=0;i<nRows;i++){
		for(int j=0;j<nCols;j++){
			if(matValue[i][j]==255){
				PointClass newPoint;
				newPoint.row_num=i;
				newPoint.col_num=j;

				pointGroup.push_back(newPoint);
			
			}
		}
	}

	while(pointGroup.size()!=0){
		
		vector<PointClass>::iterator startPoint=pointGroup.begin();
		
		int start_row_num=(*startPoint).row_num;
		int start_col_num=(*startPoint).col_num;
		while(1){
				
		
		
		
		}

		int min_row_num=0;
		int max_row_num=nRows;
		
		int min_col_num=0;
		int max_col_num=nCols;
		
		for(vector<PointClass>::iterator iter=pointGroup.begin();
				iter!=pointGroup.end();
				++iter){
			
			int row_num=(*iter).row_num;
			int col_num=(*iter).col_num;
			if(row_num>=min_row_num && row_num<=max_row_num && col_num >=min_col_num && col_num <=max_col_num){
				--(iter=pointGroup.erase(iter));
			}


		}
		break;
		
	}



	cout<<"PointSize:"<<pointGroup.size()<<endl;
	
	




}


int main(int argc, char** argv )
{	
	String firstFilename="1.png";


	namedWindow("Frame", WINDOW_AUTOSIZE );
	namedWindow("FG MASK MOG",WINDOW_AUTOSIZE);

	frame = imread(firstFilename);

	if(!frame.data)
	{
		cerr << "Unable to open first image frame: " << firstFilename << endl;
		exit(EXIT_FAILURE);
	}

	String fn(firstFilename);
	int count=0;
	while( (char)keyboard != 'q' && (char)keyboard != 27 &&count<2 ){	
	    
		//Update Background Model
		MOG(frame , fgMaskMOG );
		size_t index=fn.find_last_of("/");

		//Get the frame number and write it on the current frame
		if(index==string::npos){
			index=fn.find_last_of("\\");
		}

		size_t index2 = fn.find_last_of(".");
		string prefix = fn.substr(0,index+1);
		string suffix = fn.substr(index2);

		string frameNumberString = fn.substr(index+1, index2-index-1);

		istringstream iss(frameNumberString);
		int frameNumber = 0;
		iss >> frameNumber;

		rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
				cv::Scalar(255,255,255), -1);
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
				FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

		//imshow("Frame", frame);
  	    //imshow("FG MASK MOG", fgMaskMOG);
		ostringstream oss;
		oss << (frameNumber + 1);
		string nextFrameNumberString = oss.str();
		string nextFrameFilename = prefix + nextFrameNumberString + suffix;

		frame = imread(nextFrameFilename);
		fn.assign(nextFrameFilename);
		count++;
	}
	
	Mat lastFrame=imread("2.png");

	int mask_channels=fgMaskMOG.channels();
	int mask_nRows=fgMaskMOG.rows;
	int mask_nCols=fgMaskMOG.cols*mask_channels;


	cout<<"===Mask==="<<endl;
	cout<<"Channel:"<<mask_channels<<endl;
	cout<<"Rows:"<<mask_nRows<<endl;
	cout<<"Columns:"<<mask_nCols<<endl;
	
	
	int **maskValue;
	maskValue=new int *[mask_nRows];
	for(int i=0;i<mask_nRows;i++){
		maskValue[i]=new int[mask_nCols];
	}


	for(int i=0;i<mask_nRows;i++)
	{
		for(int j=0;j<mask_nCols;j++)
		{	
			int value=(int)fgMaskMOG.at<uchar>(i,j);
			maskValue[i][j]=value;
			if(value!=0)
			{	

				//cout<<"("<<i<<","<<j<<"):"<<value<<endl;
				/*
				fgMaskMOG.at<uchar>(i,j)=100;
				lastFrame.at<uchar>(i,j*3)=0;
				lastFrame.at<uchar>(i,j*3+1)=0;
				lastFrame.at<uchar>(i,j*3+2)=0;
				*/
			}
		
		}
	
	}
	
	
	foundObject(maskValue,mask_nRows,mask_nCols);



	
	Mat firstFrame=imread(firstFilename);
	int firstFrame_channel=firstFrame.channels();
	int firstFrame_nRows=firstFrame.rows;
	int firstFrame_nCols=firstFrame.cols*firstFrame_channel;
	
	cout<<"===First File==="<<endl;
	cout<<"Channel:"<<firstFrame_channel<<endl;
	cout<<"Rows:"<<firstFrame_nRows<<endl;
	cout<<"Columns:"<<firstFrame_nCols<<endl;
	
	

	imshow("Frame",lastFrame);
	imshow("FG MASK MOG", fgMaskMOG );
	
	waitKey(0);


	return 0;
}



