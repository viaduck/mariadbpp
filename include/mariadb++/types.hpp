//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_TYPES_HPP_
#define _MARIADB_TYPES_HPP_

#ifdef MARIADB_WITHOUT_CPP11
	#include <boost/shared_ptr.hpp>

	#define MARIADB_STD boost
#else
	#include <memory>

	#define MARIADB_STD std
#endif

namespace mariadb
{
    //
    // Default types
    //
    typedef unsigned char      u8;
    typedef unsigned short     u16;
    typedef unsigned int       u32;
    typedef signed char        s8;
    typedef signed short       s16;
    typedef signed int         s32;
    typedef float              f32;
    typedef double             f64;
    typedef long double        f128;

#if defined(_MSC_VER) || defined(__BORLANDC__)
	typedef unsigned __int64   u64;
	typedef signed __int64     s64;
#else
    typedef unsigned long long u64;
    typedef signed long long   s64;
#endif

	typedef u64                handle;

	//
	// Value type
	//
	namespace value
	{
		enum type
		{
			null = 0,
			blob,
			data,
			date,
			date_time,
			time,
			string,
			boolean,
			decimal,
			unsigned8,
			signed8,
			unsigned16,
			signed16,
			unsigned32,
			signed32,
			unsigned64,
			signed64,
			float32,
			double64,
			enumeration
		};
	}

	//
	// Isolation level
	//
	namespace isolation
	{
		enum level
		{
			repeatable_read = 0,
			read_committed,
			read_uncommitted,
			serializable
		};
	}

	//
	// Stream
	//
	typedef MARIADB_STD::shared_ptr<std::istream> stream_ref;
}

#endif
