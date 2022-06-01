#ifndef _PATH_H_
#define _PATH_H_
#include <filesystem>
#include <cassert>
using namespace std;
filesystem::path DPath, rootPath, outputPath, tmpPath, outputAPIPath;
filesystem::path sourceLinkName, sourceNodeName;
void initPath(){
#if defined(WIN32)
    DPath = filesystem::path("D:/");
#elif defined(linux)
    auto DPath1 = filesystem::path("/media/ftc/DATA");
    auto DPath2 = filesystem::path("/mnt/d");
    bool isPath1 = filesystem::is_directory(DPath1);
    bool isPath2 = filesystem::is_directory(DPath2);
    assert(isPath1 ^ isPath2);
    if(isPath1) DPath = DPath1;
    else DPath = DPath2;
#endif
    rootPath = DPath / ("ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process");
    outputPath = rootPath / "output";
    outputAPIPath = rootPath / "outputAPI";
    tmpPath = rootPath / "tmp";
    cout << "rootPath = " << rootPath << endl;
    cout << "tmpPath = " << tmpPath << endl;
    sourceLinkName = rootPath.parent_path() / "Link.csv";
    sourceNodeName = rootPath.parent_path() / "Node.csv";
}
#endif