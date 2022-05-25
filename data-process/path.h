#ifndef _PATH_H_
#define _PATH_H_
#include <filesystem>
using namespace std;
filesystem::path DPath, rootPath, outputPath, tmpPath;

void initPath(){
#if defined(WIN32)
    DPath = filesystem::path("D:");
#elif defined(linux)
//    DPath = filesystem::path("/media/ftc/DATA");
    DPath = filesystem::path("/mnt/d");
#endif
    rootPath = DPath / ("ftc/����ѧ��/���ӻ�/��ҵ/����ҵ/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process");
    outputPath = rootPath / "output";
    tmpPath = rootPath / "tmp";
    cout << "rootPath = " << rootPath << endl;
    cout << "tmpPath = " << tmpPath << endl;

}
#endif