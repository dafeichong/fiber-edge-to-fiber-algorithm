#include <iostream>
#include<opencv2/opencv.hpp>
#include "Voxel.h"

using namespace std;
using namespace cv;




int main()
{
	Mat depth_map = imread("full_depth.bmp");
	Mat mask = imread("new_skeleton.bmp");
	resize(depth_map, depth_map, cv::Size(520, 696));
	resize(mask, mask, cv::Size(520, 696));
	cvtColor(depth_map, depth_map, CV_BGR2GRAY);
	cvtColor(mask, mask, CV_BGR2GRAY);
	threshold(mask, mask, 100, 255, CV_THRESH_BINARY);
	imwrite("mask.bmp", mask);
	imwrite("depth.bmp", depth_map);
	int rows = depth_map.rows;
	int cols = depth_map.cols;
	int depth = 120;
	Voxel voxel(rows, cols, depth);
	voxel.ImportData(depth_map, mask);
	voxel.PointSpread(5);
	//voxel.Reverse();
	voxel.Voxel2Slice();
	
	//voxel.WriteToTxt("1.txt", cv::Mat());
	cout << voxel.CalPoreVolume() << endl;


	system("pause");
	return 0;
}