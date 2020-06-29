#include <iostream>
#include<opencv2/opencv.hpp>
#include "Voxel.h"

using namespace std;
using namespace cv;




int main()
{
	Mat depth_map = imread("depth_map.jpg");
	Mat mask = imread("projection.jpg");
	cvtColor(depth_map, depth_map, CV_BGR2GRAY);
	cvtColor(mask, mask, CV_BGR2GRAY);
	threshold(mask, mask, 100, 255, CV_THRESH_BINARY);
	int rows = depth_map.rows;
	int cols = depth_map.cols;
	int depth = 120;
	Voxel voxel(rows, cols, depth);
	voxel.ImportData(depth_map, mask);
	int a = voxel.at(20, 25, 45);
	voxel.WriteToTxt("1.txt", cv::Mat());


	system("pause");
	return 0;
}