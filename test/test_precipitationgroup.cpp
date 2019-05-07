/*
* Copyright (C) 2018-2019 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"
#include "metaf.h"

const auto margin = 0.01 / 2;

TEST(PrecipitationGroup, parse4digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg1->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->amount().precipitation().has_value());
	EXPECT_NEAR(pg1->amount().precipitation().value(), 21.68, margin);

	const auto pg2 = metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg2->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->amount().precipitation().has_value());
	EXPECT_NEAR(pg2->amount().precipitation().value(), 2.17, margin);

	const auto pg3 = metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg3->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->amount().precipitation().has_value());
	EXPECT_NEAR(pg3->amount().precipitation().value(), 1.25, margin);
}

TEST(PrecipitationGroup, parse4digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("P////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::TOTAL_PRECIPITATION_HOURLY);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg1->amount().precipitation().has_value());

	const auto pg2 = metaf::PrecipitationGroup::parse("6////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_3_OR_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg2->amount().precipitation().has_value());

	const auto pg3 = metaf::PrecipitationGroup::parse("7////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::FROZEN_PRECIP_24_HOURLY);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg3->amount().precipitation().has_value());
}

TEST(PrecipitationGroup, parse3digitGroup) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg1->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg1->amount().precipitation().has_value());
	EXPECT_NEAR(pg1->amount().precipitation().value(), 21, margin);

	const auto pg2 = metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg2->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg2->amount().precipitation().has_value());
	EXPECT_NEAR(pg2->amount().precipitation().value(), 1.1, margin);

	const auto pg3 = metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::REPORTED);
	EXPECT_EQ(pg3->amount().unit(), metaf::Precipitation::Unit::INCHES);
	ASSERT_TRUE(pg3->amount().precipitation().has_value());
	EXPECT_NEAR(pg3->amount().precipitation().value(), 12.5, margin);	
}

TEST(PrecipitationGroup, parse3digitGroupNotReported) {
	const auto pg1 = metaf::PrecipitationGroup::parse("4////", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg1.has_value());
	EXPECT_EQ(pg1->type(), metaf::PrecipitationGroup::Type::SNOW_DEPTH_ON_GROUND);
	EXPECT_EQ(pg1->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg1->amount().precipitation().has_value());

	const auto pg2 = metaf::PrecipitationGroup::parse("931///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg2.has_value());
	EXPECT_EQ(pg2->type(), metaf::PrecipitationGroup::Type::SNOW_6_HOURLY);
	EXPECT_EQ(pg2->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg2->amount().precipitation().has_value());

	const auto pg3 = metaf::PrecipitationGroup::parse("933///", metaf::ReportPart::RMK);
	ASSERT_TRUE(pg3.has_value());
	EXPECT_EQ(pg3->type(), metaf::PrecipitationGroup::Type::WATER_EQUIV_OF_SNOW_ON_GROUND);
	EXPECT_EQ(pg3->amount().status(), metaf::Precipitation::Status::NOT_REPORTED);
	EXPECT_FALSE(pg3->amount().precipitation().has_value());	
}

TEST(PrecipitationGroup, parseWrongReportPart) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P2168", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("60217", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("70125", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/021", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931011", metaf::ReportPart::TAF).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::UNKNOWN).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::HEADER).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::METAR).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933125", metaf::ReportPart::TAF).has_value());
}

TEST(PrecipitationGroup, parseWrongFormat4digitGroup) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("X2168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("52168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("02168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("12168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("22168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("32168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("42168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("52168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("82168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("92168", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P02168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("PA168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P/////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("602168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6A168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6/////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("702168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7A168", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7216A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7/////", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, parseWrongFormatSnowDepthGroup) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("P/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("0/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("1/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("2/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("3/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("5/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("6/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("7/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("8/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("9/021", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4///", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("4/////", metaf::ReportPart::RMK).has_value());
}

TEST(PrecipitationGroup, parseWrongFormat93Group) {
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("93A125", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("930/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("932/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("935/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("936/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("937/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("938/021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("939/021", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("931////", metaf::ReportPart::RMK).has_value());

	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/0021", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/A21", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933/21A", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933//", metaf::ReportPart::RMK).has_value());
	EXPECT_FALSE(
		metaf::PrecipitationGroup::parse("933////", metaf::ReportPart::RMK).has_value());
}