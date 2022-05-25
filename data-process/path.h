#ifndef _PATH_H_
#define _PATH_H_
#include <filesystem>
using namespace std;
filesystem::path DPath, rootPath, outputPath, tmpPath, outputAPIPath;

void initPath(){
#if defined(WIN32)
    DPath = filesystem::path("D:");
#elif defined(linux)
    DPath = filesystem::path("/media/ftc/DATA");
//    DPath = filesystem::path("/mnt/d");
#endif
    rootPath = DPath / ("ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process");
    outputPath = rootPath / "output";
    outputAPIPath = rootPath / "outputAPI";
    tmpPath = rootPath / "tmp";
    cout << "rootPath = " << rootPath << endl;
    cout << "tmpPath = " << tmpPath << endl;

}
#endif