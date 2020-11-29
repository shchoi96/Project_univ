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
    // ���� �̹���
    Mat img1 = imread("4.jpg", IMREAD_GRAYSCALE);
    Mat img2 = imread("3.jpg", IMREAD_GRAYSCALE);
    
    // SIFT �˰��� �ʿ��� ����
    Ptr<SIFT> ext = SIFT::create();
    vector<KeyPoint> kp1, kp2;
    Mat desc1, desc2;

    // SIFT �˰����� ���� Ư¡���� Descriptor ���
    // Scale-space Extrema Detection
    // Keypoint Localization
    // Orientation Assignment
    // Keypoint Descriptor 
    // �� �� �ܰ� ������ ����
    ext->detectAndCompute(img1, Mat(), kp1, desc1);
    ext->detectAndCompute(img2, Mat(), kp2, desc2);

    // Matching�� �ʿ��� ����
    vector< vector<DMatch> > rawMatches;
    vector<Point2f> p1, p2;
    vector<float> distances;
    
    // �� �̹������� ������ Ư¡���� ���� ��Ī�ϴ� ����
    Ptr<DescriptorMatcher> matcher;
    
    // ��Ī ��� �� Flann(Fast Library approximate nearest neighbors)��
    // �ٰ����� �� �˰����� �̿�
    matcher = DescriptorMatcher::create("FlannBased");
    
    // �� Descriptor �� ���� ��ġ�ϴ� k=2���� rawMatches�� ��ȯ
    // {1����, 2����}
    matcher->knnMatch(desc1, desc2, rawMatches, 2);

    // 2���� ��Ī ����� 0.75�躸�� �� ����� 1���� ���� ���
    for (size_t i = 0; i < rawMatches.size(); i++)
    {
        const vector<DMatch>& m = rawMatches[i];
        if (m.size() == 2 && m[0].distance < m[1].distance * 0.75)
        {
            // queryIdx, trainIdx : ��Ī�� desc1�� desc2�� index
            p1.push_back(kp1[m[0].queryIdx].pt);
            p2.push_back(kp2[m[0].trainIdx].pt);
            distances.push_back(m[0].distance);
        }
    }

    // �� ����� �ٸ� ��鿡 ���������� ��
    // ������ �������� ������ ������ ��ȯ���� ���ϱ�
    // RANSAC : �������� ���� �����͸� ���� ���� �ִ�� ���ǵ� �� ����
    vector<uchar> status;
    vector< pair<Point2f, Point2f> > pointPairs;
    Mat H = findHomography(p1, p2, status, RANSAC);
    // RANSAC�� ������ �𵨿��� ũ�� ����� �ʴ� ������ ����
    int inliers = 0;
    for (size_t i = 0; i < status.size(); i++)
    {
        if (status[i])
        {
            pointPairs.push_back(make_pair(p1[i], p2[i]));
            inliers++;
        }
    }

    // ��� �̹��� vis �� �� �̹��� ����
    int h1 = img1.size().height;
    int w1 = img1.size().width;
    int h2 = img2.size().height;
    int w2 = img2.size().width;
    Mat vis = Mat::zeros(max(h1, h2), w1 + w2, CV_8U);
    img1.copyTo(Mat(vis, Rect(0, 0, w1, h1)));
    img2.copyTo(Mat(vis, Rect(w1, 0, w2, h2)));
    cvtColor(vis, vis, COLOR_GRAY2BGR);

    // ��Ī�� Ư¡���� �׷��� �ִ� ���� ���� 
    int maxlines = 500; 
    // �� Ư¡���� ���׶�̸� �׸��� �� ���� �����ϴ� ���� �׸�
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
// ��ó : https://webnautes.tistory.com/1431 (�ڵ� : https://bit.ly/w0810)