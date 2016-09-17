#include <iostream>
#include <cmath>
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

	int m;
	cout << "Enter the degree of the fitting polynomial: ";
	cin >> m;

	while (m <= 0 || 2*m > n)
	{
		cout << "The degree must be a positive integer less than half the number of entries." << endl;
		cout << "Enter a valid degree: ";
		cin >> m;
	}

	// all the matrices that will be created
	double temp,x[n],y[n],vande[n][m+1],trans[m+1][n],nums[m+1][m+1],inv[m+1][m+1],prod[m+1][n],coeff[m+1];

	cout << "Enter the entries below, with a space between x and y entries:" << endl;

	// input the data
	for (int i = 0; i < n; i++)
		cin >> x[i] >> y[i];

	// create the Vandermonde matrix for m-degree polynomial
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m+1; j++)
			vande[i][j] = pow(x[i],j);
	
	// create the transpose of the Vandermonde matrix
	for (int i = 0; i < m+1; i++)
		for (int j = 0; j < n; j++)
			trans[i][j] = vande[j][i];

	// nums is the matrix V^T*V (nums is a (m+1)x(m+1) Matrix)
    for (int i = 0; i < m+1; i++)
        for (int j = 0; j < m+1; j++)
            for (int k = 0; k < n; k++)
                nums[i][j] += trans[i][k]*vande[k][j];

    // create the Identity Matrix to start computing the Inverse
	for(int i = 0; i < m+1; i++)
	{
	    for (int j = 0; j < m+1; j++)
	    {
	        if (i == j)
	            inv [i][j] = 1;
	        else
	            inv [i][j] = 0;
	   }
	}

	// First, make sure there is indeed an inverse
	for (int i = 0; i < m+1; i++)
	{
		if (nums[i][i] == 0) // if you find there is no inverse, leave the program
		{
			cout << "There is no inverse!" << endl;
			return 0;
		}
	}

	// Compute the Inverse Matrix
    for(int i = 0; i < m+1; i++)
    {
        temp = nums[i][i];

        for (int j = i; j < m+1; j++)
        	nums[i][j] /= temp;
        for (int j = 0; j < m+1; j++)
        	inv [i][j] /= temp;

        for(int j = 0; j < m+1; j++)
        {
            if (j != i)
            {
                temp = nums[j][i];
                for (int k = 0; k < m+1; k++)
                {
                	nums[j][k] -= temp*nums[i][k];
                	inv [j][k] -= temp*inv [i][k];
            	}
        	}
    	}
	}

	// inv is the inverse of nums (so it is also a (m+1)x(m+1) Matrix)
	for (int i = 0; i < m+1; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < m+1; k++)
                prod[i][j] += inv[i][k]*trans[k][j]; // prod is a (m+1)xn Matrix

    // Final step: compute coeff Matrix: (coeff=prod*y)
    for (int i = 0; i < m+1; i++)
        for (int k = 0; k < n; k++)
            coeff[i] += prod[i][k]*y[k];

	// Print results
	cout << "The desired polynomial is:" << endl;
	cout << "y = ";

	if (coeff[1] > 0)
			cout << setprecision(1) << fixed << coeff[0] << "+";
	else
		cout << setprecision(1) << fixed << coeff[0];

	if (m == 1)
		cout << setprecision(1) << fixed << coeff[1] << "x.";
	else if(coeff[2] > 0)
		cout << setprecision(1) << fixed << coeff[1] << "x+";
	else
		cout << setprecision(1) << fixed << coeff[1] << "x";

	for (int i = 2; i < m; i++)
	{
		if (coeff[i+1] > 0)
			cout << setprecision(1) << fixed << coeff[i] << "x^" << i << "+";
		else if (coeff[i+1] != 0)
			cout << setprecision(1) << fixed << coeff[i] << "x^" << i;
	}

	if (m > 1)
		cout << setprecision(1) << fixed << coeff[m] << "x^" << m << ".";
}
