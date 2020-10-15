#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

enum class BoundaryType
{
	ZERO,
	REPETITION,
	MIRRORING
};

Mat filtering(Mat img, int n, vector<vector<double>> filter);
vector<vector<double>> MovingAverage(int n);
vector<vector<double>> Laplacian(int n);
vector<vector<double>> SharpeningFilter(int n);
void PrintFilter(int n, vector<vector<double>>& filter);
int SumFilter(int n, vector<vector<double>>& filter);

int main()
{
	Mat img = imread("lena.png");

	int n, f;
	cout << "필터크기(2n+1) n : ";
	cin >> n;
	cout << "\n----------\n";
	cout << "1 : Moving Average\n";
	cout << "2 : Laplacian\n";
	cout << "3 : Sharpening Filter\n";
	cout << "Filter : ";
	cin >> f;

	vector<vector<double>> filter;
	if (f == 1)
		filter = MovingAverage(n);
	else if (f == 2)
		filter = Laplacian(n);
	else if (f == 3)
		filter = SharpeningFilter(n);

	PrintFilter(n, filter);
	cvtColor(img, img, cv::COLOR_RGB2GRAY);	// 회색으로 변환
	Mat img_out = filtering(img, n, filter);
	imshow("Output", img_out);
	imshow("Input", img);

	waitKey(0);
	return 0;
}

vector<vector<double>> MovingAverage(int n)
{
	return vector<vector<double>>(2 * n + 1, vector<double>(2 * n + 1, 1));
}

vector<vector<double>> Laplacian(int n)
{
	vector<vector<double>> out(2 * n + 1, vector<double>(2 * n + 1, 1));
	out[n][n] -= (2 * n + 1) * (2 * n + 1);
	return out;
}

vector<vector<double>> SharpeningFilter(int n)
{
	vector<vector<double>> Lap = Laplacian(n);
	vector<vector<double>> out(2 * n + 1, vector<double>(2 * n + 1, 0));
	out[n][n] = 1;
	for (int i = 0; i < 2 * n + 1; i++)
		for (int j = 0; j < 2 * n + 1; j++)
			out[i][j] -= Lap[i][j];
	return out;
}

Mat filtering(Mat img, int n, vector<vector<double>> filter)
{
	BoundaryType bt = BoundaryType::MIRRORING;
	int channel = img.channels();
	int width = img.size().width;
	int height = img.size().height;
	int sum = SumFilter(n, filter);

	Mat out(img.size(), img.type());
	uchar* pOutData = out.data;
	uchar* pData = img.data;
	for (int k = 0; k < width * height; k++)
	{
		int col = k % width;
		int row = k / width;
		for (int c = 0; c < channel; c++)
		{
			double ret = 0;
			for (int i = -n; i <= n; i++)
				for (int j = -n; j <= n; j++)
				{
					int x = col + i;
					int y = row + j;
					if (0 <= x && x < width && 0 <= y && y < height)
						ret += filter[i + n][j + n] * (double)pData[channel * (x + width * y) + c];
					else
						switch (bt)
						{
						case BoundaryType::ZERO:
							// Nothing
							break;
						case BoundaryType::REPETITION:
							x = max(0, x);
							y = max(0, y);
							ret += filter[i + n][j + n] * (double)pData[channel * (x + width * y) + c];
							break;
						case BoundaryType::MIRRORING:
							if (x < 0)
								x = abs(x);
							else if (x >= width)
								x = 2 * width - x - 2;
							if (y < 0)
								y = abs(y);
							else if (y >= height)
								y = 2 * height - y - 2;
							ret += filter[i + n][j + n] * (double)pData[channel * (x + width * y) + c];
							break;
						}
				}
			if (sum > 1)
				ret /= sum;
			if (ret > 255)
				ret = 255;
			else if (ret < 0)
				ret = 0;
			pOutData[channel * k + c] = ret;
		}
	}
	return out;
}

void PrintFilter(int n, vector<vector<double>>& filter)
{
	cout << "----Filter----\n";
	for (int i = 0; i < 2 * n + 1; i++)
	{
		for (int j = 0; j < 2 * n + 1; j++)
			cout << filter[i][j] << " ";
		cout << '\n';
	}
}

int SumFilter(int n, vector<vector<double>>& filter)
{
	int sum = 0;
	for (int i = 0; i < 2 * n + 1; i++)
		for (int j = 0; j < 2 * n + 1; j++)
			sum += filter[i][j];
	return sum;
}