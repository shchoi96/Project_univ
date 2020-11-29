#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // 비교할 이미지
    Mat img1 = imread("4.jpg", IMREAD_GRAYSCALE);
    Mat img2 = imread("3.jpg", IMREAD_GRAYSCALE);
    
    // SIFT 알고리즘에 필요한 변수
    Ptr<SIFT> ext = SIFT::create();
    vector<KeyPoint> kp1, kp2;
    Mat desc1, desc2;

    // SIFT 알고리즘을 통해 특징점과 Descriptor 계산
    // Scale-space Extrema Detection
    // Keypoint Localization
    // Orientation Assignment
    // Keypoint Descriptor 
    // 위 네 단계 과정을 수행
    ext->detectAndCompute(img1, Mat(), kp1, desc1);
    ext->detectAndCompute(img2, Mat(), kp2, desc2);

    // Matching에 필요한 변수
    vector< vector<DMatch> > rawMatches;
    vector<Point2f> p1, p2;
    vector<float> distances;
    
    // 두 이미지에서 추출한 특징점을 비교해 매칭하는 변수
    Ptr<DescriptorMatcher> matcher;
    
    // 매칭 기법 중 Flann(Fast Library approximate nearest neighbors)을
    // 근간으로 한 알고리즘을 이용
    matcher = DescriptorMatcher::create("FlannBased");
    
    // 두 Descriptor 중 가장 일치하는 k=2개를 rawMatches에 반환
    // {1순위, 2순위}
    matcher->knnMatch(desc1, desc2, rawMatches, 2);

    // 2순위 매칭 결과의 0.75배보다 더 가까운 1순위 값만 사용
    for (size_t i = 0; i < rawMatches.size(); i++)
    {
        const vector<DMatch>& m = rawMatches[i];
        if (m.size() == 2 && m[0].distance < m[1].distance * 0.75)
        {
            // queryIdx, trainIdx : 매칭된 desc1와 desc2의 index
            p1.push_back(kp1[m[0].queryIdx].pt);
            p2.push_back(kp2[m[0].trainIdx].pt);
            distances.push_back(m[0].distance);
        }
    }

    // 한 평면을 다른 평면에 투영시켰을 때
    // 투영된 대응점들 사이의 일정한 변환관계 구하기
    // RANSAC : 무작위로 샘플 데이터를 뽑은 다음 최대로 합의된 것 선택
    vector<uchar> status;
    vector< pair<Point2f, Point2f> > pointPairs;
    Mat H = findHomography(p1, p2, status, RANSAC);
    // RANSAC이 선택한 모델에서 크게 벗어나지 않는 데이터 구별
    int inliers = 0;
    for (size_t i = 0; i < status.size(); i++)
    {
        if (status[i])
        {
            pointPairs.push_back(make_pair(p1[i], p2[i]));
            inliers++;
        }
    }

    // 출력 이미지 vis 에 두 이미지 나열
    int h1 = img1.size().height;
    int w1 = img1.size().width;
    int h2 = img2.size().height;
    int w2 = img2.size().width;
    Mat vis = Mat::zeros(max(h1, h2), w1 + w2, CV_8U);
    img1.copyTo(Mat(vis, Rect(0, 0, w1, h1)));
    img2.copyTo(Mat(vis, Rect(w1, 0, w2, h2)));
    cvtColor(vis, vis, COLOR_GRAY2BGR);

    // 매칭된 특징점을 그려줄 최대 직선 개수 
    int maxlines = 500; 
    // 두 특징점에 동그라미를 그리고 두 점을 연결하는 선을 그림
    for (int idx = 0; idx < min(inliers, maxlines); idx++)
    {
        const Point2f& pi1 = pointPairs[idx].first;
        const Point2f& pi2 = pointPairs[idx].second;
        circle(vis, pi1, 2, Scalar(0, 255, 0), -1);
        circle(vis, pi2 + Point2f((float)w1, 0), 2, Scalar(0, 255, 0), -1);
        line(vis, pi1, pi2 + Point2f((float)w1, 0), Scalar(0, 255, 0));
    }

    imshow("affine find_obj", vis);
    waitKey(0);
    return 0;
}
// 출처 : https://webnautes.tistory.com/1431 (코드 : https://bit.ly/w0810)