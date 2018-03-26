# mariadb++
C++ client library for MariaDB. Uses the C connector.

## Features
* Prepared statements
* Transactions and savepoints
* Concurrency allows connection sharing between threads
* Data type support: blob, decimal, datetime, time, timespan, etc.
* Exceptions

## Setup
1. Initialize Git submodules: `git submodule update --init`
2. Install `mariadbclient` or `mysqlclient` libraries.

### Building tests
Create database and user according to the information in [test/CMakeLists.txt](test/CmakeLists.txt) or adjust these values.

## Usage
Link against the `mariadbclientpp` CMake target in your project.  
  
Usage examples can be found in the `test/` directory.

## License
This library is subject to the Boost Software License. See accompanying [LICENSE](LICENSE) file.
