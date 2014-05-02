#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

Mat frame;
Mat fgMaskMOG;

BackgroundSubtractorMOG MOG;
int keyboard;
int flagList[1000][1000]={0};
class PointClass{
	public:
		int row_num;
		int col_num;
};

class PointRange{
	public:
		PointRange();
		int row_upperBound;
		int row_lowerBound;

		int col_upperBound;
		int col_lowerBound;
};

PointRange::PointRange(){
	row_upperBound=-1;
	row_lowerBound=-1;

	col_upperBound=-1;
	col_lowerBound=-1;

}

vector<PointRange> object;


PointRange findRange(PointClass startPoint,int **matValue,int nRows,int nCols,int findNum,int direction){

	PointRange returnRange;
	int row_num=startPoint.row_num;
	int col_num=startPoint.col_num;

	if(matValue[row_num][col_num]==255){
		
		PointRange rangeList[4];
		
		//Find the upper point
		if(row_num>0&&flagList[row_num-1][col_num]==0)
		{		
			flagList[row_num-1][col_num]=1;

			PointClass up_point;
			up_point.row_num=row_num-1;
			up_point.col_num=col_num;
			rangeList[0]=findRange(up_point,matValue,nRows,nCols,findNum,1);
		}

		//Find the down point
		if(row_num<nRows&&flagList[row_num+1][col_num]==0)
		{	
			flagList[row_num+1][col_num]=1;

			PointClass down_point;
			down_point.row_num=row_num+1;
			down_point.col_num=col_num;
			rangeList[1]=findRange(down_point,matValue,nRows,nCols,findNum,2);
		}

		//Find the left point
		if(col_num>0&&flagList[row_num][col_num-1]==0)
		{	
			flagList[row_num][col_num-1]=1;

			PointClass left_point;
			left_point.row_num=row_num;
			left_point.col_num=col_num-1;
			rangeList[2]=findRange(left_point,matValue,nRows,nCols,findNum,3);
		
		}
		
		//Find the right point
		if(col_num<nCols&&flagList[row_num][col_num+1]==0)
		{
			flagList[row_num][col_num+1]=1;

			PointClass right_point;
			right_point.row_num=row_num;
			right_point.col_num=col_num+1;
			rangeList[3]=findRange(right_point,matValue,nRows,nCols,findNum,4);
		
		}
		
		//Determine Row upper Bound

		vector<int> row_upperBoundList,row_lowerBoundList,col_upperBoundList,col_lowerBoundList;
		for(int i=0;i<3;i++){
			
			if(rangeList[i].row_upperBound!=-1)
			{
				row_upperBoundList.push_back(rangeList[i].row_upperBound);
				row_lowerBoundList.push_back(rangeList[i].row_lowerBound);
				
				col_upperBoundList.push_back(rangeList[i].col_upperBound);
				col_lowerBoundList.push_back(rangeList[i].col_lowerBound);
			
			}
		
		}
		

		//Add own's point
		row_upperBoundList.push_back(row_num);
		row_lowerBoundList.push_back(row_num);

		col_upperBoundList.push_back(col_num);
		col_lowerBoundList.push_back(col_num);



		returnRange.row_upperBound=*max_element(row_upperBoundList.begin(),row_upperBoundList.end());


		returnRange.row_lowerBound=*min_element(row_lowerBoundList.begin(),row_lowerBoundList.end());

		

		returnRange.col_upperBound=*max_element(col_upperBoundList.begin(),col_upperBoundList.end());


		returnRange.col_lowerBound=*min_element(col_lowerBoundList.begin(),col_lowerBoundList.end());
		


		return returnRange;
		
	}
	else{
		
		returnRange.row_upperBound=row_num;
		returnRange.row_lowerBound=row_num;

		returnRange.col_lowerBound=col_num;
		returnRange.col_upperBound=col_num;


		return returnRange;



	}

}


void initialFlag(){
	for(int i=0;i<1000;i++)
		for(int j=0;j<1000;j++)
			flagList[i][j]=0;


}


void foundObject(int **matValue,int nRows,int nCols){
	
	//Save the point which pixel value is 255
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
	int count =0;	
	while(pointGroup.size()!=0){
		
		vector<PointClass>::iterator startPoint=pointGroup.begin();
		
		int start_row_num=(*startPoint).row_num;
		int start_col_num=(*startPoint).col_num;
		PointClass s1;
		s1.row_num=start_row_num;
		s1.col_num=start_col_num;
		cout<<"Start Point:"<<start_row_num<<","<<start_col_num<<":"<<matValue[start_row_num][start_col_num]<<endl;


		initialFlag();
		PointRange r=findRange(s1,matValue,nRows,nCols,0,0);

		int min_row_num=r.row_lowerBound;
		int max_row_num=r.row_upperBound;
		
		int min_col_num=r.col_lowerBound;
		int max_col_num=r.col_upperBound;
		

		if(min_row_num!=max_row_num||min_col_num!=max_col_num)
		{
			cout<<"Find Object:"<<min_row_num<<","<<max_row_num<<";"<<min_col_num<<","<<max_col_num<<endl;
			PointRange ob;
			ob.row_upperBound=min_row_num;
			ob.row_lowerBound=max_row_num;

			ob.col_lowerBound=min_col_num;
			ob.col_upperBound=max_col_num;
			
			object.push_back(ob);
			count++;
		}



		for(vector<PointClass>::iterator iter=pointGroup.begin();
				iter!=pointGroup.end();
				++iter){
			
			int row_num=(*iter).row_num;
			int col_num=(*iter).col_num;
			if(row_num>=min_row_num && row_num<=max_row_num && col_num >=min_col_num && col_num <=max_col_num){
				--(iter=pointGroup.erase(iter));
			}


		}


		
	}

	//	cout<<"PointSize:"<<pointGroup.size()<<endl;
	cout<<"Find "<<count<<" Object"<<endl;

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
	
	
	for(vector<PointRange>::iterator it =object.begin();it!=object.end();it++)
	{
		Point leftUp=Point((*it).col_lowerBound,(*it).row_lowerBound);
		Point rightDown=Point((*it).col_upperBound,(*it).row_upperBound);
		rectangle(lastFrame,rightDown,leftUp,Scalar(255,0,0));
	}





	imshow("Frame",lastFrame);
	imshow("FG MASK MOG", fgMaskMOG );


	waitKey(0);


	return 0;
}



