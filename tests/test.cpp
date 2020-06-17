#include "pch.h"

#include <algorithm>
#include <complex>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include <gtest/gtest.h>
/////////////////////////////////////////////////////////////////////////////////////////
typedef std::string                     T_str;
typedef double                          T_coord;
typedef std::complex    < T_coord   >   T_point_val;
/////////////////////////////////////////////////////////////////////////////////////////
class   T_point
{
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	friend  std::istream& operator>>
		(
			std::istream& istr,
			T_point& point
			)
	{
		istr >> point.point_val_;
		return  istr;
	}
	//-----------------------------------------------------------------------------------
	friend  std::ostream& operator<<
		(
			std::ostream& ostr,
			T_point         const& point
			)
	{
		ostr << point.point_val_;
		return  ostr;
	}
	//-----------------------------------------------------------------------------------
public:
	T_point_val     point_val_;
	//-----------------------------------------------------------------------------------
	T_point(T_point_val    const& point_val = 0)
		:
		point_val_(point_val)
	{}
	//-----------------------------------------------------------------------------------
	T_point(T_coord    const& coord)
		:
		point_val_(coord)
	{}
	//-----------------------------------------------------------------------------------
	T_coord     x()                                                                 const
	{
		return  point_val_.real();
	}
	//-----------------------------------------------------------------------------------
	T_coord     y()                                                                 const
	{
		return  point_val_.imag();
	}
	//-----------------------------------------------------------------------------------
	T_point     operator+   (T_point   const& p)                             const
	{
		return      point_val_
			+ p.point_val_;
	}
	//-----------------------------------------------------------------------------------
	T_point     operator-   (T_point   const& p)                             const
	{
		return      *this + -p;
	}
	//-----------------------------------------------------------------------------------
	T_point     operator-   ()                                                      const
	{
		return  -point_val_;
	}
	//-----------------------------------------------------------------------------------
	T_point     operator*=  (T_point   const& p)
	{
		point_val_ *= p.point_val_;
		return  *this;
	}
	//-----------------------------------------------------------------------------------
	T_point     operator/=  (T_point   const& p)
	{
		point_val_ /= p.point_val_;
		return  *this;
	}
	//-----------------------------------------------------------------------------------
	bool    operator==  (T_point   const& p)                                 const
	{
		static  const   auto    factor = 1000;
		static  const   auto    eps = std::numeric_limits< T_coord >::epsilon();

		auto                    max_abs = std::max(
			abs(point_val_),
			abs(p.point_val_)
		);

		auto                    delta = (1 + max_abs)
			* eps
			* factor;

		return  abs(point_val_ - p.point_val_) < delta;
	}
	//-----------------------------------------------------------------------------------
	bool    operator!=  (T_point   const& p)                                 const
	{
		return  !(*this == p);
	}
	//-----------------------------------------------------------------------------------
	bool    operator<   (T_point   const& p)                                 const
	{
		return      std::make_pair
		(
			x(),
			y()
		)

			< std::make_pair
			(
				p.x(),
				p.y()
			);
	}
	//-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
T_point     operator*
(
	T_point     const& L,
	T_point     const& R
	)
{
	auto    res = L;
	return  res *= R;
}
/////////////////////////////////////////////////////////////////////////////////////////
T_point     operator/
(
	T_point     const& L,
	T_point     const& R
	)
{
	auto    res = L;
	return  res /= R;
}
/////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector     < T_point               >   T_points;
typedef std::pair       < T_point, T_point     >   T_side;
/////////////////////////////////////////////////////////////////////////////////////////
T_point     vect
(
	T_point     const& A,
	T_point     const& B
)
{
	return  B - A;
}
/////////////////////////////////////////////////////////////////////////////////////////
T_point     vect(T_side  const& side)
{
	return      side.second
		- side.first;
}
/////////////////////////////////////////////////////////////////////////////////////////
T_coord     det
(
	T_point     const& M,
	T_point     const& N
)
{
	return      M.x() * N.y()
		- M.y() * N.x();
}
/////////////////////////////////////////////////////////////////////////////////////////
T_coord     det
(
	T_side  const& side_1,
	T_side  const& side_2
)
{
	return  det(
		vect(side_1),
		vect(side_2)
	);
}
/////////////////////////////////////////////////////////////////////////////////////////
class   T_triangle
{
	//-----------------------------------------------------------------------------------
	T_point     A_;
	T_point     B_;
	T_point     C_;
	//-----------------------------------------------------------------------------------
public:
	T_triangle(T_point a, T_point b, T_point c) {
		A_ = a;
		B_ = b;
		C_ = c;
	}
	//-----------------------------------------------------------------------------------
	T_side  get_begin_side()                                                        const
	{
		return  T_side(A_, B_);
	}
	//-----------------------------------------------------------------------------------
	T_points    get_all_vertices()                                                  const
	{
		T_points    res_vertices;

		res_vertices.push_back(A_);
		res_vertices.push_back(B_);
		res_vertices.push_back(C_);

		return  res_vertices;
	}
	//-----------------------------------------------------------------------------------
	bool    is_vert(T_point    const& point)                                 const
	{
		return      point == A_
			|| point == B_
			|| point == C_;
	}
	//-----------------------------------------------------------------------------------
	T_side  get_next_side(T_side   const& side)                              const
	{
		return  get_side_goes_from_vert(side.second);
	}
	//-----------------------------------------------------------------------------------
	bool    point_is_inside(T_point    const& w)                             const
	{
		auto    v = begin_vert();

		do
		{
			if (
				det(
					vect(w, v),

					vect(
						w,
						get_next_vert(v)
					)
				)

				<= 0
				)
			{
				return  false;
			}

			v = get_next_vert(v);
		} while (
			v != begin_vert()
			);

		return  true;
	}
	//-----------------------------------------------------------------------------------
	T_point     begin_vert()                                                        const
	{
		return  A_;
	}
	//-----------------------------------------------------------------------------------
	T_side  get_side_goes_from_vert(T_point  const& v)                       const
	{
		return  T_side(
			v,
			get_next_vert(v)
		);
	}
	//-----------------------------------------------------------------------------------
	T_point     get_next_vert(T_point  const& v)                             const
	{
		if (v == A_)
		{
			return  B_;
		}
		else if (v == B_)
		{
			return  C_;
		}
		else
		{
			return  A_;
		}//else
	}
	//-----------------------------------------------------------------------------------
	void    input_vertices_with_triangle_name(T_str    const& triangle_name)
	{
		std::cout << "\t"
			<< triangle_name
			<< std::endl
			<< "\t\tA\t= ";

		std::cin >> A_;

		do
		{
			std::cout << "\t\tB\t= ";
			std::cin >> B_;
		} while (
			B_ == A_
			);

		do
		{
			std::cout << "\t\tC\t= ";
			std::cin >> C_;
		} while (
			det(
				vect(A_, B_),
				vect(A_, C_)
			)

			== 0
			);

		std::cout << std::endl;
		arrange_vertices_counterclockwise();
	}
	//-----------------------------------------------------------------------------------
private:
	//-----------------------------------------------------------------------------------
	void    arrange_vertices_counterclockwise()
	{
		if (
			det(
				vect(A_, B_),
				vect(A_, C_)
			)
			< 0
			)
		{
			std::swap(B_, C_);
		}
	}
	//-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
bool    successfully_for_sides_set_intersection_point
(
	T_side      const& side_AB,
	T_side      const& side_CD,
	T_point& intersec_point
)
{
	bool    bool_res = false;
	/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//Находим точку R пересечения прямых, задаваемых векторами AB и CD.
		//R = A + AB * x
		//R = C + CD * y
		//A + AB * x = C + CD * y
		//AB * x - CD * y = C - A
		//
		//AB * x - CD * y = AC

	auto    vect_AB = vect(side_AB);
	auto    vect_CD = vect(side_CD);

	auto    vect_AC = vect(
		side_AB.first,
		side_CD.first
	);

	auto    main_det = det(
		vect_AB,
		-vect_CD
	);

	bool_res = main_det != 0;

	if (bool_res)
	{
		auto    det_X = det(
			vect_AC,
			-vect_CD
		);

		auto    X = det_X / main_det;

		auto    det_Y = det(
			vect_AB,
			vect_AC
		);

		auto    Y = det_Y / main_det;

		bool    intersection_point_belongs_to_both_sides = 0 <= X
			&& X <= 1
			&& 0 <= Y
			&& Y <= 1;

		bool_res = intersection_point_belongs_to_both_sides;

		if (bool_res)
		{
			intersec_point = side_AB.first
				+ X * vect_AB;
		}//if
	}//else

	return  bool_res;
}
/////////////////////////////////////////////////////////////////////////////////////////
class   T_intersection_contour
{
	//-----------------------------------------------------------------------------------
	typedef std::vector < T_triangle    >   T_triangles;
	typedef bool                            T_first_second_ind;
	//-----------------------------------------------------------------------------------
	static  const   T_first_second_ind  first = false;
	static  const   T_first_second_ind  second = !first;
	//-----------------------------------------------------------------------------------
	T_triangles         triangles_;

