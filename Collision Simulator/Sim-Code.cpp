#include <string>
#include <math.h>
#include <iostream>
#include <fstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;

const double PI = atan(1) * 4; // value of pi

class vector // vector class
{
	public:
		double x,y; // x and y components
		double mag, theta; // magnitude and angle

		vector ()
		{
			x = y = mag = theta = 0;
		}

		vector (double a, double b, bool cart) // constructor
		{
			if (cart)
			{
				x = a;
				y = b;
				mag = sqrt(x*x + y*y);
				theta = acos(x / mag);
			}
			else
			{
				mag = a;
				theta = b*PI / 180;
				x = mag*cos(theta);
				y = mag*sin(theta);
			}
		}

		vector(double x1, double x2, double y1, double y2)
		{
			x = x2 - x1;
			y = y2 - y1;
			mag = sqrt(x*x + y*y);
			theta = acos(x / mag);
		}

		void setX(double x)
		{
			this->x = x;
		}

		void setY(double y)
		{
			this->y = y;
		}

		void calcMagTheta()
		{
			mag = sqrt(x*x + y*y);
			theta = acos(x / mag);
		}

		string toString()
		{
			return "" + std::to_string(x) + "," + std::to_string(y);
		}

		vector operator+(vector v)
		{
			vector res;
			res.setX(x + v.x);
			res.setY(y + v.y);
			res.calcMagTheta();

			return res;
		}

		vector operator-(vector v)
		{
			vector res;
			res.setX(x - v.x);
			res.setY(y - v.y);
			res.calcMagTheta();

			return res;
		}

		vector operator*(double k)
		{
			vector res;
			res.setX(k*x);
			res.setY(k*y);
			res.calcMagTheta();

			return res;
		}
};

class ball
{	
	public:
		vector s, v, a; // displacement, velocity and acceleration 
		double m,r;   // mass and radius
		
		// default constructor
		ball ()
		{
			s = vector();
			v = vector();
			a = vector();
			m = 0;
			r = 0;
		}	
		
		// get input 
		void input() 
		{
			double sx, sy, vx, vy, mag, theta,choice;
			bool cart;

			cout << "Enter the x coordinate of the initial position: ";
			cin >> sx; 
		
			cout << "Enter the y coordinate of the initial position: ";
			cin >> sy; 

			cout << "Enter 0 for Cartesian velocity input and 1 for Polar: ";
			cin >> choice;

			if (choice == 0)
			{
				cout << "Enter the x-component of the intial velocity: ";
				cin >> vx;

				cout << "Enter the y-component of the intial velocity: ";
				cin >> vy;

				v = vector(vx, vy, true);
			}
			else
			{
				cout << "Enter the magnitude of the intial velocity: ";
				cin >> mag;

				cout << "Enter the angle (wrt +x) of the intial velocity: ";
				cin >> theta;
				
				v = vector(mag, theta,false);
			}

			cout << "Enter the mass of the ball: ";
			cin >> m;

			cout << "Enter the radius of the ball: ";
			cin >> r;


			s = vector(0, sx, 0, sy);			
		}
		
		// calculates the distance between the CoM of two balls
		double distance(ball b)
		{
			return (s-b.s).mag;
		}
		
		// calculates the angle formed by the line joining the CoM of two balls and +x
		double angle(ball b)
		{
			return (s-b.s).theta;
		}

		// move rewrites all variable values after a force f is applied over a time interval t
		void move(vector f, double t)
		{
			// Use N^2 (F=ma) to caclulate a

			a = f*(1.0 / m);

			// We now use kinematics formulas to update the variable values

			// Displacement component calculations using s=s0+v0*t+1/2*a*t^2

			s = s + v*t + a*(t*t / 2);

			// Velocity components calculations using v=v0+at
			v = v + a*t;
		}	

		void rotation()
		{
			// do perpendicular component: calculate new angles based on old
			// new angle = pi - angle of com line + velocity angle

			// Let THE POINT be the point of intersection between CoM line and perpendicular line

			// "Assume" F for torque is applied at THE POINT 

			// r = distance from CoM to THE POINT

			//  then torque = Fr because F is perpendicular to r

			// we need to calculate accel but inst. accel = limit as dt -> 0 of dv/dt
			// since dt is kinda small, this can just be written as inst. accel = dv/dt

			// similarly, v = ds/dt so a = ds/(dt)^2

			// since F=ma then |F|= m*ds/(dt)^2 

			// then |torque| = |r x F| = m*r*ds/(dt)^2

			//
		}
};

vector force(ball b1, ball b2) // force applied on ball 1
{	
	double compress = b1.r + b2.r - b1.distance(b2);

	if (compress <= 0) // not in contact
		return vector(0, 0,false);

	else // in contact -> k(x) = 160x^2-700x+2100 so force is 160x^3-700x^2+2100x
	{
		double x = b2.s.x - b1.s.x;
		double angle = acos(x / b1.distance(b2));
		double fmag = compress*(160 * compress*compress - 700 * compress + 2100);

		return vector(fmag*cos(angle), fmag*sin(angle),false);
	}
}

int main()
{	
	ball b1,b2;	
	bool collide = false;
	char choice = 1;
	
	std::ofstream fout("Collision Output.csv"); // output file
	
	cout << "Welcome to Collision Simulator!" << endl;
	
	cout << "Enter Ball 1's information:" << endl;
	b1.input();			
	cout << endl;

	cout << "Enter Ball 2's information:" << endl;
	b2.input();			
		
	fout << "Time (s), Ball 1 sx (m), Ball 1 sy (m), Ball 2 sx (m), Ball 2 sy (m), ,";
	fout << ", Ball 1 vx (m), Ball 1 vy (m), Ball 2 vx (m), Ball 2 vy (m), ,";
	fout << ", Ball 1 ax (m), Ball 1 ay (m), Ball 2 ax (m), Ball 2 ay (m)\n";

	for (double t = 0; t < 4; t += 0.0001)
	{
		fout << t << "," << b1.s.x << "," << b1.s.y << "," << b2.s.x << "," << b2.s.y << ", ,";
		fout << ", " << b1.v.x << "," << b1.v.y << "," << b2.v.x << "," << b2.v.y << ", ,";
		fout << ", " << b1.a.x << "," << b1.a.y << "," << b2.a.x << "," << b2.a.y << "\n";

		if (b1.distance(b2) < b1.r + b2.r)
			cout << t << endl;

		b1.move(force(b1, b2), 0.0001);
		b2.move(force(b2,b1),0.0001);
	}	
		
	cout << "\n\n\n";

	system("PAUSE");
		
	cout << "Press any key to continue." << endl;

	cin.get(); 
	system("cls");
	
	cout << "Hope you enjoyed the Collisions!" << endl;
	cout << "Goodbye and have a nice day!\n\n";			
}
