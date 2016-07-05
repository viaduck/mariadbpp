//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_SAVE_POINT_HPP_
#define _MARIADB_SAVE_POINT_HPP_

#include <mariadb++/types.hpp>

namespace mariadb
{
	class connection;
	class transaction;
	class save_point
	{
		friend class connection;
		friend class transaction;

	public:
		//
		// Destructor, rollback automatically
		//
		virtual ~save_point();

		//
		// Commit the change
		//
		void commit();

	private:
		//
		// Constructor
		//
		save_point(transaction* trans);

	private:
		transaction* m_transaction;
		std::string  m_name;
	};

	typedef std::shared_ptr<save_point> save_point_ref;
}

#endif
