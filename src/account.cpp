//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

bool account::store_result() const { return m_store_result; }

void account::set_store_result(bool store_result) { m_store_result = store_result; }

const account::map_options_t &account::options() const { return m_options; }

const std::string account::option(const std::string &name) const {
    const map_options_t::const_iterator value = m_options.find(name);

    // return option value if found
    return value == m_options.end() ? "" : value->second;
}

void account::set_option(const std::string &name, const std::string &value) {
    m_options[name] = value;
}

void account::clear_options() { m_options.clear(); }

const account::map_connect_options_t &account::connect_options() const { return m_connect_options; }

void account::clear_connect_options() { m_connect_options.clear(); }

void account::set_connect_option(mysql_option option, bool arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_bool(arg));
}

void account::set_connect_option(mysql_option option, int arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_int(arg));
}

void account::set_connect_option(mysql_option option, const std::string &arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_string(arg));
}


account_ref account::create(const std::string &host_name, const std::string &user_name,
                            const std::string &password, const std::string &schema, u32 port,
                            const std::string &unix_socket) {
    return account_ref(new account(host_name, user_name, password, schema, port, unix_socket));
}
