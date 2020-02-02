# MIT License
#
# Copyright (c) 2017-2018 The ViaDuck Project
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

set(GTEST_SRC_DIR "" CACHE PATH "Path to GTest source")

if (GTEST_SRC_DIR)
    message(STATUS "Path to GTest source specified; adding to project")

    # add subdirectory so that gtest is built automatically
    if (NOT TARGET gtest)
        add_subdirectory(${GTEST_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/gtest)
    endif()
    set(GTEST_FOUND TRUE)
    set(GTEST_BOTH_LIBRARIES gtest gtest_main)
else()
    if (NOT GTEST_FOUND)
        message(STATUS "No path to GTest source specified. Trying to find GTest.")

        find_package(GTest)

        if (GTEST_FOUND)
            message(STATUS "Found GTest.")
        endif()
    endif()
endif()
