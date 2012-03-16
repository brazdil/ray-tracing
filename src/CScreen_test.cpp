/*
 * CScreen_test.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifdef GTEST_TESTING

#include <iostream>
#include "CScreen.h"
#include "extra_math.h"
#include "gtest/gtest.h"

TEST(ScreenTest, IllegalArguments) {
	Vector3d camera_position, camera_lookat;
	camera_position << 0, 0, 0;
	camera_lookat << 0, 0, 1;
	CCamera camera(camera_position, camera_lookat, M_PI / 3.0);

	ASSERT_NO_THROW(CScreen(camera, 2.0, 4.0 / 3.0));

	ASSERT_THROW(CScreen(camera, 0.0, 4.0 / 3.0), std::invalid_argument);
	ASSERT_THROW(CScreen(camera, -1.0, 4.0 / 3.0), std::invalid_argument);

	ASSERT_THROW(CScreen(camera, 2.0, 0.0), std::invalid_argument);
	ASSERT_THROW(CScreen(camera, 2.0, -4.0 / 3.0), std::invalid_argument);
}

TEST(ScreenTest, SimpleScreen) {
	Vector3d camera_position, camera_lookat;
	camera_position << 0, 0, 0;
	camera_lookat << 0, 0, 1;
	CCamera camera(camera_position, camera_lookat, M_PI / 2.0);
	CScreen screen(camera, 2.0, 4.0 / 3.0);

	Vector3d top_left, horizontal, vertical;
	top_left << -2.0, 1.5, 2.0;
	horizontal << 4.0, 0.0, 0.0;
	vertical << 0.0, -3.0, 0.0;

	EXPECT_TRUE(isEqual(top_left, screen.getTopLeftCorner()));
	EXPECT_TRUE(isEqual(horizontal, screen.getHorizontal()));
	EXPECT_TRUE(isEqual(vertical, screen.getVertical()));
}

#endif
