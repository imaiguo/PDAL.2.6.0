/******************************************************************************
* Copyright (c) 2016, Howard Butler <howard@hobu.co>
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/pdal_test_main.hpp>

#include <pdal/PointView.hpp>
#include <pdal/Options.hpp>
#include <pdal/Polygon.hpp>
#include <pdal/util/Algorithm.hpp>
#include <pdal/util/FileUtils.hpp>

#include "Support.hpp"

namespace pdal
{

std::string getWKT()
{
    return FileUtils::readFileIntoString(
        Support::datapath("autzen/autzen-selection.wkt"));
}

std::string getJSON()
{
    return FileUtils::readFileIntoString(
        Support::datapath("autzen/autzen-selection.json"));
}

TEST(PolygonTest, test_wkt_in)
{
    std::string wkt(getWKT());

    Polygon p(wkt);
}

TEST(PolygonTest, test_json_in)
{
    std::string json(getJSON());

    Polygon p(json);
}

TEST(PolygonTest, test_wkt_out)
{

    Polygon p(getWKT());
    std::string expected("POLYGON ((636889.412951239 851528.512293259 422.7001953125,636899.142334239 851475.000686757 422.4697265625,636899.142334239 851475.000686757 422.4697265625,636928.33048324 851494.459452758 422.5400390625,636928.33048324 851494.459452758 422.5400390625,636928.33048324 851494.459452758 422.5400390625,636976.977398242 851513.918218758 424.150390625,636976.977398242 851513.918218758 424.150390625,637069.406536744 851475.000686757 438.7099609375,637132.647526246 851445.812537756 425.9501953125,637132.647526246 851445.812537756 425.9501953125,637336.964569251 851411.759697255 425.8203125,637336.964569251 851411.759697255 425.8203125,637473.175931255 851158.795739249 435.6298828125,637589.928527258 850711.244121237 420.509765625,637244.535430749 850511.791769731 420.7998046875,636758.066280736 850667.461897735 434.609375,636539.15516323 851056.637217746 422.6396484375,636889.412951239 851528.512293259 422.7001953125))");

    std::string w = p.wkt();
    EXPECT_EQ(expected, w);

}

TEST(PolygonTest, simplify)
{
    std::string s =

    R"( MULTIPOLYGON ( ( (0 0, 0 6, 6 6, 6 1, 6 .9, 6 .8, 6 .7, 6 .5, 6 0, 0 0), (.5 1, 1.5 1, 1 2, .5 1), (.5 3, 1.5 3, 1 4, .5 3), (2 1, 5 1, 5 4, 2 4, 2 1)), ( (3 2, 3 3, 4 3, 4 2, 3 2), (3.2 2.2, 3.7 2.2, 3.7 2.7, 3.2 2.7, 3.2 2.2))))";
//GDAL doesn't currently handle newlines in WKT
/**
    R"( MULTIPOLYGON ( ( (0 0, 6 0, 6 .5, 6 .7, 6 .8, 6 .9, 6 1, 6 6, 0 6, 0 0), (.5 1, 1.5 1, 1 2, .5 1), (.5 3, 1.5 3, 1 4, .5 3), (2 1, 5 1, 5 4, 2 4, 2 1)), ( (3 2, 4 2, 4 3, 3 3, 3 2), (3.2 2.2, 3.7 2.2, 3.7 2.7, 3.2 2.7, 3.2 2.2))))";
      MULTIPOLYGON (
        (
          (0 0, 6 0, 6 .5, 6 .7, 6 .8, 6 .9, 6 1, 6 6, 0 6, 0 0),
          (0 0, 0 6, 6 6, 6 1, 6 .9, 6 .8, 6 .7, 6 .5, 6 0, 0 0),
            (.5 1, 1.5 1, 1 2, .5 1),
            (.5 3, 1.5 3, 1 4, .5 3),
            (2 1, 5 1, 5 4, 2 4, 2 1)
        ),
        (
          (3 2, 3 3, 4 3, 4 2, 3 2),
            (3.2 2.2, 3.7 2.2, 3.7 2.7, 3.2 2.7, 3.2 2.2)
        )
      )
**/
    Polygon p(s);

    p.simplify(1, 1, true);
    s = p.wkt();
    Utils::remove(s, ' ');
    EXPECT_EQ(s, "MULTIPOLYGON(((00,06,66,60,00),(21,51,54,24,21)))");
}

