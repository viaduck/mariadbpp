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

namespace mariadb
{
	class account;
	typedef std::shared_ptr<account> account_ref;

	/**
	 * Class used to store account and connection information used by mariadb::connection when connecting.
	 * Note that modifying an account after the connection was established is useless.
	 */
	class account
	{
	public:
		typedef std::map<std::string, std::string> map_options_t;

	public:
		/**
		 * Destructs the account
		 */
		virtual ~account() { }

		/**
		 * Gets the name of the host to connect to
		 */
		const std::string& host_name() const;

        /**
         * Gets the username to log in with
         */
		const std::string& user_name() const;

        /**
         * Gets the password of the user to log in with
         */
		const std::string& password() const;

        /**
         * Gets the unix socket path to connect to.
         * If this option is set, host and port will be ignored
         */
		const std::string& unix_socket() const;

        /**
         * Gets the path to the key file
         */
		const std::string& ssl_key() const;

        /**
         * Gets the path to the certificate file
         */
		const std::string& ssl_certificate() const;

        /**
         * Gets the path to the certificate authority file
         */
		const std::string& ssl_ca() const;

        /**
         * Gets the path to the directory containing CA files
         */
		const std::string& ssl_ca_path() const;

        /**
         * Gets the list of allowed SSL ciphers
         */
		const std::string& ssl_cipher() const;

        /**
         * Gets the port to connect to
         */
		u32 port() const;

		/**
		 * Gets the name of the database to open on connect
		 */
		const std::string& schema() const;

        /**
         * Sets the name of the database to open on connect
         */
		void set_schema(const std::string &schema);

		/**
		 * Set SSL options. All files should be PEM format
		 *
		 * @param key           Path to the key file
		 * @param certificate   Path to the certificate file
		 * @param ca            Path to the certificate authority file
		 * @param ca_path       Path to a directory containing CA files
		 * @param cipher        List of allowed SSL ciphers. See MariaDB manual for possible values
		 */
		void set_ssl(const std::string &key, const std::string &certificate, const std::string &ca,
                     const std::string &ca_path, const std::string &cipher);

		/**
		 * Gets the current state of the auto_commit option. This option is turned on by default.
		 */
		bool auto_commit() const;

        /**
         * Sets the state of the auto_commit option.
         */
		void set_auto_commit(bool auto_commit);

		/**
		 * Gets the current value of any named option that was previously set
		 *
		 * @return Value of the found option or empty string if not found
		 */
		const std::string& option(const std::string &name) const;

        /**
         * Gets a map of all option key/value pairs previously set
         */
		const std::map<std::string, std::string>& options() const;

        /**
         * Sets a named option key/value pair
         */
		void set_option(const std::string &name, const std::string &value);

        /**
         * Deletes all stored key/value pairs of named options
         */
		void clear_options();

        /**
         * Create an account
         *
         * @param host_name Hostname to connect to
         * @param user_name Username to log in with
         * @param password  Password for the user to log in with (may be empty)
         * @param schema    Database name to select on connect. Can also be set after connecting
         * @param port      Port of host to connect to (defaults to 3306)
         * @param unix_sock Path of unix socket to connect to. If specified, host and port will be ignored
         */
		static account_ref create(const std::string &host_name, const std::string &user_name,
                                  const std::string &password, const std::string &schema = "",
                                  u32 port = 3306, const std::string &unix_socket = "");

	private:
        /**
         * Private account constructor
         */
        account(const std::string &host_name, const std::string &user_name, const std::string &password,
                const std::string &schema, u32 port, const std::string &unix_sock);


		bool        m_auto_commit = true;
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
