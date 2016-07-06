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

	/**
	 * Class used to represent a MariaDB savepoint having automatic rollback functionality
	 */
	class save_point
	{
		friend class connection;
		friend class transaction;

	public:
		/**
		 * Destructor initiates automatic rollback if changes were not committed
		 */
		virtual ~save_point();

		/**
		 * Commits the changes and releases savepoint
		 */
		void commit();

	private:
		/**
		 * Create save_point with given transaction
		 */
		save_point(transaction* trans);

	private:
		// internal transaction pointer
		transaction* m_transaction;
		// distinct name of the current save_pointS
		std::string  m_name;
	};

	typedef std::shared_ptr<save_point> save_point_ref;
}

#endif