TEST(PolygonTest, test_json_out)
{
    Polygon p(getJSON());
    std::string j = p.json(5);
    std::string expected("{ \"type\": \"Polygon\", \"coordinates\": [ [ [ 636889.41295, 851528.51229, 422.7002 ], [ 636899.14233, 851475.00069, 422.46973 ], [ 636899.14233, 851475.00069, 422.46973 ], [ 636928.33048, 851494.45945, 422.54004 ], [ 636928.33048, 851494.45945, 422.54004 ], [ 636928.33048, 851494.45945, 422.54004 ], [ 636976.9774, 851513.91822, 424.15039 ], [ 636976.9774, 851513.91822, 424.15039 ], [ 637069.40654, 851475.00069, 438.70996 ], [ 637132.64753, 851445.81254, 425.9502 ], [ 637132.64753, 851445.81254, 425.9502 ], [ 637336.96457, 851411.7597, 425.82031 ], [ 637336.96457, 851411.7597, 425.82031 ], [ 637473.17593, 851158.79574, 435.62988 ], [ 637589.92853, 850711.24412, 420.50977 ], [ 637244.53543, 850511.79177, 420.7998 ], [ 636758.06628, 850667.4619, 434.60938 ], [ 636539.15516, 851056.63722, 422.63965 ], [ 636889.41295, 851528.51229, 422.7002 ] ] ] }");
    EXPECT_EQ(j, expected);
}

TEST(PolygonTest, smooth)
{
    std::string circle("POLYGON ((3.1415926535897 0, 3.1264650262780473 -0.3079299279852183, 3.0812278316637025 -0.6128943224323173, 3.0063167286599684 -0.9119562095122759, 2.9024531521393446 -1.2022354597686569, 2.770637365121332 -1.4809365253387112, 2.6121388256927522 -1.7453753626075, 2.4284839614324176 -1.9930052810151515, 2.2214414690791173 -2.221441469079117, 1.9930052810151515 -2.4284839614324176, 1.7453753626075001 -2.6121388256927522, 1.480936525338712 -2.7706373651213316, 1.202235459768657 -2.9024531521393446, 0.9119562095122759 -3.0063167286599684, 0.6128943224323177 -3.0812278316637025, 0.3079299279852188 -3.126465026278047, 0.0000000000000002 -3.1415926535897, -0.3079299279852184 -3.1264650262780473, -0.6128943224323172 -3.0812278316637025, -0.9119562095122754 -3.0063167286599684, -1.2022354597686566 -2.9024531521393446, -1.4809365253387115 -2.770637365121332, -1.7453753626074993 -2.6121388256927522, -1.9930052810151513 -2.428483961432418, -2.221441469079117 -2.2214414690791173, -2.4284839614324176 -1.9930052810151515, -2.6121388256927522 -1.7453753626075, -2.7706373651213316 -1.4809365253387121, -2.9024531521393446 -1.202235459768657, -3.0063167286599684 -0.9119562095122761, -3.0812278316637025 -0.6128943224323186, -3.126465026278047 -0.307929927985219, -3.1415926535897 -0.0000000000000004, -3.1264650262780473 0.3079299279852182, -3.0812278316637025 0.6128943224323178, -3.0063167286599684 0.9119562095122752, -2.902453152139345 1.2022354597686564, -2.770637365121332 1.4809365253387112, -2.6121388256927527 1.7453753626074993, -2.428483961432418 1.9930052810151508, -2.2214414690791178 2.221441469079117, -1.9930052810151526 2.4284839614324167, -1.7453753626075 2.6121388256927522, -1.4809365253387121 2.7706373651213316, -1.2022354597686586 2.902453152139344, -0.9119562095122763 3.0063167286599684, -0.6128943224323187 3.081227831663702, -0.3079299279852178 3.1264650262780473, -0.0000000000000006 3.1415926535897, 0.3079299279852167 3.1264650262780473, 0.6128943224323176 3.0812278316637025, 0.9119562095122751 3.0063167286599684, 1.2022354597686575 2.902453152139344, 1.4809365253387112 2.770637365121332, 1.7453753626074988 2.6121388256927527, 1.993005281015152 2.4284839614324176, 2.2214414690791164 2.2214414690791178, 2.4284839614324167 1.9930052810151528, 2.6121388256927522 1.7453753626075, 2.770637365121331 1.4809365253387123, 2.902453152139344 1.2022354597686589, 3.0063167286599684 0.9119562095122764, 3.081227831663702 0.6128943224323189, 3.1264650262780473 0.307929927985218, 3.1415926535897 0))");
    Polygon p(circle);

    p.simplify(0.25, 0.0);

    EXPECT_FLOAT_EQ(static_cast<float>(p.area()), 27.915457f);

}



