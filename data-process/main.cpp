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
    // ftc 2022 05 29 18:00
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
void myGroup2(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
            Hash("a77b63d27d07fd9cc522afb93664f99d9f56f9edadf84e44ef4537748dc19141"),// Cert
            Hash("1b22e6e2c9f9d7afd041a1a0ef2178dbaaf3248c4261496a382ff46520d55e71"),// Cert
            Hash("62aa1e710d536e8e5d3eac2f47dc48c309982f040b98917e38afe85fedc9c516"),// IP
            Hash("f9b588fa3410ab89fa0e50b011c9ac8ddfa4a3125ea3df13fa4598faa5e15f8a"),// IP
            Hash("36b2ba5b0800d154ef3add5672b7561af9535edd92d2c3323c64880498b45a05"),// IP
    };
    vector<Hash> clues = {
            Hash("400c19e584976ff2a35950659d4d148a3d146f1b71692468132b849b0eb8702c"),// support by clue
            Hash("b10f98a9b53806ccd3a5ee45676c7c09366545c5b12aa96955cde3953e7ad058"),// support by clue
    };
    vector<Hash> exceptButInclude = {
        Hash("01c3543672af6b0070ead622b73d93619da9a124441a69c287ed0fcc941d62d7"), // Domain
        Hash("f6a5ffbe3609107c67362efaf5b804721398c40da76db27bc161aeef895e67bb"), // Domain
        Hash("bbe15474b14bdf9f8f8f401595e8a9c6285e844245946224bb11f58fc6196edf"), // cert
        Hash("92053475f3698b9e4b5ab8493a67702090c5a5c15fed4887913210b29205faba"), // ASN
    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.begin(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
            Hash("62aa1e710d536e8e5d3eac2f47dc48c309982f040b98917e38afe85fedc9c516"),// IP
            Hash("1b22e6e2c9f9d7afd041a1a0ef2178dbaaf3248c4261496a382ff46520d55e71"),// Cert
            Hash("36b2ba5b0800d154ef3add5672b7561af9535edd92d2c3323c64880498b45a05"),// IP
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
    int maxNodes = 370;// 150 is ok
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
    if(!pathByHand.empty())
    for(size_t i=0; i<pathByHand.size()-1; i++){
        auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
        path = Graph::concatPath(path, pathi);
    }
    {
        // cwd json
        ofstream f;
        string fileName = "cwd2.json";
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
    myGroup2(0);
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