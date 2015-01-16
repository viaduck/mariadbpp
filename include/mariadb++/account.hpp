//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_ACCOUNT_HPP_
#define _MARIADB_ACCOUNT_HPP_

#include <string>
#include <map>
#include <mariadb++/types.hpp>

//
// Account is an helper class used for storing
// HOST / USER NAME / PASSWORD / DEFAULT SCHEMA / AUTO COMMIT / SSL / OPTION(S)
// without the need to setup manually the connection each time
//
// Please note you must set the value before a connection, once the connection is created
// the account is no longer used by the connection
//

namespace mariadb
{
	class account;
	typedef MARIADB_STD::shared_ptr<account> account_ref;

	class account
	{
	public:
		typedef std::map<std::string, std::string> map_options_t;

	public:
		//
		// Constructor
		//
		account(const char* host_name, const char* user_name, const char* password, const char* schema = NULL, u32 port = 0, const char* unix_socket = NULL);

		//
		// Destructor
		//
		virtual ~account();

		//
		// Get account informations
		//
		const std::string& host_name() const;
		const std::string& user_name() const;
		const std::string& password() const;
		const std::string& unix_socket() const;
		const std::string& ssl_key() const;
		const std::string& ssl_certificate() const;
		const std::string& ssl_ca() const;
		const std::string& ssl_ca_path() const;
		const std::string& ssl_cipher() const;
		u32 port() const;

		//
		// Get / Set default schema
		//
		const std::string& schema() const;
		void set_schema(const char* schema);

		//
		// Establishing secure connections using SSL
		//
		void set_ssl(const char* key, const char* certificate, const char* ca, const char* ca_path, const char* cipher);

		//
		// Set auto commit mode
		//
		bool auto_commit() const;
		void set_auto_commit(bool auto_commit);

		//
		// Get / Set options
		//
		const std::string& option(const char* name) const;
		const std::map<std::string, std::string>& options() const;
		void set_option(const char* name, const char* value);
		void clear_options();

	public:
		//
		// Account creation helper
		//
		static account_ref create(const char* host_name, const char* user_name, const char* password, const char* schema = NULL, u32 port = 0, const char* unix_socket = NULL);

	private:
		bool        m_auto_commit;
		u32         m_port;
		std::string m_host_name;
		std::string m_user_name;
		std::string m_password;
		std::string m_schema;
		std::string m_unix_socket;
		std::string m_ssl_key;
		std::string m_ssl_certificate;
		std::string m_ssl_ca;
		std::string m_ssl_ca_path;
		std::string m_ssl_cipher;
		map_options_t m_options;
	};
}

#endif