TEST(PolygonTest, covers)
{
    using namespace pdal::Dimension;

    pdal::Polygon p(getWKT());

    PointTable table;
    PointLayoutPtr layout(table.layout());
    layout->registerDim(Dimension::Id::X);
    layout->registerDim(Dimension::Id::Y);


    // Check if the very first point in the polygon is
    // covered by the polyg. Should be true
    //
    PointViewPtr view(new PointView(table));
    view->setField(Id::X, 0, 636889.412951239268295 );
    view->setField(Id::Y, 0, 851528.512293258565478);

    PointRef ref(*view, 0);
    bool covered = p.covers(ref);
    EXPECT_EQ(covered, true);
}

TEST(PolygonTest, valid)
{
    pdal::Polygon p(getWKT());
    EXPECT_EQ(p.valid(), true);
}

TEST(PolygonTest, bounds)
{
    pdal::Polygon p(getWKT());
    BOX3D b = p.bounds();
    EXPECT_NEAR(b.minx, 636539.1, .1);
    EXPECT_NEAR(b.miny, 850511.8, .1);
    EXPECT_NEAR(b.minz, 420.50977, .00001);
    EXPECT_NEAR(b.maxx, 637589.9, .1);
    EXPECT_NEAR(b.maxy, 851528.5, .1);
    EXPECT_NEAR(b.maxz, 438.70996, .00001);
}

TEST(PolygonTest, bounds2d)
{
    BOX2D box(636539.12, 850511.81, 637589.94, 851528.5);
    pdal::Polygon p(box);

    BOX3D b = p.bounds();
    EXPECT_NEAR(b.minx, 636539.12, .01);
    EXPECT_NEAR(b.miny, 850511.81, .01);
    EXPECT_NEAR(b.minz, 0.0, .1);
    EXPECT_NEAR(b.maxx, 637589.94, .01);
    EXPECT_NEAR(b.maxy, 851528.5, .1);
    EXPECT_NEAR(b.maxz, 0.0, .1);
}

TEST(PolygonTest, bounds3d)
{
    BOX3D box(636539.12, 850511.81, 420.50977, 637589.94, 851528.5, 438.70996);
    pdal::Polygon p(box);

    BOX3D b = p.bounds();
    EXPECT_NEAR(b.minx, 636539.12, .01);
    EXPECT_NEAR(b.miny, 850511.81, .01);
    EXPECT_NEAR(b.minz, 420.5097, .0001);
    EXPECT_NEAR(b.maxx, 637589.94, .01);
    EXPECT_NEAR(b.maxy, 851528.5, .1);
    EXPECT_NEAR(b.maxz, 438.70996, .00001);
}


TEST(PolygonTest, streams)
{
    pdal::Polygon p(getWKT());

    std::stringstream ss(std::stringstream::in | std::stringstream::out);

    ss << p;

    pdal::Polygon p2;
    ss >> p2;

    EXPECT_TRUE(p == p2);

}

} // namespace pdal
