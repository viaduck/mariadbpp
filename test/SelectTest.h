//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_SELECTTEST_H
#define MARIADBCLIENTPP_SELECTTEST_H

#include "SkeletonTest.h"

class SelectTest : public SkeletonTest {
    // TODO missing: time-based types, string/byte-based types, statement-based tests

    virtual void CreateTestTable() override {}
};

#endif  // MARIADBCLIENTPP_SELECTTEST_H
