#include "parser.h"
#include "graph.h"
#include "outputJson.h"
#include <filesystem>
void divideGraph(){
    string linkName = "./Link.csv";
    string nodeName = "./Node.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    vector<vector<Hash>> subGraphs = graph.divideSubGraph();
    outputSubGraph(nodes, links, subGraphs);
}
void analyseTheBiggest(){
    string linkName = "./output/00000055-Link-00172977.csv";
    string nodeName = "./output/00000055-Node-00172977.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    graph.analyseDegree();
}
int main(){
    auto outputFolder = filesystem::path("./output/");
    if(!filesystem::is_directory(outputFolder)){
        if(filesystem::create_directory(outputFolder)){
            divideGraph();
        }
        else{
            cerr << "try to mkdir './output', but failed!" << endl;
            exit(1);
        }
    }
    DEBUG
    analyseTheBiggest();
    return 0;
}