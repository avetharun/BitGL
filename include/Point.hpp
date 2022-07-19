#ifndef NIKONOVA_NOVA_BITGL_INCLUDE_POINT_HPP
#define NIKONOVA_NOVA_BITGL_INCLUDE_POINT_HPP






#include "Vec.hpp"






namespace BitGL
{
	struct Point2D : Vec2<int> {
		int x() {return this->at(0);}
		int y() {return this->at(1);}
		friend Point2D operator +(Point2D l, Point2D r){
			return Point2D{l[0] + r[0], l[1] + r[1]};
		}
		friend Point2D operator -(Point2D l, Point2D r){
			return Point2D{l[0] - r[0], l[1] - r[1]};
		}
		friend Point2D operator *(Point2D l, Point2D r){
			return Point2D{l[0] * r[0], l[1] * r[1]};
		}
		friend Point2D operator /(Point2D l, Point2D r){
			return Point2D{l[0] / r[0], l[1] / r[1]};
		}
	};
	constexpr auto to_1d_index(Point2D const &position, Vec2<int> const &size) -> std::size_t;
}






#include "impl/Point.ipp"






#endif
