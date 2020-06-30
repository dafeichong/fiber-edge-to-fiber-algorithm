#include "Voxel.h"

Voxel::Voxel()
{
}

Voxel::Voxel(int r, int c, int d)
{
	m_data = new int[r * c * d];
	m_used = new bool[r * c * d];
	for (int i = 0; i < r * c * d; i++)
	{
		m_data[i] = 0;
		m_used[i] = false;
	}
	m_rows = r, m_cols = c, m_depth = d;
	m_slice_offset = r * c, m_row_offset = c;
	m_length = r * c * d;
	
}

Voxel::~Voxel()
{
	delete[] m_data;
	delete[] m_used;
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
			if (mask.at<uchar>(i, j) == 255)
			{
				r = i, c = j;
				d = src.at<uchar>(i, j);
				m_data[d * m_slice_offset + r * m_row_offset + c] = 1;
				m_used[d * m_slice_offset + r * m_row_offset + c] = true;
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
	while (cnt > 0 && !m_q.empty())
	{
		int count = m_q.size();
		cnt--;
		while (count > 0)
		{
			Pt top = m_q.front();
			m_q.pop();
			count--;
			//搜索周围26领域的体素
			for (int i = 0; i < 26; i++)
			{
				int ni = top.row + di[i], nj = top.col + dj[i], nk = top.depth + dk[i];
				if (ni >= 0 && ni < m_rows && nj >= 0 && nj < m_cols && nk >= 0 && nk < m_depth
					&& !m_used[nk * m_slice_offset + ni * m_row_offset + nj])
				{
					int offset = nk * m_slice_offset + ni * m_row_offset + nj;
					//cout << m_data[offset] << endl;
					m_data[offset] = 1;
					//cout << m_data[offset] << endl;
					m_q.push(Pt(ni, nj, nk));
					m_used[offset] = true;
				}
			}
		}
	}
}

void Voxel::Voxel2Slice(const char* output_filename)
{
	for (int k = 0; k < m_depth; k++)
	{
		cv::Mat mat = cv::Mat::zeros(m_rows, m_cols, CV_8UC1);
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				if(m_data[k * m_slice_offset + i * m_row_offset + j] == 1)
					mat.at<uchar>(i, j) = 255;
			}
		}
		std::string name = output_filename + cv::format("%d.jpg", k);
		if (!cv::imwrite(name, mat))
		{
			cout << "save error!" << endl;
			return;
		}
	}
}

void Voxel::Reverse()
{
	for (int i = 0; i < m_length; i++)
	{
		if (m_data[i] == 0)
			m_data[i] = 1;
		else
			m_data[i] = 0;
	}
}
