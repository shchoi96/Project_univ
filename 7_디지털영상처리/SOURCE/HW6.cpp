#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // ���� ��
    int k = 5;

    // �̹��� ����
    Mat img = imread("�󱼻���.jpg");
    int width = img.cols;
    int height = img.rows;
    int nPoints = width * height;
    
    // �ʱ�ȭ
    Mat points(nPoints, 1, CV_32FC3);   // �Է� ������ 2D -> 1D
    Mat centers(k, 1, CV_32FC3);        // k���� Ŭ������ ��ǥ��
    Mat out(img.size(), img.type());    // ��� ����
    Mat labels;                         // �󺧸� ���� ���� ����
    
    // k means �Լ��� �°� ������ ��ȯ. 2D -> 1D, uchar -> float
    for (int y = 0, n = 0; y < height; y++)
        for (int x = 0; x < width; x++, n++)
        {
            points.at<Vec3f>(n)[0] = img.at<Vec3b>(y, x)[0];
            points.at<Vec3f>(n)[1] = img.at<Vec3b>(y, x)[1];
            points.at<Vec3f>(n)[2] = img.at<Vec3b>(y, x)[2];
        }

    // k-means clustering
    kmeans(points, k, labels, // �Էµ�����, ���� ��, �󺧸�
        TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), // �ִ� 10�� �ݺ��ϸ�, 1.0 ���� ��Ȯ���� �䱸
        3, KMEANS_RANDOM_CENTERS, centers); // �˰����� 3�� �ݺ��� ������ ��ġ�� ã������. �ʱ� ���� �߽ɰ� ���. ��� centers�� ����

    // Ŭ�����͸� ����� �̹����� �����ֱ� ���� ��ȯ
    for (int y = 0, n = 0; y < height; y++)
        for (int x = 0; x < width; x++, n++)
        {
            int cIndex = labels.at<int>(n); // �ش� �ȼ��� ���� ����
            int iTemp = cvRound(centers.at<Vec3f>(cIndex, 0)[0]); // ���� ��ǥ���� �� ����� �ݿø�
            iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;    // 0 ~ 255 ������ ��ȭ
            out.at<Vec3b>(y, x)[0] = (uchar)iTemp; // ��� ���� ����

            iTemp = cvRound(centers.at<Vec3f>(cIndex, 0)[1]);
            iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;
            out.at<Vec3b>(y, x)[1] = (uchar)iTemp;

            iTemp = cvRound(centers.at<Vec3f>(cIndex, 0)[2]);
            iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;
            out.at<Vec3b>(y, x)[2] = (uchar)iTemp;
        }

    imshow("Input", img);
    imshow("K-means Clustering", out);
    waitKey(0);
    return 0;
}