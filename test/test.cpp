/*
 * Created by Alexander Sorochynskyi
 * on 12/04/18
 */


#include <gtest/gtest.h>
/*// I beleive this isn't necessary
#include "recipe_test.cpp"
#include "storage_test.cpp"
*/

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

