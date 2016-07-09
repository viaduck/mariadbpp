//
// Created by John Watson on 08.07.16.
//

#include "TimeTest.h"

TEST_F(TimeTest, testConstructors) {
    // valid times
    mariadb::time a;
    mariadb::time b(13, 37, 42, 000);
    mariadb::time c(b);

    // invalid times
    EXPECT_ANY_THROW(mariadb::time d(static_cast<u8>(24)));
    EXPECT_ANY_THROW(mariadb::time d(23u, 60u));
    EXPECT_ANY_THROW(mariadb::time d(23u, 59u, 62u));
    EXPECT_ANY_THROW(mariadb::time d(23u, 59u, 59u, 1000u));

    // test time.h constructors
    tm sometime;
    sometime.tm_sec = 42;
    sometime.tm_min = 37;
    sometime.tm_hour = 13;

    tm invtime;
    invtime.tm_hour = 41;

    time_t someothertime = ::time(nullptr);

    mariadb::time e(sometime);
    EXPECT_ANY_THROW(mariadb::time d(invtime));

    mariadb::time f(someothertime);

    // test string conversion
    mariadb::time g("00:00:00.000");
    mariadb::time h("23:59:59.999");
    mariadb::time i("13:37:42.000");
    mariadb::time j("18-59.59?0");
    mariadb::time k("8:9:5-01");
    mariadb::time l("8:59:59.0");
    EXPECT_ANY_THROW(mariadb::time d("24"));
    EXPECT_ANY_THROW(mariadb::time d("23:60"));
    EXPECT_ANY_THROW(mariadb::time d("23:59:62"));
    EXPECT_ANY_THROW(mariadb::time d("23:59:59.1000"));
    EXPECT_ANY_THROW(mariadb::time d("23:59:a59.1000"));
    EXPECT_ANY_THROW(mariadb::time m("859"));

    EXPECT_NE(a, b);
    EXPECT_EQ(b, c);
    EXPECT_EQ(c, e);
    EXPECT_NE(e, f);
    EXPECT_EQ(a, g);
    EXPECT_NE(g, h);
    EXPECT_EQ(e, i);

    EXPECT_EQ("00:00:00.000", g.str_time(true));
    EXPECT_EQ("23:59:59.999", h.str_time(true));
    EXPECT_EQ("13:37:42.000", i.str_time(true));
    EXPECT_EQ("18:59:59.000", j.str_time(true));
    EXPECT_EQ("08:09:05.001", k.str_time(true));
    EXPECT_EQ("08:59:59", l.str_time());
}

TEST_F(TimeTest, testArithm) {
    // test ms
    mariadb::time a(13, 37);
    mariadb::time b(13, 36, 59, 999);
    mariadb::time c = b.add_milliseconds(1);

    ASSERT_EQ(a, c);
    ASSERT_NE(a, b);

    // test day overflow with ms
    mariadb::time d(23, 59, 59, 999);
    mariadb::time e = d.add_milliseconds(1);
    ASSERT_EQ(mariadb::time(), e);

    // test add second
    mariadb::time f(13, 36, 59);
    mariadb::time g = f.add_seconds(1);
    ASSERT_EQ(a, g);

    // test day overflow with second
    mariadb::time h = d.add_seconds(1);
    ASSERT_EQ(mariadb::time(0, 0, 0, 999), h);

    // test add minute
    mariadb::time i(13, 36);
    mariadb::time j = i.add_minutes(1);
    ASSERT_EQ(a, j);

    // test day overflow with minute
    mariadb::time k = d.add_minutes(1);
    ASSERT_EQ(mariadb::time(0, 0, 59, 999), k);

    // test add hour
    mariadb::time l(12, 37);
    mariadb::time m = l.add_hours(1);
    ASSERT_EQ(a, m);

    // test day overflow with hour
    mariadb::time n = d.add_hours(1);
    ASSERT_EQ(mariadb::time(0, 59, 59, 999), n);

    // test subtraction
    ASSERT_EQ(d, n.add_hours(-1));
    ASSERT_EQ(d, k.add_minutes(-1));
    ASSERT_EQ(d, h.add_seconds(-1));
    ASSERT_EQ(d, e.add_milliseconds(-1));
}

TEST_F(TimeTest, testDiff) {
    mariadb::time a(13, 37, 42, 007);
    mariadb::time b(12, 37, 42, 007);
    mariadb::time c(23, 37, 42, 007);

    mariadb::time_span s12 = a.time_between(b);
    mariadb::time_span s21 = b.time_between(a);

    mariadb::time_span s23 = b.time_between(c);
    mariadb::time_span s32 = c.time_between(b);

    mariadb::time_span s31 = c.time_between(a);
    mariadb::time_span s13 = a.time_between(c);

    ASSERT_EQ(1, s12.hours());
    ASSERT_EQ(s12.hours(), s21.hours());
    ASSERT_NE(s12.negative(), s21.negative());

    ASSERT_EQ(11, s23.hours());
    ASSERT_EQ(s23.hours(), s32.hours());
    ASSERT_NE(s23.negative(), s32.negative());

    ASSERT_EQ(10, s31.hours());
    ASSERT_EQ(s31.hours(), s13.hours());
    ASSERT_NE(s31.negative(), s13.negative());
}

TEST_F(TimeTest, testNow) {
    mariadb::time n1 = time::now();
    mariadb::time n2 = time::now();
    mariadb::time n3 = time::now();

    mariadb::time un1 = time::now_utc();
    mariadb::time un2 = time::now_utc();
    mariadb::time un3 = time::now_utc();

    // make sure n4 and un4 are actually different
    sleep(1);
    mariadb::time n4 = time::now();
    mariadb::time un4 = time::now_utc();

    ASSERT_GE(n4, n3);
    ASSERT_GE(n3, n2);
    ASSERT_GE(n2, n1);

    ASSERT_GE(un4, un3);
    ASSERT_GE(un3, un2);
    ASSERT_GE(un2, un1);

    ASSERT_NE(n1, n4);
    ASSERT_NE(un1, un4);
}