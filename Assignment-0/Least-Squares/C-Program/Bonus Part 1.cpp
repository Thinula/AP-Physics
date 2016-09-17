#include <iostream>
#include <iomanip> // for outputting doubles

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

	double temp,x[n],y[n],vande[n][4],trans[4][n],nums[4][4],inv[4][4],prod[4][n],coeff[4];

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

	// nums is the matrix V^T*V (nums is a 4x4 Matrix)
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < n; k++)
                nums[i][j] += trans[i][k]*vande[k][j];

    // create the Identity Matrix to start computing the Inverse
	for(int i = 0; i < 4; i++)
	{
	    for (int j = 0; j < 4; j++)
	    {
	        if (i == j)
	            inv [i][j] = 1;
	        else
	            inv [i][j] = 0;
	   }
	}

	// First, make sure there is indeed an inverse
	for (int i = 0; i < 4; i++)
	{
		if (nums[i][i] == 0) // if you find there is no inverse, leave the program
		{
			cout << "There is no inverse!" << endl;
			return 0;
		}
	}

	// Compute the Inverse Matrix
    for(int i = 0; i < 4; i++)
    {
        temp = nums[i][i];

        for (int j = i; j < 4; j++)
        	nums[i][j] /= temp;
        for (int j = 0; j < 4; j++)
        	inv [i][j] /= temp;

        for(int j = 0; j < 4; j++)
        {
            if (j != i)
            {
                temp = nums[j][i];
                for (int k = 0; k < 4; k++)
                {
                	nums[j][k] -= temp*nums[i][k];
                	inv [j][k] -= temp*inv [i][k];
            	}
        	}
    	}
	}

	// nums is the matrix V^T*V (nums is a 4x4 Matrix)
	// inv is the inverse of nums (also a 4x4 Matrix)
	for (int i = 0; i < 4; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < 4; k++)
                prod[i][j] += inv[i][k]*trans[k][j]; // prod is a 4xn Matrix

    // Final step: compute coeff Matrix: (coeff=prod*y)
    for (int i = 0; i < 4; i++)
        for (int k = 0; k < n; k++)
            coeff[i] += prod[i][k]*y[k];

	// Print results
	cout << "The desired polynomial is:" << endl;
	cout << "y = ";

	if (coeff[2] > 0)
		cout << setprecision(1) << fixed << coeff[3] << "+";
	else
		cout << setprecision(1) << fixed << coeff[3];

	if (coeff[1] > 0)
		cout << setprecision(1) << fixed << coeff[2] << "x+";
	else
		cout << setprecision(1) << fixed << coeff[2] << "x";

	if (coeff[0] > 0)
		cout << setprecision(1) << fixed << coeff[1] << "x^2+";
	else
		cout << setprecision(1) << fixed << coeff[1] << "x^2";

	cout << setprecision(1) << fixed << coeff[0] << "x^3.";
}
