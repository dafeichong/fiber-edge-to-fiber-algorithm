#include <iostream>
#include <opencv2/opencv.hpp>
#include <queue>
using namespace std;
using namespace cv;
#pragma once
class Pt 
{
public:
	Pt(int r, int c, int d)
	{
		row = r, col = c, depth = c;
	}
	int row, col, depth;
};

class Voxel
{
public:
	//ÈýÎ¬ÏòÁ¿
	int di[26] = { -1, -1, -1, 0, 0, 0, 1, 1, 1,
					-1, -1, -1, 0, 0, 1, 1, 1,
					-1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int dj[26] = { -1, 0, 1, -1, 0, 1, -1, 0, -1,
					-1, 0, 1, -1, 1, -1, 0, -1,
					-1, 0, 1, -1, 0, 1, -1, 0, -1 };
	int dk[26] = { 1, 1, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0,
					-1, -1, -1, -1, -1, -1, -1, -1, -1 };
	Voxel();
	Voxel(int r, int c, int d);
	~Voxel();
	int* Data();
	void ImportData(const cv::Mat& src, const cv::Mat& mask);
	int at(int r, int c, int d);
	void WriteToTxt(const char* output_filename, const cv::Mat& mask);
	void PointSpread(int cnt);
	

	int m_rows, m_cols, m_depth;
	int m_slice_offset, m_row_offset;
	int m_length;
private:
	int* m_data;
	queue<Pt> m_q;
};

