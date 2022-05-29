#include<iostream>
#include<vector>
using namespace std;
int main()
{
	int n;
	cin >> n;
	vector<int> a(n);
	int sum = 0;
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
		sum += a[i];
	}
	vector<vector<int> > form(n + 1, vector<int>(sum / 2 + 1, 0));
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= sum / 2; ++j)
		{
			if (j >= a[i - 1])
				form[i][j] = max(form[i - 1][j], form[i - 1][j - a[i - 1]] + a[i - 1]);
			else
				form[i][j] = form[i - 1][j];
		}
	}
	bool flag = (sum - 2 * form[n][sum / 2] == 0);
	if (flag)
		cout << "true" << endl;
	else
		cout << "false" << endl;
	return 0;
}
