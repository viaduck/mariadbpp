//
//  M A R I A D B + +
//
//	Author   : Piotr Likus (https://github.com/vpiotr/decimal_for_cpp)
//	Adapted  : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_DECIMAL_HPP_
#define _MARIADB_DECIMAL_HPP_

#include <string>
#include <mariadb++/types.hpp>

namespace mariadb
{
	class decimal
	{
	private:
		static const s64 g_factors[];

	public:
		//
		// Constructors
		//
		decimal();
		decimal(const decimal& src);
		decimal(const char* string);
		template <typename type>
		explicit decimal(type value, u8 precision) :
			m_precision(precision)
		{
			m_value = round(g_factors[precision] * static_cast<type>(value));
		}

		//
		// Methods
		//
		s64 factor() const;
		u8 precision() const;
		decimal abs() const;

		//
		// Operators
		//
		int compare(const decimal& dec) const;
		decimal& operator = (const decimal& dec);
		decimal& operator = (s64 dec);
		decimal& operator = (s32 dec);
		decimal& operator = (f32 dec);
		decimal& operator = (f64 dec);
		bool operator == (const decimal& dec) const;
		bool operator < (const decimal& dec) const;
		bool operator <= (const decimal& dec) const;
		bool operator > (const decimal& dec) const;
		bool operator >= (const decimal& dec) const;
		bool operator != (const decimal& dec) const;
		const decimal operator + (const decimal& dec) const;
		decimal& operator += (const decimal& dec);
		const decimal operator - (const decimal& dec) const;
		decimal& operator -= (const decimal& dec);
		const decimal operator * (const decimal& dec) const;
		decimal& operator *= (const decimal& dec);
		const decimal operator / (const decimal& dec) const;
		decimal& operator /= (const decimal& dec);

		//
		// String
		//
		std::string str() const;

		//
		// Get decimal
		//
		f32 float32() const;
		f64 double64() const;
		f128 double128() const;

		//
		// Returns interget value = real_value * (10 ^ precision)
		//
		s64 unbiased() const;

	private:
		//
		// Round
		//
		template <typename type>
		s64 round(type value)
		{
			return s64(value < 0.0 ? value - 0.5 : value + 0.5);
		}

		//
		// Convert a decimal to the same precision as this one
		//
		s64 convert(const decimal& dec) const;

	private:
		u8  m_precision;
		s64 m_value;
	};
}

#endif
