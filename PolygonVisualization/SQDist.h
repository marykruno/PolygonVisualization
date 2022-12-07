#pragma once
#include <limits>
#include <type_traits>

template <typename T> class Accessor {
public:
	static const auto& getRefX(const T& value) {
		return value.x;
	}
	static const auto& getRefY(const T& value) {
		return value.y;
	}

	static auto& getRefX(T& value) {
		return value.x;
	}
	static auto& getRefY(T& value) {
		return value.y;
	}

	static auto getValX(const T& value) {
		return value.x;
	}
	static auto getValY(const T& value) {
		return value.y;
	}
};

template <class T, class TypeAccessor = Accessor<T> > double SQDistFromPointToEdge(const T& p, const T& edge_start, const T& edge_end, T& proj)
{
    const double eps = std::numeric_limits<double>::epsilon();
	auto edge_end_start_x = TypeAccessor::getRefX(edge_end - edge_start);
	auto edge_end_start_y = TypeAccessor::getRefY(edge_end - edge_start);
	double A = edge_end_start_x*edge_end_start_x + edge_end_start_y*edge_end_start_y;
	auto edge_start_p_x = TypeAccessor::getRefX(edge_start - p);
	auto edge_start_p_y = TypeAccessor::getRefY(edge_start - p);
	double B = edge_end_start_x*edge_start_p_x + edge_end_start_y*edge_start_p_y;
	double C = edge_start_p_x * edge_start_p_x + edge_start_p_y * edge_start_p_y;
	
	double t;
    if (A < eps || ((t = -B / A)< eps))
	{
		proj = edge_start;
		return C;
	}
	if (t > 1)
	{
		proj = edge_end;
		return A + 2 * B + C;
	}
	auto edge_start_x = TypeAccessor::getRefX(edge_start);
	auto edge_start_y = TypeAccessor::getRefY(edge_start);

	typedef decltype(TypeAccessor::getValX(proj)) fieldTypeX;
	typedef decltype(TypeAccessor::getValY(proj)) fieldTypeY;

	auto proj_x = fieldTypeX(edge_start_x + t * edge_end_start_x);
	auto proj_y = fieldTypeY(edge_start_y + t * edge_end_start_y);
	
	TypeAccessor::getRefX(proj) = proj_x;
	TypeAccessor::getRefY(proj) = proj_y;
	return -B*B / A + C;
};
