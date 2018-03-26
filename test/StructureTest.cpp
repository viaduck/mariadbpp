//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "StructureTest.h"

TEST_F(StructureTest, testDateTime) {
    date_time dt("2000-01-02 03:04:05.666");
    date_time dt2(2000, 1, 2, 3, 4, 5, 666);

    EXPECT_EQ(dt, dt2);
    EXPECT_FALSE(date_time::valid_date(2000, 0, 32));

    dt = dt.add_milliseconds(1000 * 60 * 60 * 24 * 10);
    dt2 = dt2.add_days(10);

    EXPECT_EQ(dt, dt2);
    EXPECT_EQ("2000-01-12 03:04:05", dt2.str(false));
    EXPECT_EQ("2000-01-12 03:04:05.666", dt2.str(true));
}

TEST_F(StructureTest, testTime) {
    mariadb::time t("03:04:05.666");
    mariadb::time t2(3, 4, 5, 666);

    EXPECT_EQ(t, t2);

    t = t.add_milliseconds(1000 * 60 * 60 * 5);
    t2 = t2.add_hours(5);

    EXPECT_EQ(t, t2);
    EXPECT_EQ("08:04:05", t2.str_time(false));
    EXPECT_EQ("08:04:05.666", t2.str_time(true));
}

TEST_F(StructureTest, testTimeSpan) {
    date_time dt(2000, 1, 2, 3, 4, 5, 666);
    date_time dt2(2000, 1, 5, 5, 5, 5, 999);

    time_span ts = dt2.time_between(dt);
    EXPECT_EQ(3, ts.days());
    EXPECT_EQ(2, ts.hours());
    EXPECT_EQ(1, ts.minutes());
    EXPECT_EQ(333, ts.milliseconds());
}

TEST_F(StructureTest, testDecimal) {
    decimal d("24.1234");

    EXPECT_EQ(24.1234, d.double64());
    EXPECT_EQ("24.1234", d.str());
}