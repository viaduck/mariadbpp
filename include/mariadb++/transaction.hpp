//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_TRANSACTION_HPP_
#define _MARIADB_TRANSACTION_HPP_

#include <vector>
#include "save_point.hpp"
#include "types.hpp"

namespace mariadb
{
	//
	// Transaction only work with connection for a short period of time, it's
	// why the connection pointer is not a reference to the shared object.
	//
	// If a transaction exist while a connection as been closed, your code need to be changed.
	//
	class connection;
	class save_point;
	class transaction
	{
		friend class connection;
		friend class save_point;

	public:
		//
		// Destructor, rollback automatically
		//
		virtual ~transaction();

		//
		// Commit the change
		//
		void commit();

		//
		// Create save point
		//
		save_point_ref create_save_point();

	private:
		//
		// Constructor
		//
		transaction(connection* connection, isolation::level level, bool consistent_snapshot);

		//
		// Remove save_point
		//
		void remove_save_point(save_point* save_point);

		//
		// Cleanup transaction
		//
		void cleanup();

	private:
		connection*              m_connection;
		std::vector<save_point*> m_save_points;
	};

	typedef MARIADB_STD::shared_ptr<transaction> transaction_ref;
}

#endif
