#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using namespace std;

class Matrix
{
    int rows;
    int cols;
    vector< vector<double> > matrix;
    public:
        Matrix(int, int);
        Matrix (vector < vector<double> >);
        void transpose();
        Matrix add(vector < vector<double> >);
        Matrix subtract(vector <vector<double> >);
        Matrix multiply(vector < vector<double> >);
        void invert();
        //virtual ~Matrix(); don't know what this was for...
    protected:
    private:
        vector< vector<double> > scale(int, vector< vector<double> >); // I don't think this will be needed publicly?
        vector< vector<double> > coFactor(int, int, vector< vector<double> >);

};

Matrix::Matrix(int r, int c) // constructor
{
	rows = r;
	cols = c;

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			matrix[i][j] = 0;
	}
}

Matrix::Matrix(vector < vector<double> > copy) // copy the data
{
	rows = copy.size();
	cols = copy[0].size();
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = copy[i][j];
}

void Matrix::transpose() // transpose the Matrix
{
	// element ij becomes element ji
	vector < vector<double> > temp;
	// re-assigns the cols and rows variables accordingly
	cols = matrix.size();
	rows = matrix[0].size();

	for (unsigned int i = 0; i < matrix.size(); i++) // not sure why this has to be unsigned int...
		for (unsigned int j = 0; j < matrix[i].size(); j++)
			temp[i][j] = matrix[j][i];

	matrix = temp; // sets matrix as it's transposed version
}

vector< vector<double> > Matrix:: scale(int i, vector< vector<double> > vect)
{
    for (unsigned int i = 0; i < vect.size(); i++)
        for (unsigned int j = 0; j < vect.at(0).size(); j++)
            vect[i][j] *= -1;
    return vect;
}

Matrix Matrix::add(vector < vector<double> > addVect) // add two Matrices
{
    Matrix sum = Matrix(rows,cols);

    // the size method returns an unsigned int...so convert it to an int before comparing in the if statement
    int row = (int) addVect.size();
    int col = (int) addVect.at(0).size();

    if (rows == row && cols == col) // make sure matrices are the same size (addition is only defined this way)
    {
        for (int i = 0; i < row; i++) // add all the elements
            for (int j = 0; j < col; j++)
                sum.matrix[i][j] += addVect[i][j];
    }
	return sum; // how would i return something null in C++?
}

Matrix Matrix::subtract(vector < vector<double> > subVect) // subtract two Matrices
{
    // NOTE: subtracting a matrix is same as adding its "negative" matrix
    vector< vector<double> > diff = scale(-1,subVect);

    return add(diff); // return the sum
}

Matrix Matrix::multiply(vector < vector<double> > multiVect) // multiply two Matrices
{
    // the size method returns an unsigned int...so convert it to an int before comparing in the if statement
    int row = (int) multiVect.size();
    int col = (int) multiVect.at(0).size();

    Matrix prod = Matrix(rows,col);

    if (cols == row) // make sure the dimensions for multiplication work
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < col; j++)
                for (int k = 0; k < cols; k++)
                    prod.matrix[i][j] += matrix[i][k]*multiVect[k][j];
    }
    return prod;
}

vector < vector<double> > Matrix::coFactor(int r, int c, vector< vector<double> > det)
{
    vector< vector<double> > ret;

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
            ret[i][j] = det[i][j];
        for (unsigned int j = c+1; j < det.at(i).size(); j++)
            ret[i][j-1] = det[i][j];
    }

    for (unsigned int i = r+1; i < det.size(); i++)
    {
        for (int j = 0; j < c; j++)
            ret[i-1][j] = det[i][j];
        for (unsigned int j = c+1; j < det.at(i).size(); j++)
            ret[i-1][j-1] = det[i][j];
    }

    return ret;
}

void Matrix::invert()
{

}

#endif // MATRIX_H
