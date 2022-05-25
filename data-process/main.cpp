#include "parser.h"
#include "graph.h"
#include "outputAPI.h"
#include "randomOutputAPI.h"
#include <filesystem>
#include "givenGroups.h"
void finalGroup2(){
    string name = __FUNCTION__;
    vector<Hash> centers = {
            Hash("fe794a69eacd63b21245bf4eda826222fc6c5862bebf77aa05459cb308cfd063"),
            Hash("c58c149eec59bb14b0c102a0f303d4c20366926b5c3206555d2937474124beb9"),
            Hash("f3554b666038baffa5814c319d3053ee2c2eb30d31d0ef509a1a463386b69845"),
    };
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    auto linkName = rootPath.parent_path() / "Link.csv";
    auto nodeName = rootPath.parent_path() / "Node.csv";
    auto links = linkReader(linkName.string());
    auto nodes = nodeReader(nodeName.string());
    Graph graph;
    graph.insert(links, nodes);
    auto subGraphVec = graph.divideSubGraphByDepth(centers, 5);
    tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
    cout << "node size = " << subNodes.size() << endl;
    {// output csv and echarts json
        Graph subGraph;
        subGraph.insert(subLinks, subNodes);
        ofstream f;
        int maxNodes = 800;
        string fileName = name + ".json";
        f.open(outputPath / fileName, ios::out);
        if (!f.good()) {
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        outputGraph(group1Nodes, group1Links, tuple<string, string>(
                (outputPath / (name + "-Node.csv")).string(),
                (outputPath / (name + "-Link.csv")).string()
        ));
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>(centers.cbegin(), centers.cend()));
        f.close();
    }
    {
        Graph finalGraph2;
        {
            auto nodes = nodeReader((outputPath / (name+"-Node.csv")).string());
            auto links = linkReader((outputPath / (name+"-Link.csv")).string());
            finalGraph2.insert(links, nodes);
        }
        auto degreeStatistics = finalGraph2.getStatistics(vector<Hash>(centers.cbegin(), centers.cend()));
        ofstream f;
        f.open(outputAPIPath/(name+"-lxd.json"), ios::out);
        f << graphToJson_lxd(degreeStatistics);
        f.close();
    }
}
void finalGroup1(){
    string name = __FUNCTION__;
    Hash h1 = Hash("421753e1f9d50442ad058315e2ab1ba76104ebc39764039f788257df5dc643d4");
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    auto linkName = rootPath.parent_path() / "Link.csv";
    auto nodeName = rootPath.parent_path() / "Node.csv";
    auto links = linkReader(linkName.string());
    auto nodes = nodeReader(nodeName.string());
    Graph graph;
    graph.insert(links, nodes);
    auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1}, 5);
    tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
    cout << "node size = " << subNodes.size() << endl;
    {
        Graph subGraph;
        subGraph.insert(subLinks, subNodes);
        ofstream f;
        uint32_t maxNodes = 400;
        string fileName = name + ".json";
        f.open(outputPath / fileName, ios::out);
        if (!f.good()) {
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        outputGraph(group1Nodes, group1Links, tuple<string, string>(
                (outputPath / (name + "-Node.csv")).string(),
                (outputPath / (name + "-Link.csv")).string()
        ));
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1});
        f.close();
    }
    {
        Graph finalGraph1;
        {
            auto nodes = nodeReader((outputPath / (name+"-Node.csv")).string());
            auto links = linkReader((outputPath / (name+"-Link.csv")).string());
            finalGraph1.insert(links, nodes);
        }
        auto degreeStatistics = finalGraph1.getStatistics(vector<Hash>{h1});
        ofstream f;
        f.open(outputAPIPath/(name+"-lxd.json"), ios::out);
        f << graphToJson_lxd(degreeStatistics);
        f.close();
    }
}
void myGroup1(bool buildFromScratch){
    string name = __FUNCTION__;
    Hash h1 = Hash("421753e1f9d50442ad058315e2ab1ba76104ebc39764039f788257df5dc643d4");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1}, 5);
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
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1});
        f.close();
    }

}
int main(){
    initPath();
    finalGroup2();
//    auto outputStatistics = [](const array<uint32_t, 8>& statistic)->string{
//        return string("count: ") + to_string(statistic[0])+
//        string(", Domain: ") + to_string(statistic[1])+
//        string(", IP: ") + to_string(statistic[2])+
//        string(", Cert: ") + to_string(statistic[3])+
//        string(", Other: ") + to_string(statistic[4])+
//        string(", Top: ") + to_string(statistic[5])+
//        string(", Mid: ") + to_string(statistic[6])+
//        string(", Bot: ") + to_string(statistic[7]);
//    };
//    for(uint32_t degree = 0; degree < degreeStatistics.size(); degree++){
//        cout << "degree = " << degree << ", " << outputStatistics(degreeStatistics[degree]) << endl;
//    }
    return 0;
}