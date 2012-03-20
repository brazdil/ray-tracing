/*
 * CScreen_test.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifdef GTEST_TESTING

#include <iostream>
#include "Screen.h"
#include "extra_math.h"
#include "gtest/gtest.h"

TEST(ScreenTest, IllegalArguments) {
	Vector3d camera_position, camera_lookat;
	camera_position << 0, 0, 0;
	camera_lookat << 0, 0, 1;
	pCamera camera = pCamera(new Camera(camera_position, camera_lookat, M_PI / 3.0));

	ASSERT_NO_THROW(Screen(camera, 2.0, Ratio(4, 3)));

	ASSERT_THROW(Screen(camera, 0.0, Ratio(4, 3)), std::invalid_argument);
	ASSERT_THROW(Screen(camera, -1.0, Ratio(4, 3)), std::invalid_argument);

	ASSERT_THROW(Screen(camera, 2.0, Ratio(1, 0)), std::invalid_argument);
	ASSERT_THROW(Screen(camera, 2.0, Ratio(0, 1)), std::invalid_argument);
	ASSERT_THROW(Screen(camera, 2.0, Ratio(0, 0)), std::invalid_argument);
}

TEST(ScreenTest, SimpleScreen) {
	Vector3d camera_position, camera_lookat;
	camera_position << 0, 0, 0;
	camera_lookat << 0, 0, 1;
	pCamera camera = pCamera(new Camera(camera_position, camera_lookat, M_PI / 2.0));
	Screen screen(camera, 2.0, Ratio(4, 3));

	Vector3d top_left, horizontal, vertical;
	top_left << -2.0, 1.5, 2.0;
	horizontal << 4.0, 0.0, 0.0;
	vertical << 0.0, -3.0, 0.0;

	EXPECT_TRUE(isEqual(top_left, screen.getTopLeftCorner()));
	EXPECT_TRUE(isEqual(horizontal, screen.getHorizontal()));
	EXPECT_TRUE(isEqual(vertical, screen.getVertical()));
}

#endif
