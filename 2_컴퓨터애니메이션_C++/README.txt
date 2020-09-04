실행영상 링크 : https://youtu.be/MZ0Tq9c0UUk
실행파일 링크 : https://drive.google.com/file/d/1_pn_7n9GuFkVeNHmRpHIjSQ2gRpspDdK/view?usp=sharing

[Incremental_rotation]
Quaternion 또는 Rotation Matrix를 사용해 물체를 회전하는 실습입니다.
Quaternion은 쉽게 normalize가 가능해 많은 회전을 할 때 오차에 의해 크기가 변하는 것을 막을 수 있지만
Rotation Matrix만을 이용하면 normalize에 많은 연산이 필요하고, normalize를 하지 않으면 오차에 의해 크기가 변하게 됩니다.

[SLERP]
두 위치 또는 쿼터니언을 주고 Linear 또는 Spherical linear interpolation 을 구현한 실습입니다.
[1]  양 끝 위치를 Linear interpolation 합니다.
[2]  주어진 두 회전 행렬을 Linear interpolation 합니다.
[3]  주어진 유닛 쿼터니언을 Linear interpolation 합니다.
[4],[5],[6]  주어진 두 쿼터니언을 SLerp 합니다. Eigen에서 주어진 함수를 사용하거나 구현해 SLerp를 적용했습니다.

[HW_1 : natural_cubic_spline]
주어진 Data points로 natural cubic spline을 계산해 곡선을 그리는 과제입니다.
[a]  Data point를 추가합니다.
[d]  Data point를 선택해 드래그 합니다.
[r]  Data point를 선택해 지웁니다.
[i]  가까운 곡선의 한 지점에 Data point를 삽입합니다.

[HW_2 : b_spline]
주어진 Control points로 b_spline을 계산해 곡선을 그리는 과제입니다.
[a]  Control point를 추가합니다.
[d]  Control point를 선택해 드래그 합니다.
[r]  Control point를 선택해 지웁니다.
[i]  가까운 Control Polygon의 한 지점에 Control point를 삽입합니다.

[forward_kinematics]
각 Joint의 각에 따라 가장 끝 위치, end-effector가 어떻게 놓이는 지 확인하는 Forward Kinematics 실습입니다.
[1:3] 각 관절을 선택합니다.
[left], [right] 선택한 Joint의 각도를 조절합니다.

[inverse_kinematics]
가장 끝 위치, end-effector가 특정 위치에 놓일 수 있는 Joint의 각을 구하는 Inverse Kinematics를 각 방법에 따라 구하는 실습입니다.
[1:3] 각 관절을 선택합니다.
[left], [right] 선택한 Joint의 각도를 조절합니다.
[j] Jacobian transpose method를 사용해 IK를 진행합니다.
[p] pseudoinverse method를 사용해 IK를 진행합니다.
[d] damped least squares method를 사용해 IK를 진행합니다.
[Mouse Left] 마우스의 위치를 end-effector의 목표 위치로 설정합니다.
