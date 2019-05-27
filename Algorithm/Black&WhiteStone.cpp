#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	vector<int> output;
	string s;
	int i = 0;
	cin >> s;
	char* bw;  //문자 구분
	int* arr;  //문자에 숫자 부여
	while (s != "EOI") {
		int n = stoi(s);
		int cnt = 1;
		//초기화
		output.clear();
		bw = new char[2 * n + 1];
		for (i = 0; i < n; i++) {
			bw[i] = 'w';
		}
		bw[i] = 's';

		for (i = i + 1; i < 2 * n + 1; i++) {
			bw[i] = 'b';
		}
		//수행
		int index = -1;
		//공백 위치 찾음
		for (i = 0; i < 2 * n + 1; i++) {
			if (bw[i] == 's') {
				index = i;
				break;
			}
		}

		int finish = 0;
		int changeIndex;
		bool pass[2] = { false, false };
		while (finish == 0) {
			if ((index == 0) || (index == 2 * n)) {
				pass[0] = true;
				pass[1] = true;
			}
			if ((index == 1) || (index == 2 * n - 1)) {
				pass[0] = true;
			}

			changeIndex = index;
			if (((index == 1 && bw[index - 1] == 'b') || (index == 0)) && bw[index + 1] == 'b' && bw[index + 2] == 'w') {
				changeIndex = index + 1;
			}
			else if ((index == 1 && bw[index - 1] == 'b') && bw[index + 1] == 'w' && bw[index + 2] == 'b') {
				changeIndex = index + 2;
			}
			else {
				if (((index < 2) && (pass[0])) || (bw[index - 2] == 'w'))
					if (((index < 2) && (pass[1])) || (bw[index - 1] == 'w'))
						if (((index > 2*n -2) && (pass[1])) || (bw[index + 1] == 'w'))
							changeIndex = index + 2;
						else
							changeIndex = index - 1;
					else
						changeIndex = index - 2;
				else
					if ((pass[1]) || (bw[index - 1] == 'w'))
						if ((pass[1]) || (bw[index + 1] == 'w'))
							if ((pass[0]) || (bw[index + 2] == 'w'))
								changeIndex = index - 1;
							else
								changeIndex = index + 2;
						else
							changeIndex = index + 1;
					else
						if (bw[index + 1] == 'w') {
							if (bw[index + 2] == 'b')
								changeIndex = index + 2;
							else
								finish = 1; //종료
						}
						else {
							changeIndex = index + 1;
						}
			}
			if (changeIndex == -1)
				finish = 1;
			pass[0] = false;
			pass[1] = false;
			output.push_back(changeIndex + 1);
			char temp1;
			char temp2;

			temp1 = bw[index];
			bw[index] = bw[changeIndex];
			bw[changeIndex] = temp1;

			index = changeIndex;
			cnt++;
		}
		cout << cnt - 2 << "\n";
		int count = 0;
		for (int i = 0; i < output.size() - 1; i++) {
			count++;
			cout << output[i] << " ";
			if (count == 20) {
				count = 0;
				cout << "\n";
			}
		}
		cout << "\n";
		cin >> s;
	}



	return 0;
}