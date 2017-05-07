#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include "boost\numeric\odeint.hpp"

using namespace std;
using namespace boost::numeric::odeint;

//dx/dt=-2x
void my_system(const vector<double> &x, vector<double> &dxdt, const double t)
{
	dxdt[0] = -2 * x[0];
}
//
void my_system2(const vector<double> &x, vector<double> &dxdt, const double t)
{
	dxdt[0] = -2;
}

//logisztikus egyenlet: dx/dt=r*x(1 - x)
void logistic_system(const vector<double> &x, vector<double> &dxdt, const double t)
{
	dxdt[0] = 0.1 * x[0] * (1 - x[0]);
}

void epsilon(const vector<double> &x, vector<double> &dxdt, const double t)
{
	//dxdt[0] = 0.05 * exp( 0.5 * x[0] * (1 - x[0]) * t);
	for (unsigned int i = 0; i < dxdt.size(); ++i)
	{
		dxdt[i] = 0.005 * exp(10 * x[i] * (1 - x[i]) * t);
	}
}

void connected_epsilon(const vector<double> &x, vector<double> &dxdt, const double t)
{
	double alpha = 0.3;
	double beta = 0.4;
	double k1 = 1;
	double k2 = 1;
	double o1 = x[0];
	double o2 = x[1];
	dxdt[0] = 2 * o1 * (1 - (o1 + alpha * o2) / k1);
	dxdt[1] = 2 * o2 * (1 - (o2 + beta * o1) / k2);
}

void my_observer(const vector<double> &x, const double t)
{
	cout << t << " ";
	for (auto o : x)
	{
		cout << o << " ";
	}
	cout << endl;
}

string tar = "";
void toFile(const vector<double> &x, const double t)
{
	tar += to_string(t) + " " + to_string(x[0]) + " " + to_string(x[1]) + "\n";
}


int main()
{
	vector<double> x(10);
	double o = 0.0;
	std::generate(x.begin(), x.end(), [&o]{
		o += 0.1;
		return o;
	});
	//iota(x.begin(), x.end(), 1);
	//x.assign(10000, 11.2);
	ofstream file("kettesf.data");
	file.clear();
	
	double t0 = 0;
	double t1 = 100;
	double dt = 0.01;

	
	integrate_const(runge_kutta4<vector<double>>(), connected_epsilon, x, t0, t1, dt, toFile);
	file << tar;
	file.close();
}