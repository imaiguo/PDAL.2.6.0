/******************************************************************************
* Copyright (c) 2019, Hobu Inc. (info@hobu.co)
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
*     * Neither the name of Hobu, Inc. nor the
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

#include <pdal/StageFactory.hpp>
#include <pdal/private/Raster.hpp>
#include <pdal/private/gdal/Raster.hpp>

#include "Support.hpp"


namespace pdal
{
// NOTE: The test data has an accompanying jpg that depicts the points,
//  their triangulation and the interesting barycentric calculation.
TEST(FaceRasterTest, basic)
{
    Options ro;
    ro.add("filename", Support::datapath("filters/hagtest.txt"));

    StageFactory factory;
    Stage& r = *(factory.createStage("readers.text"));
    r.setOptions(ro);

    Options rngO;
    rngO.add("limits", "Classification[2:2]");
    Stage& rng = *(factory.createStage("filters.range"));
    rng.setOptions(rngO);
    rng.setInput(r);

    Stage& d = *(factory.createStage("filters.delaunay"));
    d.setInput(rng);

    Options fo;
    fo.add("resolution", 1);
    Stage& f = *(factory.createStage("filters.faceraster"));
    f.setInput(d);
    f.setOptions(fo);

    const std::string output_file = Support::temppath("test.tif");

    Options wo;
    wo.add("filename", output_file);
    Stage& w = *(factory.createStage("writers.raster"));
    w.setInput(f);
    w.setOptions(wo);

    PointTable t1;
    w.prepare(t1);
    PointViewSet s = w.execute(t1);
    PointViewPtr v = *s.begin();

    gdal::Raster raster(output_file, "GTiff");
    if (raster.open() != gdal::GDALError::None)
        throw pdal_error(raster.errorMsg());
    std::vector<double> data;
    raster.readBand(data, 1);
    int row = 0;
    int col = 0;

    const std::vector<double> expected
    {
        -9999,   10,      -9999,   -9999,   -9999, -9999,
        -9999,   9,       7,       -9999,   -9999, -9999,
        10,      8,       6,       4,       -9999, -9999,
        6.66667, 4.66667, 2.66667, 6,       7,     -9999,
        3.33333, 1.33333, 4.66667, 8,       9,     10,
        0,       3.33333, 6.66667, 10,      -9999, -9999
    };

    size_t size = raster.width() * raster.height();
    for (size_t i = 0; i < size; ++i)
        EXPECT_NEAR(expected[i], data[i], .00001);
}

TEST(FaceRasterTest, numerical_imprecision)
{
    // Test for edgecase when the pixel center is on a triangle border but
    // can be missed because of a numerical imprecision if no tolerance is applied
    Options ro;
    ro.add("filename", Support::datapath("filters/faceraster_numerical_imprecision.laz"));

    StageFactory factory;
    Stage& r = *(factory.createStage("readers.las"));
    r.setOptions(ro);


    Stage& d = *(factory.createStage("filters.delaunay"));
    d.setInput(r);

    Options fo;
    fo.add("resolution", 0.1);
    fo.add("height", 80);
    fo.add("width", 80);
    fo.add("origin_x", 687009.95);
    fo.add("origin_y", 6232990.05);
    Stage& f = *(factory.createStage("filters.faceraster"));
    f.setInput(d);
    f.setOptions(fo);

    const std::string output_file = Support::temppath("test.tif");

    Options wo;
    wo.add("filename", output_file);
    wo.add("gdaldriver", "GTiff");
    wo.add("nodata", -9999);
    Stage& w = *(factory.createStage("writers.raster"));
    w.setInput(f);
    w.setOptions(wo);

    PointTable t1;
    w.prepare(t1);
    w.execute(t1);

    gdal::Raster raster(output_file, "GTiff");
    if (raster.open() != gdal::GDALError::None)
        throw pdal_error(raster.errorMsg());
    std::vector<double> data;
    raster.readBand(data, 1);

    size_t size = raster.width() * raster.height();
    for (size_t i = 0; i < size; ++i)
        EXPECT_TRUE(data[i] > 0);
}

} // namespace pdal