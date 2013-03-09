//
//  A M A N A T S U   G A M E S
//
//	Engine : Amanatsu Games
//	Author : Sylvain Rochette Langlois
//
//	Copyright (c) 2007 by Amanatsu Games Inc. All Rights Reserved.
//

#include <sstream>
#include <mariadb++/exceptions.hpp>

using namespace mariadb::exception;
using namespace std;

//
// Constructors
//
date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond) throw() :
	base()
{
	std::ostringstream oss;
	oss << "Invalid date time: year - " << year << ", month - " << month << ", day - " << day
		<< ", hour - " << hour << ", minute - " << minute
		<< ", second - " << second << ", millisecond - " << millisecond;

	m_error = oss.str();
}

time::time(u8 hour, u8 minute, u8 second, u16 millisecond) throw() :
	base()
{
	std::ostringstream oss;
	oss << "Invalid time: hour - " << hour << ", minute - " << minute
		<< ", second - " << second << ", millisecond - " << millisecond;

	m_error = oss.str();
}
