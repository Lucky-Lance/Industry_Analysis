#include "parser.h"
#include "graph.h"
#include "outputAPI.h"
#include "randomOutputAPI.h"
#include <filesystem>
#include "givenGroups.h"
void myGroup2(bool buildFromScratch){
    string name = __FUNCTION__;
    Hash h1 = Hash("d4e844046d099bbdd48536d0431618a45cde4f0c5593f027d8ceee11f8b9c211");
    Hash h2 = Hash("f1513192619f52c62bebdf52ec048bfc6261f792055a50b72267a64159fc5a12");
    Hash h3 = Hash("8170a48a4ca837cbfcfe6126afa36c9ed320dd4c0d7f9af7bd8755b0d97028cd");
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto linkName = rootPath.parent_path() / "Link.csv";
        auto nodeName = rootPath.parent_path() / "Node.csv";
        auto links = linkReader(linkName.string());
        auto nodes = nodeReader(nodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2, h3}, 5);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / (name+"-Node.csv")).string(),
                (outputPath / (name+"-Link.csv")).string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / (name+"-Link.csv");
        auto subNodeName = outputPath / (name+"-Node.csv");
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG

    for(auto maxNodes: vector<int>{300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800}){
        ofstream f;
        string fileName = name + "-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2, h3}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2, h3});
        f.close();
    }

}
int main(){
    DEBUG
    initPath();
    DEBUG
    cout << DPath << endl;
    myGroup2(1);
    return 0;
}