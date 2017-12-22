//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mariadb++/account.hpp>
#include <mariadb++/conversion_helper.hpp>

using namespace mariadb;

account::account(const std::string &host_name, const std::string &user_name,
                 const std::string &password, const std::string &schema, u32 port,
                 const std::string &unix_socket)
    : m_auto_commit(true),
      m_port(port),
      m_host_name(host_name),
      m_user_name(user_name),
      m_password(password),
      m_schema(schema),
      m_unix_socket(unix_socket) {}

const std::string &account::host_name() const { return m_host_name; }

const std::string &account::user_name() const { return m_user_name; }

const std::string &account::password() const { return m_password; }

const std::string &account::unix_socket() const { return m_unix_socket; }

u32 account::port() const { return m_port; }

const std::string &account::ssl_key() const { return m_ssl_key; }

const std::string &account::ssl_certificate() const { return m_ssl_certificate; }

const std::string &account::ssl_ca() const { return m_ssl_ca; }

const std::string &account::ssl_ca_path() const { return m_ssl_ca_path; }

const std::string &account::ssl_cipher() const { return m_ssl_cipher; }

const std::string &account::schema() const { return m_schema; }

void account::set_schema(const std::string &schema) { m_schema = schema; }

void account::set_ssl(const std::string &key, const std::string &certificate, const std::string &ca,
                      const std::string &ca_path, const std::string &cipher) {
    m_ssl_key = key;
    m_ssl_certificate = certificate;
    m_ssl_ca = ca;
    m_ssl_ca_path = ca_path;
    m_ssl_cipher = cipher;
}

bool account::auto_commit() const { return m_auto_commit; }

void account::set_auto_commit(bool auto_commit) { m_auto_commit = auto_commit; }

const std::map<std::string, std::string> &account::options() const { return m_options; }

const std::string account::option(const std::string &name) const {
    const map_options_t::const_iterator value = m_options.find(name);

    // return option value if found
    return value == m_options.end() ? "" : value->second;
}

void account::set_option(const std::string &name, const std::string &value) {
    m_options[name] = value;
}

void account::clear_options() { m_options.clear(); }

account_ref account::create(const std::string &host_name, const std::string &user_name,
                            const std::string &password, const std::string &schema, u32 port,
                            const std::string &unix_socket) {
    return account_ref(new account(host_name, user_name, password, schema, port, unix_socket));
}
