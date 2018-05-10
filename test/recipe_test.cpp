/*
 * Created on 8/5/18
 * by Alexander Sorochynskyi
 */

#include "test_common.h"

#include <recipe.h>

using RecipeTest = RecipeTestBase;

TEST_F(RecipeTest, ToJson) {
	auto rj {r.toJson()};
}

TEST_F(RecipeTest, ToFromJson) {
	auto rj {r.toJson()};
	Recipe rec {rj};
}

