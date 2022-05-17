#include "parser.h"
#include "graph.h"
#include "outputAPI.h"
#include "randomOutputAPI.h"
#include <filesystem>
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
void subGraphToJsonForAnalyse(){
//    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
//    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
    string linkName = "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/sub01-link.csv";
    string nodeName = "/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/sub01-nodes.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
//    ofstream f; f.open("subgraph55.json", ios::out);
    ofstream f; f.open("/media/ftc/DATA/ftc/第六学期/可视化/作业/大作业/ChinaVis Data Challenge 2022-mini challenge 1-Dataset/data-process/subgraph01.json", ios::out);
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
void outputForFTC(){
    DEBUG
    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    auto _ = graph.bfsAnalyseGraph();
}
int main(){
//    auto outputFolder = filesystem::path("/media/ftc/DATA/output/");
//    if(!filesystem::is_directory(outputFolder)){
//        if(filesystem::create_directory(outputFolder)){
//            divideGraph();
//        }
//        else{
//            cerr << "try to mkdir '" << outputFolder << "', but failed!" << endl;
//            exit(1);
//        }
//    }
//    analyseTheBiggest();
//    subGraphToJsonForAnalyse();
    outputForFTC();
    return 0;
}