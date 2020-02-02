# mariadb++
C++ client library for MariaDB. Uses the C connector.

## Features
* Prepared statements
* Transactions and savepoints
* Concurrency allows connection sharing between threads
* Data type support: blob, decimal, datetime, time, timespan, etc.
* Exceptions

## Setup
1. Install `mariadbclient` or `mysqlclient` libraries.
2. Build and install. E.g.

```shell
# In this project directory
mkdir build
cd build
cmake ..
make install
```

3. Use CMake to declare the dependency to `mariadbclientpp`:

```cmake
# CMakeLists.txt of your project
find_package(mariadbclientpp REQUIRED)
...
target_link_libraries(yourtarget PUBLIC mariadbclientpp::mariadbclientpp)
```

### Building tests
Create database and user according to the information in [test/CMakeLists.txt](test/CMakeLists.txt) or adjust these values.

## Usage example
```c++
// set up the account
account_ref acc = account::create(...);

// create connection
connection_ref con = connection::create(acc);

// insert, update, select on the connection
u64 id = con->insert("INSERT INTO table VALUES (1, 2, 3)");
u64 affected = con->execute("UPDATE table SET a=1");
result_set_ref result = con->query("SELECT * FROM table");

// create statement
statement_ref stmt = con->create_statement(...);

// insert, update, select on the statement
u64 id = stmt->insert();
u64 affected = stmt->execute();
result_set_ref result = stmt->query();

// reading from a result set
while (result->next()) {
    int a = result->get_unsigned32(0);
    int b = result->get_unsigned32("b");
}

// insert using prepared statement
statement_ref stmt = con->create_statement("INSERT INTO table (?, ?, ?)");
stmt->set_unsigned32(0, 13);
stmt->set_unsigned32(1, 37);
stmt->set_unsigned32(2, 42);
stmt->insert();

```
More usage examples can be found in the `test/` directory.

## License
This library is subject to the Boost Software License. See accompanying [LICENSE](LICENSE) file.
