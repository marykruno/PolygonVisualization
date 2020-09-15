#if !defined _SQDIST_H
#define _SQDIST_H

using namespace std;

template <class T> double SQDistFromPointToEdge(const T& p, const T& edge_start, const T& edge_end, T& proj)
{
	const double eps = 1e-8;
	T edge(edge_end.x - edge_start.x, edge_end.y - edge_start.y);
	double A = edge.x*edge.x + edge.y*edge.y;
	double B = edge.x*(edge_start.x - p.x) + edge.y*(edge_start.y - p.y);
	double C = (p.x - edge_start.x)*(p.x - edge_start.x) + (p.y - edge_start.y)*(p.y - edge_start.y);
	double t;
	if (A < eps || ((t = -B / A)<0))
	{
		proj = edge_start;
		return C;
	}
	if (t > 1)
	{
		proj = edge_end;
		return A + 2 * B + C;
	}
	proj.x = edge_start.x + t*edge.x;
	proj.y = edge_start.y + t*edge.y;
	return -B*B / A + C;
};


#endif