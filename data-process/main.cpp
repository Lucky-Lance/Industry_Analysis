#include "parser.h"
#include "graph.h"
#include "outputAPI.h"
#include "randomOutputAPI.h"
#include <filesystem>
auto rootPath = filesystem::path("/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/");
auto outputPath = rootPath / "output";
auto tmpPath = rootPath / "tmp";

void outputsubsubForFTC(){
    auto subLinkName = outputPath / "subsubbig-Link.csv";
    auto subNodeName = outputPath / "subsubbig-Node.csv";
    auto subLinks = linkReader(subLinkName);
    auto subNodes = nodeReader(subNodeName);
    Graph subGraph;
    subGraph.insert(subLinks, subNodes);
    auto subSubGraphNodes =subGraph.raw_node;
    auto subGraphBorderNodes = subGraph.getBorderNodes();
    for(const auto& h: subGraphBorderNodes){
        subSubGraphNodes.erase(h);
    }
    cout << "cut size = " << subSubGraphNodes.size() << endl;
    auto [subSubNodes, subSubLinks] = getSubGraph(subNodes, subLinks, subSubGraphNodes);
    outputGraph(subNodes, subLinks,
                tuple<string, string>(outputPath / "subsubsubbig-Node.csv", outputPath / "subsubsubbig-Link.csv"));
    ofstream f;
    f.open(outputPath/"subsubsubbig.json", ios::out);
    if(!f.good()){
        cerr << "open file '" << outputPath/"subsubsubbig.json" << "' error!" << endl;
        exit(1);
    }
    f << graphToJson_echarts(subSubLinks, subSubNodes);
    f.close();
}
int main(){
    outputsubsubForFTC();

    return 0;
}