실행영상 링크 : https://youtu.be/OVgt18iU8_8
실행파일 링크 : https://drive.google.com/file/d/1qBTf5oH4nO6GHlAUVnf56xy_5TZkGTUv/view?usp=sharing

[2d_keyboard]
OpenGL에서 점을 줄 때 설정한 기능에 따라 어떻게 Points, Lines, Polygons를 그리는 지 확인하는 실습입니다.
[1] POINTS : 주어진 점을 찍는다.
[2] LINES : 주어진 점을 끊어진 선으로 그린다. 1-2, 3-4, 5-6
[3] LINE_STRIP : 주어진 점을 연속된 선으로 그린다. [2]에서 점을 중복되게 주는 것과 동일한 효과.  1-2, 2-3, 3-4, 4-5, 5-6
[4] LINE_LOOP : 주어진 점을 연속된 선으로 그리고, 맨 처음 주어진 점과 맨 마지막 주어진 점을 연결한다. 1-2, 2-3, 3-4, 4-5, 5-6, 6-1
[5] TRIANGLES : 주어진 점을 점의 중복없이 삼각형을 그린다. 1-2-3, 4-5-6
[6] TRIANGLE_STRIP : 주어진 점을 중간 점들을 중복해 삼각형을 그린다. 1-2-3, 2-3-4, 3-4-5, 4-5-6
[7] TRIANGLE_FAN : 주어진 점을 삼각형의 처음과 끝점만 중복해 삼각형을 그린다. 1-2-3, 3-4-5, 5-6-1
[S] 선을 점선으로 표현한다.
[F] 면을 색으로 채운다.

[transformation]
OpenGL에서 위치 이동, 회전, 크기 변환 등에 관한 실습입니다.
[1] z 축(파랑)을 따라 물체를 회전시킵니다.
[2] 임의의 한 축을 중심으로 물체를 회전시킵니다.
[3] x 축(빨강)을 따라 물체의 크기를 변환시킵니다.
[4] 임의의 한 축을 중심으로 물체의 크기를 변환시킵니다.
[5] 임의의 방향으로 물체의 크기를 변환시킵니다.
[6] 임의의 한 축을 중심으로 물체를 회전시키는 방법을 이용해 행성 운동을 표현한 예제입니다.

[shading]
얼마나 쪼개서 구를 표현할 지 결정하고 어떻게 보이는 지 확인하는 실습입니다. 또한 Bunny 형태를 주고 flat shading과 smooth shading의 개념을 구현해보며 결과를 확인하는 실습입니다.
[1], [2], [3], [4] : 구
[5] : bunny에 flat shading 적용
[6] : bunny에 smooth shading 적용

[lights_materials]
다양한 조명 효과(point, distant, spot light)와 조명의 속성들을 변화시키며 물체에 어떻게 조명이 적용되는지 확인하는 실습입니다.
[v] 회전을 시작하거나 멈춥니다.
[f] front/top의 시점 변화
[p] point light
[d] distant light
[s] spot light
[a] light attenuation
[e] exponent of a spot light
[c] cutoff of a spot light
[l] 조명 회전
[o] 물체 회전
[1] 3x3 구
[2] 3x3 flat bunnies
[3] 3x3 smooth bunnies
[4] 평면

[HW_1]
Torus를 그리는 과제로, 아래와 같은 기능들이 존재합니다.
[1] Torus를 점으로 표현합니다.
[2] Torus를 이루는 점을 연결해 사각형 면으로 표현합니다.
[3] Torus를 양면이 같은 색으로 된 외곽선이 없는 사각형 면으로 표현합니다.
[4] Torus를 양면이 같은 색으로 된 외곽선이 있는 사각형 면으로 표현합니다.
[5] Torus를 이루는 면들의 법선 벡터를 표현합니다.(토글)
[6] Torus를 양면이 다른 색으로 된 외곽선이 있는 사각형 면으로 표현합니다.
[a], [s] y축(초록)을 중심으로 Torus의 일부를 감추거나 보이도록 합니다.
[j], [k] Torus를 이루는 원의 중심을 기준으로 Torus의 일부를 감추거나 보이도록 합니다.

[HW_2]
Torus를 그리고 아래와 같은 조명 효과를 적용해보는 과제입니다.
[space] Torus의 주위로 조명을 회전시키거나 멈춥니다.
[p] point light
[d] distant light
[s] spot light
[t] 시간에 따라 물체의 shininess를 변화

[HW_3]
Torus에 Texture를 적용해보는 과제입니다.
[1], [2], [3] 번호에 따라 다른 Texture가 적용합니다.
