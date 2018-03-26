//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_STRUCTURETEST_H
#define MARIADBCLIENTPP_STRUCTURETEST_H

#include "SkeletonTest.h"

class StructureTest : public SkeletonTest {
   protected:
    virtual void CreateTestTable() override {}
};

#endif  // MARIADBCLIENTPP_STRUCTURETEST_H
