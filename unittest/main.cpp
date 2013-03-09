//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <getopt.h>
#include <iostream>
#include "test.hpp"

using namespace std;
using namespace mariadb;

namespace
{
	char*        g_schema = const_cast<char *>("test_mariadb_plus_plus");
	char*        g_host_name = NULL;
	char*        g_user_name = NULL;
	char*        g_password = NULL;
	char*        g_socket_name = NULL;
	unsigned int g_port = 0;
}

//
// Show help
//
static void usage()
{
	printf("Execute test with the following options:\n");
	printf("-h hostname\n");
	printf("-u username\n");
	printf("-p password\n");
	printf("-d database\n");
	printf("-S socketname\n");
	printf("-P port number\n");
	printf("?  displays this help and exits\n");
}

//
// Get options
//
void get_options(int argc, char **argv)
{
	int c = 0;

	while ((c = getopt(argc, argv, "h:u:p:d:P:S:?")) >= 0)
	{
		switch (c)
		{
			case 'h':
				g_host_name = optarg;
				break;

			case 'u':
				g_user_name = optarg;
				break;

			case 'p':
				g_password = optarg;
				break;

			case 'd':
				g_schema = optarg;
				break;

			case 'P':
				g_port = atoi(optarg);
				break;

			case 'S':
				g_socket_name = optarg;
				break;

			case '?':
				usage();
				exit(0);
				break;

			default:
				printf("Unknown option %c\n", c);
				usage();
				exit(0);
				break;
		}
	}
}

//
// function get_envvars
//
// checks for connection related environment variables
//
void get_envvars()
{
	char* envvar;

	if (!g_host_name && (envvar = getenv("MYSQL_TEST_HOST")))
		g_host_name = envvar;

	if (!g_user_name && (envvar = getenv("MYSQL_TEST_USER")))
		g_user_name = envvar;

	if (!g_password && (envvar = getenv("MYSQL_TEST_PASSWD")))
		g_password = envvar;

	if (!g_schema && (envvar = getenv("MYSQL_TEST_DB")))
		g_schema = envvar;

	if (!g_port && (envvar = getenv("MYSQL_TEST_PORT")))
		g_port = atoi(envvar);

	if (!g_socket_name && (envvar = getenv("MYSQL_TEST_SOCKET")))
		g_socket_name = envvar;
}

int main(int argc, char **argv)
{
	if (argc > 1)
		get_options(argc, argv);

	get_envvars();

	if (!g_host_name ||
		!g_user_name ||
		!g_password)
	{
		if (!g_host_name)
			std::cout << "Invalid host name\n";

		if (!g_user_name)
			std::cout << "Invalid user name\n";

		if (!g_password)
			std::cout << "Invalid password\n";

		usage();
		return 0;
	}

	unittest::test test(g_host_name, g_user_name, g_password, g_schema, g_port, g_socket_name);
    return test.run();
}
