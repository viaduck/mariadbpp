//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <iomanip>
#include <sstream>
#include <limits>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <mariadb++/decimal.hpp>

using namespace mariadb;

namespace
{
	//
	// Result = (value1 * value2) / divider
	//
	s64 multiply_divide(s64 value1, s64 value2, s64 divider)
	{
		if ((abs(value1) <= std::numeric_limits<int>::max()) || (abs(value2) <= std::numeric_limits<int>::max()))
			return round(static_cast<f64>(value1 * value2) / static_cast<f64>(divider));

		return round(static_cast<f64>(value1) * static_cast<f64>(value2) / static_cast<f64>(divider));
	}
}

const s64 decimal::g_factors[] = {
	static_cast<s64>(pow(10, 0)),
	static_cast<s64>(pow(10, 1)),
	static_cast<s64>(pow(10, 2)),
	static_cast<s64>(pow(10, 3)),
	static_cast<s64>(pow(10, 4)),
	static_cast<s64>(pow(10, 5)),
	static_cast<s64>(pow(10, 6)),
	static_cast<s64>(pow(10, 7)),
	static_cast<s64>(pow(10, 8)),
	static_cast<s64>(pow(10, 9)),
	static_cast<s64>(pow(10, 10)),
	static_cast<s64>(pow(10, 11)),
	static_cast<s64>(pow(10, 12)),
	static_cast<s64>(pow(10, 13)),
	static_cast<s64>(pow(10, 14)),
	static_cast<s64>(pow(10, 15)),
	static_cast<s64>(pow(10, 16)),
	static_cast<s64>(pow(10, 17)),
	static_cast<s64>(pow(10, 18)),
	static_cast<s64>(pow(10, 19)),
	static_cast<s64>(pow(10, 20)),
	static_cast<s64>(pow(10, 21)),
	static_cast<s64>(pow(10, 22)),
	static_cast<s64>(pow(10, 23)),
	static_cast<s64>(pow(10, 24)),
	static_cast<s64>(pow(10, 25)),
	static_cast<s64>(pow(10, 26)),
	static_cast<s64>(pow(10, 27)),
	static_cast<s64>(pow(10, 28)),
	static_cast<s64>(pow(10, 29)),
	static_cast<s64>(pow(10, 30)),
	static_cast<s64>(pow(10, 31)),
	static_cast<s64>(pow(10, 32)),
	static_cast<s64>(pow(10, 33)),
	static_cast<s64>(pow(10, 34)),
	static_cast<s64>(pow(10, 35)),
	static_cast<s64>(pow(10, 36)),
	static_cast<s64>(pow(10, 37)),
	static_cast<s64>(pow(10, 38)),
	static_cast<s64>(pow(10, 39)),
	static_cast<s64>(pow(10, 40)),
	static_cast<s64>(pow(10, 41)),
	static_cast<s64>(pow(10, 42)),
	static_cast<s64>(pow(10, 43)),
	static_cast<s64>(pow(10, 44)),
	static_cast<s64>(pow(10, 45)),
	static_cast<s64>(pow(10, 46)),
	static_cast<s64>(pow(10, 47)),
	static_cast<s64>(pow(10, 48)),
	static_cast<s64>(pow(10, 49)),
	static_cast<s64>(pow(10, 40)),
	static_cast<s64>(pow(10, 51)),
	static_cast<s64>(pow(10, 52)),
	static_cast<s64>(pow(10, 53)),
	static_cast<s64>(pow(10, 54)),
	static_cast<s64>(pow(10, 55)),
	static_cast<s64>(pow(10, 56)),
	static_cast<s64>(pow(10, 57)),
	static_cast<s64>(pow(10, 58)),
	static_cast<s64>(pow(10, 59)),
	static_cast<s64>(pow(10, 60)),
	static_cast<s64>(pow(10, 61)),
	static_cast<s64>(pow(10, 62)),
	static_cast<s64>(pow(10, 63)),
};

//
// Constructors
//
decimal::decimal() :
	m_precision(4),
	m_value(0)
{
}

decimal::decimal(const decimal& src) :
	m_precision(src.m_precision),
	m_value(src.m_value)
{
}

