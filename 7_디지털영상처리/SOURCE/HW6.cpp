#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // 군집 수
    int k = 5;

    // 이미지 정보
    Mat img = imread("얼굴사진.jpg");
    int width = img.cols;
    int height = img.rows;
    int nPoints = width * height;
    
    // 초기화
    Mat points(nPoints, 1, CV_32FC3);   // 입력 데이터 2D -> 1D
    Mat centers(k, 1, CV_32FC3);        // k개의 클러스터 대표값
    Mat out(img.size(), img.type());    // 결과 영상
    Mat labels;                         // 라벨링 정보 저장 공간
    
    // k means 함수에 맞게 데이터 변환. 2D -> 1D, uchar -> float
    for (int y = 0, n = 0; y < height; y++)
        for (int x = 0; x < width; x++, n++)
        {
            points.at<Vec3f>(n)[0] = img.at<Vec3b>(y, x)[0];
            points.at<Vec3f>(n)[1] = img.at<Vec3b>(y, x)[1];
            points.at<Vec3f>(n)[2] = img.at<Vec3b>(y, x)[2];
        }

    // k-means clustering
    kmeans(points, k, labels, // 입력데이터, 군집 수, 라벨링
        TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), // 최대 10번 반복하며, 1.0 내의 정확도를 요구
        3, KMEANS_RANDOM_CENTERS, centers); // 알고리즘을 3번 반복해 최적의 위치를 찾고자함. 초기 랜덤 중심값 사용. 결과 centers에 저장

    // 클러스터링 결과를 이미지로 보여주기 위한 변환
    for (int y = 0, n = 0; y < height; y++)
        for (int x = 0; x < width; x++, n++)
        {
            int cIndex = labels.at<int>(n); // 해당 픽셀이 속한 군집
            int iTemp = cvRound(centers.at<Vec3f>(cIndex, 0)[0]); // 군집 대표값의 색 결과를 반올림
            iTemp = iTemp > 255 ? 255 : iTemp < 0 ? 0 : iTemp;    // 0 ~ 255 범위로 변화
            out.at<Vec3b>(y, x)[0] = (uchar)iTemp; // 결과 영상에 저장

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