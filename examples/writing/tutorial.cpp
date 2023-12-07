#include <pdal/PointView.hpp>
#include <pdal/PointTable.hpp>
#include <pdal/Dimension.hpp>
#include <pdal/Options.hpp>
#include <pdal/StageFactory.hpp>

#include <io/BufferReader.hpp>

#include <vector>

struct Point{
    double x;
    double y;
    double z;
};

void fillView(pdal::PointViewPtr view){

    for (int i = 0; i < 1000; ++i){

        Point p;

        p.x = -93.0 + i*0.001;
        p.y = 42.0 + i*0.001;
        p.z = 106.0 + i;

        view->setField(pdal::Dimension::Id::X, i, p.x);
        view->setField(pdal::Dimension::Id::Y, i, p.y);
        view->setField(pdal::Dimension::Id::Z, i, p.z);
    }
}


// PointTable -> PointViewPtr -> BufferReader ->  writers.las [StageFactory -> Stage] -> execute

int main(int argc, char* argv[]){

    pdal::Options options;
    options.add("filename", "myfile.las");

    pdal::PointTable table;
    table.layout()->registerDim(pdal::Dimension::Id::X);
    table.layout()->registerDim(pdal::Dimension::Id::Y);
    table.layout()->registerDim(pdal::Dimension::Id::Z);

    pdal::PointViewPtr view(new pdal::PointView(table));

    fillView(view);

    pdal::BufferReader reader;
    reader.addView(view);

    pdal::StageFactory factory;

    // StageFactory always "owns" stages it creates. They'll be destroyed with the factory.
    pdal::Stage *writer = factory.createStage("writers.las");

    writer->setInput(reader);
    writer->setOptions(options);
    writer->prepare(table);
    writer->execute(table);

    return 0;
}
