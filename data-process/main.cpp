#include "parser.h"
#include "graph.h"
#include <filesystem>
#include <json/json.h>
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
    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    graph.analyseDegree();
}
void subGraphToJsonForAnalyse(){
    string linkName = "/media/ftc/DATA/output/00000014-Link-00000044.csv";
    string nodeName = "/media/ftc/DATA/output/00000014-Node-00000044.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    ofstream f; f.open("subgraph14.json", ios::out);
    f << graphToJson(links, nodes) << endl;
    f.close();
}
int main(){
    auto outputFolder = filesystem::path("/media/ftc/DATA/output/");
    if(!filesystem::is_directory(outputFolder)){
        DEBUG
        if(filesystem::create_directory(outputFolder)){
            DEBUG
            divideGraph();
        }
        else{
            cerr << "try to mkdir './output', but failed!" << endl;
            exit(1);
        }
    }
    DEBUG
//    analyseTheBiggest();
    subGraphToJsonForAnalyse();
    return 0;
}