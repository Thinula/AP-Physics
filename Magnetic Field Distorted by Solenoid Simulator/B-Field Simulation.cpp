#include <bits/stdc++.h>

using namespace std;

double mu = 1.26E-6; // permeability constant
double pi = atan(1)*4; // value for pi
double xMin = -0.4; // minimum x value that B field is being computed for
double xMax = 0.4; // maximum x value that B field is being computed for
double yMin = -0.4; // minimum y value that B field is being computed for
double yMax = 0.4; // maximum y value that B field is being computed for
double zMin = -0.4; // minimum z value that B field is being computed for
double zMax = 0.4; // maximum value that B field is being computed for 
double scale = 0.01; // scale for the graph
double i = 0.6; // current

class Vector // Vector class (mainly with dimensions <= 3 so cross-product is defined)
{
	public:
		double x,y,z; // three parameters for each direction
		
		Vector() // default constructor
		{
			x = 0, y = 0, z = 0;	
		}	
		
		Vector(double a, double b, double c) 
		{
			x = a, y = b, z = c; 
		}
		
		Vector operator+(Vector v) // operator to add two vectors
		{
			return Vector(x+v.x,y+v.y,z+v.z);
		}
		
		Vector operator*(double k) // operator to multiply vector by a scalar value
		{
			return Vector(x*k,y*k,z*k);
		}
		
		Vector operator-(Vector v) // operator to subtract two vectors
		{
			return Vector(x-v.x,y-v.y,z-v.z);
		}
		
		Vector cross(Vector v) // cross-product of two vectors
		{
			return Vector(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
		}
		
		double mag() // returns magnitude of vector
		{
			return sqrt(x*x+y*y+z*z);
		}
		
		Vector rotate(double theta) // rotate the vector by an angle theta about z-axis
		{
			double angle = theta*pi/180; // angle of rotation in radians
			
			double newX = x*cos(angle)-y*sin(angle); // x-coordinate after rotation
			double newY = x*sin(angle)+y*cos(angle); // y-coordinate after rotation
			double newZ = z; // z-coordinate after rotation
			
			return Vector(newX,newY,newZ); // returns the rotated vector
		}
};

class Solenoid // Solenoid class
{
	public:
		Vector pos; 
		double theta,R,L,N,k;
		
		Solenoid(double x, double y, double z, double theta, double R, double N, double L)
		{
			// Note theta is in degrees 
			pos = Vector(x, y, z).rotate(theta); // gives position vector of solenoid
			this->theta = theta;
			this->R = R; // radius of each coil
			this->N = N; // number of coils
			this->L = L; // length of solenoid
			k = 2*pi*N; // stretching factor for helix shape
		}
		
		// parameters are the coordinates of the point where B-Field is being computed
		Vector bField (double x, double y, double z)
		{
			Vector r,ds,dB;
			Vector B = Vector(0,0,0); // before integrating along the solenoid, the B-Field is 0
			
			for (double t = 0; t <= 1; t += 0.01)
			{
				// r is given by the vector parameterization of a helix in 3D
				r = Vector(x,y,z)-pos-Vector(R*cos(k*t),R*sin(k*t),L*t).rotate(theta);
				// ds is given by dr/dt
				ds = Vector(-R*k*sin(2*pi*N*t),R*k*cos(k*t),L).rotate(theta);
				// apply Biot-Savart's Law to get dB, the differential element of the B-Field created
				dB = ds.cross(r)*((i*mu)/(4*pi*r.mag()*r.mag()*r.mag()));
				// add dB to the already existing B
				B = B + dB;
			}
			
			return B; // return the B-Field at the point
		}
		
};

int main()
{
	// make a 3d vector of Vector for the B-field
	ofstream fout("B-Field Output.csv"); // output file
	
	Solenoid s1 = Solenoid(0, 0, 0, 72, 0.5, 12, 2);
	Solenoid s2 = Solenoid(0.2, -0.3, 0.15, 120, 0.5, 12, 2);
	
	int a = floor((xMax-xMin)/scale);
	int b = floor((yMax-yMin)/scale);
	int c = floor((zMax-zMin)/scale);
	
	cout << a << "\n" << b << "\n" << c << endl;
	vector<vector<vector<Vector> > > bField(a+1,vector<vector<Vector> >(b+1,vector<Vector>(c+1)));
	
	for (int i = 0; i <= a; i ++)
		for (int j = 0; j <= b; j++)
			for (int k = 0; k <= c; k ++)
				bField[i][j][k] = s1.bField(i*scale+xMin,j*scale+yMin,k*scale+zMin)+s2.bField(i*scale+xMin,j*scale+yMin,k*scale+zMin);
	
	fout << "x, y, z, bField.x, bField.y, bField.z" << endl;				
	for (int i = 0; i <= a; i ++)
		for (int j = 0; j <= b; j++)
			for (int k = 0; k <= c; k ++)
				fout << i*scale+xMin << "," << j*scale+yMin << "," << k*scale+zMin << "," << bField[i][j][k].x << "," << bField[i][j][k].y << "," << bField[i][j][k].z << "\n";
	cout << "Sim is finished!";
}
