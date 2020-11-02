디지털영상처리 과목을 수강하며 배운 내용과 과제입니다.
주요 라이브러리 : OpenCV
언어 : C++

[HW1]
1. Resize : Bilinear Interpolation 을 적용해서 입력 이미지의 크기를 입력한 크기에 맞게 변환시켜 출력한다.
2. Rotate : 입력한 각도만큼 이미지를 회전해서 출력한다.

[HW2_1]
- 입력된 이미지의 RGB 정보를 YCrCb (ITU-R BT.601)로 변환해 출력한다.

[HW2_2]
- 입력된 이미지의 픽셀 값에 접근해 얼굴색 여부를 판단한다.
- Color Slicing 방법으로 해당 픽셀이 얼굴색이면 그 픽셀값을 결과 영상에 그대로 저장하는 방식
- 해당 픽셀이 얼굴색이면 255, 아니면 0을 저장하는 방식을 구현했다.

[HW_3]
- 입력된 이미지에 입력 n에 대해서 (2*n+1) 크기의 Moving Average, Laplacian, Sharpening Filter를 적용한다.
- 각 필터를 적용하는 과정에 경계부분에서 발생하는 바깥으로 나가는 영역은 Zero, Repetition, Mirroring Padding 방법을 구현해 적용했다.