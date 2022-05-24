#include "parser.h"
#include "graph.h"
#include "outputAPI.h"
#include "randomOutputAPI.h"
using namespace std;
void divideGraph(){
    string linkName = "./Link.csv";
    string nodeName = "./Node.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    vector<vector<Hash>> subGraphs = graph.divideSubGraph();
    outputSubGraphs(nodes, links, subGraphs, "./output/");
}
void analyseTheBiggest(){
    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    vector<vector<Hash>> subGraphs = graph.bfsAnalyseGraphs();
//    tuple<string, string> names = std::make_tuple(
//            "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/sub01-nodes.csv",
//            "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/sub01-link.csv"
//            );
    outputSubGraphs(nodes, links, subGraphs, "./output/");
}
void outputForEcharts(){
    string linkName = "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/output/subsubsubbig-Link.csv";
    string nodeName = "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/output/subsubsubbig-Node.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    clog << "nodesize = " << nodes.size() << endl;
    ofstream f; f.open("/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/subbig.json", ios::out);
    f << graphToJson_echarts(links, nodes) << endl;
    f.close();
}
void outputForCWH(){
    ofstream f;
    f.open("./outputAPI/cwh.json", ios::out);
//    f.open("./cwh.json", ios::out);
    if(!f.good()){
        cerr << "try to open './outputAPI/cwh.json' but failed!" << endl;
        exit(1);
    }
    f << random_graphToJson_cwh(300);
    f.close();
}
void outputsubsubForFTC(){
    string subLinkName = "./output/subsubbig-Link.csv";
    string subNodeName = "./output/subsubbig-Node.csv";
    auto subLinks = linkReader(subLinkName);
    auto subNodes = nodeReader(subNodeName);
    Graph subGraph;
    subGraph.insert(subLinks, subNodes);
    auto subSubGraphNodes =subGraph.raw_node;
    auto subGraphBorderNodes = subGraph.getBorderNodes();
    for(const auto& h: subGraphBorderNodes){
        subSubGraphNodes.erase(h);
    }
    auto [subSubNodes, subSubLinks] = getSubGraph(subNodes, subLinks, subSubGraphNodes);
    outputGraph(subNodes, subLinks, tuple<string, string>("./output/subsubsubbig-Node.csv", "./output/subsubsubbig-Link.csv"));
}
void outputForFTC(){
    DEBUG
//    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
//    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
//    vector<LinkItemType> links = linkReader(linkName);
//    map<Hash, ItemType> nodes  = nodeReader(nodeName);
//    Graph graph;
//    graph.insert(links, nodes);
//    auto subGraphHash = graph.bfsAnalyseGraph();
//    auto [subNodes, subLinks] = getSubGraph(nodes, links, subGraphHash);
    string subLinkName = "./output/subbig-Link.csv";
    string subNodeName = "./output/subbig-Node.csv";
    auto subLinks = linkReader(subLinkName);
    auto subNodes = nodeReader(subNodeName);
    Graph subGraph;
    subGraph.insert(subLinks, subNodes);
    auto subGraphBorderNodes = subGraph.getBorderNodes();
    auto subGraphRawNodes = subGraph.raw_node;
    for(const auto& h: subGraphBorderNodes){
        subGraphRawNodes.erase(h);
    }
    auto [subSubNodes, subSubLinks] = getSubGraph(subNodes, subLinks, subGraphRawNodes);
    outputGraph(subNodes, subLinks, tuple<string, string>("./output/subsubbig-Node.csv", "./output/subsubbig-Link.csv"));
}