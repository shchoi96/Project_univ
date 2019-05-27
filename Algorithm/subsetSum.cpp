// 개발환경 : Visual Studio 2017
// 학번 : 2015726045
// 이름 : 최승혁
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool** dp; // dynamic programming으로 풀기위한 배열
unsigned long** dp2;
bool overflow;
bool cant;
vector<int> v;
vector<int> b;

//dp의 값을 통해 경로를 추적
void traceSubsets(int *arr, int i, int sum) {
	if ((i == 0 && sum != 0))
	{
		v.push_back(arr[i]);
		return;
	}
	if (i == 0 && sum == 0)
	{
		b.push_back(arr[i]);
		return;
	}
	if (sum >= arr[i] && dp[i - 1][sum - arr[i]])
	{
		v.push_back(arr[i]);
		traceSubsets(arr, i - 1, sum - arr[i]);
	}
	else if (dp[i - 1][sum])
	{
		b.push_back(arr[i]);
		traceSubsets(arr, i - 1, sum);
	}

}
//dp, dp2를 채우는 함수
void findSubsets(int *arr, int n, int sum) {
	dp = new bool*[n];
	dp2 = new unsigned long*[n];
	for (int i = 0; i < n; ++i)
	{
		dp[i] = new bool[sum + 1];
		dp[i][0] = true;
		dp2[i] = new unsigned long[sum + 1];
		dp2[i][0] = 1;
	}

	for (int i = 0; i <= sum; i++) {
		dp[0][i] = false;
		dp2[0][i] = 0;
	}


	if (arr[0] <= sum) {
		dp[0][arr[0]] = true;
		dp2[0][arr[0]] = 1;
	}

	for (int i = 1; i < n; ++i)
		for (int j = 0; j < sum + 1; ++j) {
			dp[i][j] = (arr[i] <= j) ? (dp[i - 1][j] || dp[i - 1][j - arr[i]]) : dp[i - 1][j];
			if (arr[i] <= j) {
				unsigned long long num;
				num = (unsigned long long)dp2[i - 1][j] + (unsigned long long)dp2[i - 1][j - arr[i]];
				if (num > 4294967295ULL) overflow = true;
				if (overflow) continue;
				dp2[i][j] = dp2[i - 1][j] + dp2[i - 1][j - arr[i]];
			}
			else
				dp2[i][j] = dp2[i - 1][j];
		}

	if ((dp[n - 1][sum] == false) || (dp[n - 1][sum] == 0))
	{
		return;
	}

	traceSubsets(arr, n - 1, sum);

}

int main() {
	int* arr;
	int sum, arr_size;
	string s;
	cin >> s;
	while (s != "EOI") {
		//초기화 및 입력
		arr_size = stoi(s);
		v.clear();
		b.clear();
		overflow = false;
		cant = false;
		sum = 0;
		arr = new int[arr_size];
		for (int i = 0; i < arr_size; i++) {
			int num;
			cin >> num;
			arr[i] = num;
			sum += num;
		}
		if (arr_size == 0 || sum < 0) {
			cout << "0\n";
			cin >> s;
			continue;
		}
		if (sum % 2 == 1) {
			cout << "0\n";
			cin >> s;
			continue;
		}
		sort(arr, arr + arr_size);
		//DP를 이용해 갯수와 경로를 구하는 작업
		findSubsets(arr, arr_size, sum / 2);
		//출력
		if (cant)
			continue;
		if (overflow)
			cout << "NUMEROUS\n";
		else
			cout << dp2[arr_size - 1][sum / 2] << "\n";
		if (dp2[arr_size - 1][sum / 2] != 0) {
			cout << "{";
			for (unsigned int i = v.size() - 1; i > 0; --i)
				cout << v[i] << ",";
			cout << v[0] << "},{";
			for (unsigned int i = b.size() - 1; i > 0; --i)
				cout << b[i] << ",";
			cout << b[0] << "}\n";
		}

		//재입력
		cin >> s;
	}

}