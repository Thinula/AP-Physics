#include <iostream>

using namespace std;

double pow(double base, long long exp)
{
	double res = 1, value = base;
    long long power = exp;

    while(power > 0)
    {
        if(power % 2 == 1)
            res *= value;
        value *= value;
        power /= 2;
    }
    return res;
}

int main()
{
	int n;
	cout << "Enter the number of entries: ";
	cin >> n;

	double x[n],y[n];
	double vande[n][4];
	double trans[4][n];
	double nums[n][n];

	cout << "Enter the entries below, with a space between x and y entries:" << endl;

	// input the data
	for (int i = 0; i < n; i++)
		cin >> x[i] >> y[i];

	// create the Vandermonde matrix for cubic
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 4; j++)
			vande[i][j] = pow(x[i],j);

	// create the transpose of the Vandermonde matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < n; j++)
			trans[i][j] = vande[j][i];

	// nums is the matrix V*V^T
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < 4; k++)
                nums[i][j] += vande[i][k]*trans[k][j];

    // print out the data

    // Vandermonde Matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << vande[i][j] << " ";
		cout << endl;
	}

	// Transpose Matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < n; j++)
			cout << trans[i][j] << " ";
		cout << endl;
	}

	// Product Matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << nums[i][j] << " ";
		cout << endl;
	}

}
