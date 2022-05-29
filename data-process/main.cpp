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
    auto links = linkReader(sourceLinkName.string());
    auto nodes = nodeReader(sourceNodeName.string());
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
        auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight,bool hasBlack,uint32_t depth)->int{
            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        });
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
    auto links = linkReader(sourceLinkName.string());
    auto nodes = nodeReader(sourceNodeName.string());
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
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1}, maxNodes,[](uint32_t edgeWeight,bool hasBlack,uint32_t depth)->int{
            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        });
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
    vector<Hash> centers = {
            Hash("5a3d16b7df3d815d5f3436bd5dd5c5e1054ee7cb74d4fd8d9efdf3af362a4a18"),// who is email
            Hash("f6974ce3fa84ae76d75b9211f3162155db77566a36c82549b66a9a3d966a928b"),// who is phone
            Hash("db0925a5aeb1849fa7b41f7a29c1192d38e12e97fb6e82e72e894e3c733130ef"),// who is name

    };
    vector<Hash> clues = {
            Hash("f3554b666038baffa5814c319d3053ee2c2eb30d31d0ef509a1a463386b69845"),// support by clue
            Hash("c58c149eec59bb14b0c102a0f303d4c20366926b5c3206555d2937474124beb9"),// support by clue
    };
    vector<Hash> exceptButInclude = {
            Hash("fe794a69eacd63b21245bf4eda826222fc6c5862bebf77aa05459cb308cfd063"),// cert fe794
            Hash("110fb52a3afd8dbd4d2c36688306166f216e5a19362ae554addf7677ee568aea"),

    };
    vector<Hash> excepts = {
            Hash("3acb98d8043248d884b0236acff6e75a199b89dff521dcc142d59df236c7992e"),// asn
            Hash("1d624c9be374e9b85d0105f3fc5e474a305c294f11402195edd43d093b9d4ab9"),// asn
    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.begin(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
            Hash("fe794a69eacd63b21245bf4eda826222fc6c5862bebf77aa05459cb308cfd063"),// cert fe794
            Hash("c58c149eec59bb14b0c102a0f303d4c20366926b5c3206555d2937474124beb9"),// clue domain
            Hash("cee489b4e75d3b79f86fc13516688cb4ed6a84e39f139f25014bf97c0d7bfbf1"),
            Hash("f3554b666038baffa5814c319d3053ee2c2eb30d31d0ef509a1a463386b69845"),// support by clue
            Hash("392d981eaf712a3ecb8553b3b90974d538e484bad7ccff19f6ef89d1b6456226"),// cert
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 3);
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
    int maxNodes = 150;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
    Graph group1Graph;
    group1Graph.insert(group1Links, group1Nodes);
    vector<Hash> path;
    for(size_t i=0; i<pathByHand.size()-1; i++){
        auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
        path = Graph::concatPath(path, pathi);
    }

    {
        // cwd json
        ofstream f;
        string fileName = "cwd1.json";
        f.open(outputAPIPath/fileName, ios::out);
        if(!f.good()){
            DEBUG
            cerr << "open file '" << outputAPIPath/fileName << "' error!" << endl;
            exit(1);
        }
        cout << "output to '" << outputAPIPath/fileName << "'" << endl;

        f << graphToJson_cwd(group1Links, group1Nodes,
            vector<vector<Hash>>{path},
            group1Graph.getNodeStatistics(centers)
                );
    }
    {// echarts
            ofstream f;
            string fileName = name + "-" + to_string(maxNodes) + ".json";
            f.open(outputPath / fileName, ios::out);
            if (!f.good()) {
                cerr << "open file error!" << endl;
                DEBUG;
                exit(1);
            }
            cout << "output to '" << outputPath / fileName << "'." << endl;
            f << graphToJson_echarts(group1Links, group1Nodes,
                                     set<Hash>(centers.cbegin(), centers.cend()),
                                     set<Hash>(exceptAll.cbegin(), exceptAll.cend()),
                                     set<Hash>(clues.cbegin(), clues.cend()),
                                     set<Hash>(path.cbegin(), path.cend()));
            f.close();
    }
}
int main(){
    initPath();
    myGroup1(0);
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