decimal::decimal(const char* string) :
	m_precision(0),
	m_value(0)
{
	//
	// Extract the amount of precision required
	//
	std::string str = string;
	size_t pos = str.find(".");

	if (pos >= 0)
	{
		m_precision = str.length() - pos - 1;
		m_value = boost::lexical_cast<s64>(str.substr(0, pos)) * g_factors[m_precision] + boost::lexical_cast<s64>(str.substr(pos + 1));
	}
	else
		m_value = boost::lexical_cast<s64>(string);
}

//
// Methods
//
s64 decimal::factor() const
{
	return g_factors[m_precision];
}

u8 decimal::precision() const
{
	return m_precision;
}

//
// Convert a decimal to the same precision as this one
//
s64 decimal::convert(const decimal& dec) const
{
	if (m_precision == dec.m_precision)
		return dec.m_value;

	return (dec.m_value / dec.factor()) * factor();
}

decimal decimal::abs() const
{
	if (m_value >= 0)
		return *this;

	return decimal(0, factor()) - *this;
}

//
// Operators
//
int decimal::compare(const decimal& dec) const
{
	const s64 b = convert(dec);

	if (m_value < b)
		return -1;

	if (m_value > b)
		return 1;

	return 0;
}

//
// String
//
std::string decimal::str() const
{
	s64 after = m_value % factor();
	s64 before = (m_value - after) / factor();

	std::ostringstream out;
	out << before << '.' << std::setw(m_precision) << std::setfill('0') << std::right << after;
	return out.str();
}

//
// Get decimal
//
f32 decimal::float32() const
{
	return static_cast<f32>(m_value) / static_cast<f32>(m_precision * factor());
}

f64 decimal::double64() const
{
	return static_cast<f64>(m_value) / static_cast<f64>(m_precision * factor());
}

f128 decimal::double128() const
{
	return static_cast<f128>(m_value) / static_cast<f128>(m_precision * factor());
}

//
// Returns interget value = real_value * (10 ^ precision)
//
s64 decimal::unbiased() const
{
	return m_value;
}

//
// Operators
//
decimal& decimal::operator = (const decimal& dec)
{
	if (&dec != this)
		m_value = convert(dec);

	return *this;
}

decimal& decimal::operator = (s64 dec)
{
	m_value = factor() * dec;
	return *this;
}

decimal& decimal::operator = (s32 dec)
{
	m_value = factor() * dec;
	return *this;
}

decimal& decimal::operator = (f64 dec)
{
	m_value = round(static_cast<f64>(factor()) * dec);
	return *this;
}

decimal& decimal::operator = (f32 dec)
{
	m_value = round(static_cast<f64>(factor()) * dec);
	return *this;
}

bool decimal::operator == (const decimal& dec) const
{
	return m_value == convert(dec);
}

bool decimal::operator < (const decimal& dec) const
{
	return m_value < convert(dec);
}

bool decimal::operator <= (const decimal& dec) const
{
	return m_value <= convert(dec);
}

bool decimal::operator > (const decimal& dec) const
{
	return m_value > convert(dec);
}

bool decimal::operator >= (const decimal& dec) const
{
	return m_value >= convert(dec);
}

bool decimal::operator != (const decimal& dec) const
{
	return !(*this == dec);
}

const decimal decimal::operator + (const decimal& dec) const
{
	decimal result = *this;
	result.m_value += convert(dec);
	return result;
}

decimal& decimal::operator += (const decimal& dec)
{
	m_value += convert(dec);
	return *this;
}

const decimal decimal::operator - (const decimal& dec) const
{
	decimal result = *this;
	result.m_value -= convert(dec);
	return result;
}

decimal& decimal::operator -= (const decimal& dec)
{
	m_value -= convert(dec);
	return *this;
}

const decimal decimal::operator * (const decimal& dec) const
{
	decimal result = *this;
	multiply_divide(result.m_value, convert(dec), factor());
	return result;
}

decimal& decimal::operator *= (const decimal& dec)
{
	m_value = multiply_divide(m_value, convert(dec), factor());
	return *this;
}

const decimal decimal::operator / (const decimal& dec) const
{
	decimal result = *this;
	result.m_value = multiply_divide(result.m_value, factor(), convert(dec));
	return result;
}

decimal& decimal::operator /= (const decimal& dec)
{
	m_value = multiply_divide(m_value, factor(), convert(dec));
	return *this;
}