	T_point             cur_vert_;
	T_first_second_ind  cur_triangle_ind_;
	T_point             cur_triangle_directive_vert_;
	//-----------------------------------------------------------------------------------
public:
	T_points            vertices_;
	//-----------------------------------------------------------------------------------
	T_intersection_contour
	(
		T_triangle  const& triangle_1,
		T_triangle  const& triangle_2
	)
	{
		triangles_.push_back(triangle_1);
		triangles_.push_back(triangle_2);
	}
	//-----------------------------------------------------------------------------------
	void    fill_vertices()//заполнение массива точек пересечения
	{
		if (
			triangles_coincide()
			)
		{
			vertices_ = for_triangle_with_ind_get_all_vertices(first);
		}
		else if (
			successfully_set_first_cur_vert_data()
			)
		{
			do
			{
				vertices_.push_back(cur_vert_);
				inc_cur_vert();
			} while (
				cur_vert_ != vertices_.front()
				);
		}//if
	}
	//-----------------------------------------------------------------------------------
	void    print_vertices()                                                        const
	{
		if (
			vertices_.empty()
			)
		{
			std::cout << "Треугольники не имеют контура пересечения."
				<< std::endl;
		}
		else
		{
			std::cout << "Контур пересечения треугольников состоит из точек:"
				<< std::endl;

			std::copy
			(
				vertices_.begin(),
				vertices_.end(),
				std::ostream_iterator< T_point >(std::cout, "\t")
			);
		}
	}
	//-----------------------------------------------------------------------------------
private:
	//-----------------------------------------------------------------------------------
	T_points    for_triangle_with_ind_get_all_vertices(T_first_second_ind  triangle_ind)  const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).get_all_vertices();
	}
	//-----------------------------------------------------------------------------------
	bool    triangles_coincide()                                                    const
	{
		auto    vertices_first = for_triangle_with_ind_get_all_vertices(first);
		auto    vertices_second = for_triangle_with_ind_get_all_vertices(second);

		std::sort(vertices_first.begin(), vertices_first.end());
		std::sort(vertices_second.begin(), vertices_second.end());

		return  vertices_first == vertices_second;
	}
	//-----------------------------------------------------------------------------------
	void    inc_cur_vert()
	{
		auto    cur_side = T_side(
			cur_vert_,
			cur_triangle_directive_vert_
		);

		if (
			!successfully_cur_side_intersects_other_triangle_and_set_cur_vert_data(cur_side)
			)
		{
			cur_vert_ = cur_triangle_directive_vert_;

			for_triangle_with_ind_inc_vert
			(
				cur_triangle_ind_,
				cur_triangle_directive_vert_
			);
		}//if
	}
	//-----------------------------------------------------------------------------------
	void    for_triangle_with_ind_inc_vert
	(
		T_first_second_ind      triangle_ind,
		T_point& vert
	)
	{
		vert = get_const_triangle_ref_with_ind(triangle_ind).get_next_vert(vert);
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_cur_side_intersects_other_triangle_and_set_cur_vert_data(T_side   const& side)
	{
		bool    bool_res = false;
		auto    outside_triangle_ind = !cur_triangle_ind_;
		auto    outside_begin_side = for_triangle_with_ind_get_begin_side(outside_triangle_ind);
		auto    outside_cur_side = outside_begin_side;

		do
		{
			bool_res = successfully_for_cur_side_and_side_set_cur_intersec_vert_data
			(
				side,
				outside_cur_side
			);

			if (bool_res)
			{
				return  bool_res;
			}

			for_triangle_with_ind_inc_side
			(
				outside_triangle_ind,
				outside_cur_side
			);
		} while (
			outside_cur_side
			!= outside_begin_side
			);

		return  bool_res;
	}
	//-----------------------------------------------------------------------------------
	T_side  for_triangle_with_ind_get_begin_side(T_first_second_ind    triangle_ind)  const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).get_begin_side();
	}
	//-----------------------------------------------------------------------------------
	void    for_triangle_with_ind_inc_side
	(
		T_first_second_ind      triangle_ind,
		T_side& side
	)                                                                           const
	{
		side = get_const_triangle_ref_with_ind(triangle_ind).get_next_side(side);
	}
	//-----------------------------------------------------------------------------------
	T_point     for_triangle_with_ind_get_next_vert
	(
		T_first_second_ind              triangle_ind,
		T_point             const& vert
	)                                                                           const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).get_next_vert(vert);
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_for_cur_side_and_side_set_cur_intersec_vert_data
	(
		T_side  const& cur_side_AB,
		T_side  const& side_CD
	)
	{
		auto        det_AB_CD = det(
			cur_side_AB,
			side_CD
		);

		T_point     intersec_point;

		bool        bool_res = det_AB_CD > 0

			&& successfully_for_sides_set_intersection_point
			(
				cur_side_AB,
				side_CD,
				intersec_point
			)

			&& intersec_point != side_CD.second;

		if (bool_res)
		{
			for_cur_side_and_side_and_intersec_point_set_cur_intersec_vert_data
			(
				cur_side_AB,
				side_CD,
				intersec_point
			);
		}//if

		return  bool_res;
	}
	//-----------------------------------------------------------------------------------
	void    for_cur_side_and_side_and_intersec_point_set_cur_intersec_vert_data
	(
		T_side      const& cur_side_AB,
		T_side      const& side_CD,
		T_point     const& intersec_point
	)
	{
		auto    cur_triangle_ind_side_or_next_side
			= for_triangle_with_ind_side_and_intersec_point_get_side_or_next_side
			(
				cur_triangle_ind_,
				cur_side_AB,
				intersec_point
			);

		auto    not_cur_triangle_ind_side_or_next_side
			= for_triangle_with_ind_side_and_intersec_point_get_side_or_next_side
			(
				!cur_triangle_ind_,
				side_CD,
				intersec_point
			);

		auto    det_cur_and_not_cur = det(
			cur_triangle_ind_side_or_next_side,
			not_cur_triangle_ind_side_or_next_side
		);

		cur_vert_ = intersec_point;

		cur_triangle_directive_vert_ = det_cur_and_not_cur > 0
			? not_cur_triangle_ind_side_or_next_side.second
			: cur_triangle_ind_side_or_next_side.second;

		if (det_cur_and_not_cur > 0)
		{
			cur_triangle_ind_ = !cur_triangle_ind_;
		}
	}
	//-----------------------------------------------------------------------------------
	T_side  for_triangle_with_ind_side_and_intersec_point_get_side_or_next_side
	(
		T_first_second_ind              triangle_ind,
		T_side              const& side,
		T_point             const& intersec_point
	)                                                                           const
	{
		return  for_triangle_with_ind_is_vert
		(
			triangle_ind,
			intersec_point
		)

			? for_triangle_with_ind_get_side_goes_from_vert
			(
				triangle_ind,
				intersec_point
			)

			: side;
	}
	//-----------------------------------------------------------------------------------
	bool    for_triangle_with_ind_is_vert
	(
		T_first_second_ind              triangle_ind,
		T_point             const& intersec_point
	)                                                                           const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).is_vert(intersec_point);
	}
	//-----------------------------------------------------------------------------------
	T_triangle  const& get_const_triangle_ref_with_ind(T_first_second_ind   triangle_ind)     const
	{
		return  triangles_[triangle_ind];
	}
	//-----------------------------------------------------------------------------------
	T_side  for_triangle_with_ind_get_side_goes_from_vert
	(
		T_first_second_ind              triangle_ind,
		T_point             const& vert
	)                                                                           const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).get_side_goes_from_vert(vert);
	}
	//-----------------------------------------------------------------------------------
	void    cur_vert_go_to_next_vert_in_triangle_with_ind(T_first_second_ind      triangle_ind)
	{
		for_triangle_with_ind_inc_vert
		(
			triangle_ind,
			cur_vert_
		);
	}
	//-----------------------------------------------------------------------------------
	T_point     for_triangle_with_ind_get_begin_vert(T_first_second_ind    triangle_ind)  const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).begin_vert();
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data()
	{
		return      successfully_set_first_cur_vert_data_as_sides_intersection_not_side_end()
			|| successfully_set_first_cur_vert_data_as_located_inside_triangle()
			|| successfully_set_first_cur_vert_data_as_begin_vert_of_side_middle_of_which_lies_inside_triangle();
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data_as_begin_vert_of_side_middle_of_which_lies_inside_triangle()
	{
		return
			successfully_set_first_cur_vert_data_as_begin_vert_of_side_middle_of_which_lies_inside_triangle_with_ind(first)
			|| successfully_set_first_cur_vert_data_as_begin_vert_of_side_middle_of_which_lies_inside_triangle_with_ind(second);
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data_as_begin_vert_of_side_middle_of_which_lies_inside_triangle_with_ind
	(T_first_second_ind    outside_triangle_ind)
	{
		bool    bool_res = false;
		auto    inside_triangle_ind = !outside_triangle_ind;

		auto    inside_begin_side = for_triangle_with_ind_get_begin_side(inside_triangle_ind);
		auto    inside_cur_side = inside_begin_side;

		do
		{
			bool_res = middle_of_side_is_inside_triangle_with_ind
			(
				inside_cur_side,
				outside_triangle_ind
			);

			if (bool_res)
			{
				cur_vert_ = inside_cur_side.first;
				cur_triangle_directive_vert_ = inside_cur_side.second;
				cur_triangle_ind_ = inside_triangle_ind;
				return  bool_res;
			}

			for_triangle_with_ind_inc_side
			(
				inside_triangle_ind,
				inside_cur_side
			);
		} while (
			inside_cur_side
			!= inside_begin_side
			);

		return  bool_res;
	}
	//-----------------------------------------------------------------------------------
	bool    middle_of_side_is_inside_triangle_with_ind
	(
		T_side              const& side,
		T_first_second_ind              outside_triangle_ind
	)                                                                           const
	{
		auto    middle_point = (
			side.first
			+ side.second
			)
			/ 2.0;

		return  get_const_triangle_ref_with_ind(outside_triangle_ind).point_is_inside(middle_point);
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data_as_sides_intersection_not_side_end()
	{
		bool    bool_res = false;
		auto    triangle_first_start_side = for_triangle_with_ind_get_begin_side(first);
		auto    triangle_first_cur_side = triangle_first_start_side;

		do
		{
			auto    triangle_second_start_side = for_triangle_with_ind_get_begin_side(second);
			auto    triangle_second_cur_side = triangle_second_start_side;

			do
			{
				T_point     intersec_point;

				bool_res = successfully_for_sides_set_intersection_point_not_side_end
				(
					triangle_first_cur_side,
					triangle_second_cur_side,
					intersec_point
				);

				if (bool_res)
				{
					cur_vert_ = intersec_point;

					auto    det_first_second = det(
						triangle_first_cur_side,
						triangle_second_cur_side
					);

					if (det_first_second > 0)
					{
						cur_triangle_ind_ = second;
						cur_triangle_directive_vert_ = triangle_second_cur_side.second;
					}
					else
					{
						cur_triangle_ind_ = first;
						cur_triangle_directive_vert_ = triangle_first_cur_side.second;
					}//else

					return  bool_res;
				}//if

				for_triangle_with_ind_inc_side
				(
					second,
					triangle_second_cur_side
				);
			} while (
				triangle_second_cur_side != triangle_second_start_side
				);

			for_triangle_with_ind_inc_side
			(
				first,
				triangle_first_cur_side
			);
		} while (
			triangle_first_cur_side != triangle_first_start_side
			);

		return  bool_res;
	}
	//-----------------------------------------------------------------------------------
	static  bool    successfully_for_sides_set_intersection_point_not_side_end
	(
		T_side  const& side_AB,
		T_side  const& side_CD,
		T_point& intersec_point
	)
	{
		return      successfully_for_sides_set_intersection_point
		(
			side_AB,
			side_CD,
			intersec_point
		)

			&& intersec_point != side_AB.first
			&& intersec_point != side_AB.second
			&& intersec_point != side_CD.first
			&& intersec_point != side_CD.second;
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data_as_located_inside_triangle()
	{
		return      successfully_set_first_cur_vert_data_as_located_inside_triangle_with_ind(first)
			&& successfully_set_first_cur_vert_data_as_located_inside_triangle_with_ind(second);
	}
	//-----------------------------------------------------------------------------------
	bool    successfully_set_first_cur_vert_data_as_located_inside_triangle_with_ind
	(T_first_second_ind    outside_triangle_ind)
	{
		bool    bool_res = false;
		auto    inside_triangle_ind = !outside_triangle_ind;
		cur_vert_ = for_triangle_with_ind_get_begin_vert(inside_triangle_ind);

		do
		{
			bool_res = cur_vert_is_inside_triangle_with_ind(outside_triangle_ind);

			if (bool_res)
			{
				cur_triangle_ind_ = inside_triangle_ind;

				cur_triangle_directive_vert_ = for_triangle_with_ind_get_next_vert
				(
					cur_triangle_ind_,
					cur_vert_
				);

				return  bool_res;
			}//if

			cur_vert_go_to_next_vert_in_triangle_with_ind(inside_triangle_ind);
		} while (
			cur_vert_ != for_triangle_with_ind_get_begin_vert(inside_triangle_ind)
			);

		return  bool_res;
	}
	//-----------------------------------------------------------------------------------
	bool    cur_vert_is_inside_triangle_with_ind(T_first_second_ind    triangle_ind)  const
	{
		return  get_const_triangle_ref_with_ind(triangle_ind).point_is_inside(cur_vert_);
	}
	//-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
int     main(int argc, char* argv[]) {
	std::locale::global(std::locale(""));


	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(Triangle, Testthree) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(3);
	a1.point_val_.imag(4);

	b1.point_val_.real(0);
	b1.point_val_.imag(0);

	c1.point_val_.real(3);
	c1.point_val_.imag(-4);

	a2.point_val_.real(2);
	a2.point_val_.imag(0);

	b2.point_val_.real(5);
	b2.point_val_.imag(-3);

	c2.point_val_.real(5);
	c2.point_val_.imag(3);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 3);

}
TEST(Triangle, Testfive) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(3);
	a1.point_val_.imag(4);

	b1.point_val_.real(0);
	b1.point_val_.imag(0);

	c1.point_val_.real(3);
	c1.point_val_.imag(-4);

	a2.point_val_.real(2);
	a2.point_val_.imag(4);

	b2.point_val_.real(1);
	b2.point_val_.imag(0);

	c2.point_val_.real(2);
	c2.point_val_.imag(-4);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 5);

}

TEST(Triangle, Testfour) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(3);
	a1.point_val_.imag(4);

	b1.point_val_.real(0);
	b1.point_val_.imag(0);

	c1.point_val_.real(3);
	c1.point_val_.imag(-4);

	a2.point_val_.real(2);
	a2.point_val_.imag(4);

	b2.point_val_.real(1);
	b2.point_val_.imag(0);

	c2.point_val_.real(2);
	c2.point_val_.imag(-2);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 4);

}
TEST(Triangle, Testtwo) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(3);
	a1.point_val_.imag(4);

	b1.point_val_.real(0);
	b1.point_val_.imag(0);

	c1.point_val_.real(3);
	c1.point_val_.imag(-4);

	a2.point_val_.real(3);
	a2.point_val_.imag(4);

	b2.point_val_.real(4);
	b2.point_val_.imag(0);

	c2.point_val_.real(3);
	c2.point_val_.imag(-4);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 0);

}
TEST(Triangle, Testone) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(3);
	a1.point_val_.imag(4);

	b1.point_val_.real(0);
	b1.point_val_.imag(0);

	c1.point_val_.real(3);
	c1.point_val_.imag(-4);

	a2.point_val_.real(3);
	a2.point_val_.imag(4);

	b2.point_val_.real(5);
	b2.point_val_.imag(0);

	c2.point_val_.real(4);
	c2.point_val_.imag(-4);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 0);

}


