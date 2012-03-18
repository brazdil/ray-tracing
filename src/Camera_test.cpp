/*
 * Camera_test.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifdef GTEST_TESTING

#include <iostream>
#include "Camera.h"
#include "extra_math.h"
#include "gtest/gtest.h"

/*
 * Check that exceptions are thrown when the camera is pointing straight up/down
 */
TEST(CameraTest, PointingUpDown) {
	Vector3d position, look_up, look_down;

	position << 0, 0, 0;
	look_up << 0, 1, 0;
	look_down << 0, -2, 0;

	ASSERT_THROW(Camera(position, look_up, M_PI / 3.0), std::invalid_argument);
	ASSERT_THROW(Camera(position, look_down, M_PI / 3.0), std::invalid_argument);

	position << 4, 3, 2;
	look_up << 4, 4, 2;
	look_down << 4, -2, 2;

	ASSERT_THROW(Camera(position, look_up, M_PI / 3.0), std::invalid_argument);
	ASSERT_THROW(Camera(position, look_down, M_PI / 3.0), std::invalid_argument);
}

TEST(CameraTest, IllegalArgument) {
	Vector3d position, look_at;
	position << 0, 0, 0;
	look_at << 0, 0, 1;

	ASSERT_NO_THROW(Camera(position, look_at, M_PI / 3.0));

	ASSERT_THROW(Camera(position, look_at, 0.0), std::invalid_argument);
	ASSERT_THROW(Camera(position, look_at, M_PI), std::invalid_argument);
	ASSERT_THROW(Camera(position, look_at, M_PI + 0.1), std::invalid_argument);
	ASSERT_THROW(Camera(position, look_at, -0.1), std::invalid_argument);

}

/*
 * This test checks that the rotation into yz plane works fine
 */
TEST(CameraTest, Horizontal) {
	Vector3d position, look_at, up, side, direction;
	Camera camera;
	double OneOverSq2 = 1.0 / sqrt(2.0);

	// 0 degrees

	position << 1, 1, 1;
	look_at << 3, 1, 1;
	up << 0, 1, 0;
	direction << 1, 0, 0;
	side << 0, 0, -1;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 45 degrees

	position << 1, 1, 1;
	look_at << 2, 1, 2;
	up << 0, 1, 0;
	direction << OneOverSq2, 0, OneOverSq2;
	side << OneOverSq2, 0, -OneOverSq2;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 90 degrees

	position << 1, 1, 1;
	look_at << 1, 1, 3;
	up << 0, 1, 0;
	direction << 0, 0, 1;
	side << 1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 135 degrees

	position << 1, 1, 1;
	look_at << 0, 1, 2;
	up << 0, 1, 0;
	direction << -OneOverSq2, 0, OneOverSq2;
	side << OneOverSq2, 0, OneOverSq2;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 180 degrees

	position << 1, 1, 1;
	look_at << -1, 1, 1;
	up << 0, 1, 0;
	direction << -1, 0, 0;
	side << 0, 0, 1;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 225 degrees

	position << 1, 1, 1;
	look_at << 2, 1, 0;
	up << 0, 1, 0;
	direction << OneOverSq2, 0, -OneOverSq2;
	side << -OneOverSq2, 0, -OneOverSq2;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 270 degrees

	position << 1, 1, 1;
	look_at << 1, 1, -1;
	up << 0, 1, 0;
	direction << 0, 0, -1;
	side << -1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// 315 degrees

	position << 1, 1, 1;
	look_at << 0, 1, 0;
	up << 0, 1, 0;
	direction << -OneOverSq2, 0, -OneOverSq2;
	side << -OneOverSq2, 0, OneOverSq2;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));
}

/*
 * Test the rotation inside yz plane
 */
TEST(CameraTest, Vertical) {
	Vector3d position, look_at, up, side, direction;
	Camera camera;
	double OneOverSq2 = 1.0 / sqrt(2.0);

	// positive Z

	position << 1, 1, 1;
	look_at << 1, 2, 2;
	up << 0, OneOverSq2, -OneOverSq2;
	direction << 0, OneOverSq2, OneOverSq2;
	side << 1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	position << 1, 1, 1;
	look_at << 1, 1, 2;
	up << 0, 1, 0;
	direction << 0, 0, 1;
	side << 1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	position << 1, 1, 1;
	look_at << 1, 0, 2;
	up << 0, OneOverSq2, OneOverSq2;
	direction << 0, -OneOverSq2, OneOverSq2;
	side << 1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	// negative Z

	position << 1, 1, 1;
	look_at << 1, 2, 0;
	up << 0, OneOverSq2, OneOverSq2;
	direction << 0, OneOverSq2, -OneOverSq2;
	side << -1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	position << 1, 1, 1;
	look_at << 1, 1, 0;
	up << 0, 1, 0;
	direction << 0, 0, -1;
	side << -1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));

	position << 1, 1, 1;
	look_at << 1, 0, 0;
	up << 0, OneOverSq2, -OneOverSq2;
	direction << 0, -OneOverSq2, -OneOverSq2;
	side << -1, 0, 0;
	camera = Camera(position, look_at, M_PI / 3.0);
	EXPECT_TRUE(isEqual(up, camera.getUp()));
	EXPECT_TRUE(isEqual(direction, camera.getDirection()));
	EXPECT_TRUE(isEqual(side, camera.getSideways()));
}

#endif
