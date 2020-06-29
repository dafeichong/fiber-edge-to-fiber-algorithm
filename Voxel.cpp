#include "Voxel.h"

Voxel::Voxel()
{
}

Voxel::Voxel(int r, int c, int d)
{
	m_data = new int[r * c * d];
	for (int i = 0; i < r * c * d; i++)
	{
		m_data[i] = 0;
	}
	m_rows = r, m_cols = c, m_depth = d;
	m_slice_offset = r * c, m_row_offset = c;
	m_length = r * c * d;
}

Voxel::~Voxel()
{
	delete[] m_data;
}

int * Voxel::Data()
{
	return m_data;
}

void Voxel::ImportData(const cv::Mat & src, const cv::Mat& mask)
{
	int r, c, d;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (mask.at<uchar>(i, j) == 0)
			{
				r = i, c = j;
				d = src.at<uchar>(i, j);
				m_data[d * m_slice_offset + r * m_row_offset + c] = 1;
				m_q.push(Pt(r, c, d));
			}
		}
	}
}

int Voxel::at(int r, int c, int d)
{
	int res = -1;
	res = m_data[d * m_slice_offset + r * m_row_offset + c];
	return res;
}

void Voxel::WriteToTxt(const char * output_filename, const cv::Mat& mask)
{
	std::ofstream output(output_filename);     //打开要写入的文件，如果该文件不存在，则自动新建
	output.precision(1);
	for (int i = 0; i < m_length; i++)
	{
		int d = i / m_slice_offset;
		int r = (i - d * m_slice_offset) / m_row_offset;
		int c = (i - d * m_slice_offset - r * m_row_offset);
		if (mask.empty())
		{
			if (at(r, c, d) != 0)
			{
				output << c << ' ' << r << ' ' << d << std::endl;
			}
		}
		else
		{
			if (at(r, c, d) != 0 && mask.at<uchar>(r, c) == 0)
			{
				output << c << ' ' << r << ' ' << d << std::endl;
			}
		}
	}
}

void Voxel::PointSpread(int cnt)
{
	while (!m_q.empty())
	{
		Pt top = m_q.front();
		m_q.pop();
		for (int i = 0; i < 26; i++)
		{
			int ni = top.row + di[i], nj = top.col + dj[i], nk = top.depth + dk[i];
			if (ni >= 0 && ni < m_rows && nj >= 0 && nj < m_cols && nk >= 0 && nk < m_depth)
			{

			}
		}
	}
}