TEST(Triangle, Test_together) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(6);
	a1.point_val_.imag(3);

	b1.point_val_.real(1);
	b1.point_val_.imag(3);

	c1.point_val_.real(3);
	c1.point_val_.imag(-1);

	a2.point_val_.real(6);
	a2.point_val_.imag(3);

	b2.point_val_.real(1);
	b2.point_val_.imag(3);

	c2.point_val_.real(3);
	c2.point_val_.imag(-1);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 3);

}
TEST(Triangle, Testsix) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(6);
	a1.point_val_.imag(3);

	b1.point_val_.real(1);
	b1.point_val_.imag(3);

	c1.point_val_.real(3);
	c1.point_val_.imag(-1);

	a2.point_val_.real(3);
	a2.point_val_.imag(4);

	b2.point_val_.real(1);
	b2.point_val_.imag(1);

	c2.point_val_.real(6);
	c2.point_val_.imag(1);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 6);

}
TEST(Triangle, Testcheckеtriangle_onepoint) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(6);
	a1.point_val_.imag(3);

	b1.point_val_.real(1);
	b1.point_val_.imag(3);

	c1.point_val_.real(6);
	c1.point_val_.imag(3);

	a2.point_val_.real(6);
	a2.point_val_.imag(1);

	b2.point_val_.real(3);
	b2.point_val_.imag(4);

	c2.point_val_.real(6);
	c2.point_val_.imag(1);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 1);

}
TEST(Triangle, Testcheckеtriangle_zeropoint) {
	T_point a1, a2, b1, b2, c1, c2;
	a1.point_val_.real(6);
	a1.point_val_.imag(3);

	b1.point_val_.real(1);
	b1.point_val_.imag(3);

	c1.point_val_.real(6);
	c1.point_val_.imag(3);

	a2.point_val_.real(6);
	a2.point_val_.imag(1);

	b2.point_val_.real(1);
	b2.point_val_.imag(1);

	c2.point_val_.real(6);
	c2.point_val_.imag(1);

	T_triangle  triangle_1(a1, b1, c1);
	T_triangle  triangle_2(a2, b2, c2);


	T_intersection_contour  intersection_contour
	(
		triangle_1,
		triangle_2
	);

	intersection_contour.fill_vertices();
	//intersection_contour.print_vertices     ();

	EXPECT_EQ(size(intersection_contour.vertices_), 0);

}