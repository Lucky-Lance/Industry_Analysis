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
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
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
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
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
void myGroup3(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
            Hash("9c72287c3f9bb38cb0186acf37b7054442b75ac32324dfd245aed46a03026de1"),// support by clue
            Hash("717aa5778731a1f4d6f0218dd3a27b114c839213b4af781427ac1e22dc9a7dea"),// support by clue
            Hash("8748687a61811032f0ed1dcdb57e01efef9983a6d9c236b82997b07477e66177"),// support by clue
            Hash("d4e844046d099bbdd48536d0431618a45cde4f0c5593f027d8ceee11f8b9c211"),// who is phone
            Hash("8170a48a4ca837cbfcfe6126afa36c9ed320dd4c0d7f9af7bd8755b0d97028cd"),// who is name
            Hash("24acfd52f9ceb424d4a2643a832638ce1673b8689fa952d9010dd44949e6b1d9"),// domain, support by clue
            Hash("aadd79b44201b5aa051dd5d38af5c51baed45c630717d43db8a260b274deaedb"),// ip
            Hash("f4a84443fb72da27731660695dd00877e8ce25b264ec418504fface62cdcbbd7"),// phone, support by clue

    };
    vector<Hash> clues = {
            Hash("24acfd52f9ceb424d4a2643a832638ce1673b8689fa952d9010dd44949e6b1d9"),// domain, support by clue
            Hash("9c72287c3f9bb38cb0186acf37b7054442b75ac32324dfd245aed46a03026de1"),// domain, support by clue
            Hash("717aa5778731a1f4d6f0218dd3a27b114c839213b4af781427ac1e22dc9a7dea"),// domain, support by clue
            Hash("8748687a61811032f0ed1dcdb57e01efef9983a6d9c236b82997b07477e66177"),// domain, support by clue
            Hash("f4a84443fb72da27731660695dd00877e8ce25b264ec418504fface62cdcbbd7"),// phone, support by clue
    };
    vector<Hash> exceptButInclude = {
            Hash("bfe47d08b0915207ce5f3b739e2bd60484069a0f0591adf4ca6baf9f5779d27a"),// ASN
    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
            Hash("d4e844046d099bbdd48536d0431618a45cde4f0c5593f027d8ceee11f8b9c211"),// who is phone
            Hash("9c72287c3f9bb38cb0186acf37b7054442b75ac32324dfd245aed46a03026de1"),// domain, support by clue
            Hash("aadd79b44201b5aa051dd5d38af5c51baed45c630717d43db8a260b274deaedb"),// ip
            Hash("f4a84443fb72da27731660695dd00877e8ce25b264ec418504fface62cdcbbd7"),// phone, support by clue
            Hash("717aa5778731a1f4d6f0218dd3a27b114c839213b4af781427ac1e22dc9a7dea"),// domain, support by clue
            Hash("8170a48a4ca837cbfcfe6126afa36c9ed320dd4c0d7f9af7bd8755b0d97028cd"),// who is name
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 5);
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
    int maxNodes = 800;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
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
        string fileName = "cwd3.json";
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
void myGroup4(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
        Hash("7e730b193c2496fc908086e8c44fc2dbbf7766e599fabde86a4bcb6afdaad66e"), // IP, clues
        Hash("6724539e5c0851f37dcf91b7ac85cb35fcd9f8ba4df0107332c308aa53d63bdb"), // Cert, clues
        Hash("4bc12ad46aae48bd6c12bcbf626389c6e8d7733ace2fce23162223ebf80d285b"), // email
        Hash("d93e740c6670760fce94cd3199e7e24bae82b16739c488d8191290ef7b403e0e"), // name
        Hash("edd207d24f6a00614700f6438b24f816a00b0f4357136a26e3cf89162a7f4a51"), // phone
    };
    vector<Hash> clues = {
            Hash("7e730b193c2496fc908086e8c44fc2dbbf7766e599fabde86a4bcb6afdaad66e"), // IP, clues
            Hash("6724539e5c0851f37dcf91b7ac85cb35fcd9f8ba4df0107332c308aa53d63bdb"), // Cert, clues
    };
    vector<Hash> exceptButInclude = {
        Hash("4eba77aac4cf89cf4d89ac512cebbaae9c589e31878d997f02bf320085cf07e0"), // ASN
    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
            Hash("7e730b193c2496fc908086e8c44fc2dbbf7766e599fabde86a4bcb6afdaad66e"), // IP, clues
            Hash("6724539e5c0851f37dcf91b7ac85cb35fcd9f8ba4df0107332c308aa53d63bdb"), // Cert, clues
            Hash("d93e740c6670760fce94cd3199e7e24bae82b16739c488d8191290ef7b403e0e"), // name
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 5);
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
    int maxNodes = 800;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
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
        string fileName;
        if(maxNodes <= 800) fileName = "cwd4-small.json";
        else fileName = "cwd4.json";
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
void myGroup6(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
        Hash("39846ff8a28f77f3b70371ba7fa2c3fc3923cde5df6c2deba2f4f4ceee78471c"), // IP
        Hash("2b615e766c6122a631a987fe73c4814e8cb2ce0ccae3a630f4ac434178e3ebdb"), // IP
        Hash("32aabdc3677cf52a9ff9f306e86049c2785b6368105a636db95eaeabef1e6e9f"), // IP
        Hash("7412fcb866b18fcb04955d4e65688080535467550921c30be7a5f41d7354ff04"), // IP
        Hash("8c25ed410181776e4bccd358feea4d33a9fa430e598accdf4e589c90be26f58d"), // IP

        Hash("2847424811ddd6856c2cf05708cd277620ad624340f7c594063c71187e496026"), // IP
        Hash("b0a473a1b5aab3f57ebb0557cb83173f109b20028875c87221543907e7cc990c"), // IP
        Hash("45611df503502357ece21c6f462cea838248b02eabfc05a828f9719cf892531c"), // IP
        Hash("f99395d1701b601326eddb62fa2d4b26ee027fecac62e548ab7d78d968801199"), // IP
        Hash("f99e0cd99b24381b415e60421517bce4e2debca1c6e16dacdb019635141bc4be"), // IP

        Hash("378c5d24a248d3a62d34cbe41eb02acb7e794ccf8e684cd3ad568ea2dfc08793"), // IP
        Hash("e8d4c083de303baa08594eec19cef6316f0872e8a332f936150169fc5ffa4553"), // IP
        Hash("a0151ef02b48d338671884d34f290eca90e9f4a888e496008b8573031df23967"), // IP
    };
    vector<Hash> clues = {

    };
    vector<Hash> exceptButInclude = {
        Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
        Hash("609e82d85112df1dcfcf89fcb3884be9dd70ee6b9fb954361d3b4b2e8720d3c2"), // Domain
        //        Hash("77f88b5761ec473e126f7bd8a6f3cf54d8c6d216a5fd03c20310af571ed84550"),
//        Hash("3ae89f055a3a8337a9955e716562b536611c2e6e903c97dc8e6d52cf850f1282"),
    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<vector<Hash>> pathByHand = {
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("39846ff8a28f77f3b70371ba7fa2c3fc3923cde5df6c2deba2f4f4ceee78471c"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("2b615e766c6122a631a987fe73c4814e8cb2ce0ccae3a630f4ac434178e3ebdb"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("32aabdc3677cf52a9ff9f306e86049c2785b6368105a636db95eaeabef1e6e9f"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("7412fcb866b18fcb04955d4e65688080535467550921c30be7a5f41d7354ff04"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("8c25ed410181776e4bccd358feea4d33a9fa430e598accdf4e589c90be26f58d"), // IP
            },

            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("2847424811ddd6856c2cf05708cd277620ad624340f7c594063c71187e496026"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("b0a473a1b5aab3f57ebb0557cb83173f109b20028875c87221543907e7cc990c"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("45611df503502357ece21c6f462cea838248b02eabfc05a828f9719cf892531c"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("f99395d1701b601326eddb62fa2d4b26ee027fecac62e548ab7d78d968801199"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("f99e0cd99b24381b415e60421517bce4e2debca1c6e16dacdb019635141bc4be"), // IP
            },

            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("378c5d24a248d3a62d34cbe41eb02acb7e794ccf8e684cd3ad568ea2dfc08793"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("e8d4c083de303baa08594eec19cef6316f0872e8a332f936150169fc5ffa4553"), // IP
            },
            {
                    Hash("5420e22575b3db0fd30df91dde6bd750b37dd1d739f70dfad17919ed9711b6f4"), // ASN
                    Hash("a0151ef02b48d338671884d34f290eca90e9f4a888e496008b8573031df23967"), // IP
            },
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 5);
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
    int maxNodes = 1000;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
    auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
    Graph group1Graph;
    group1Graph.insert(group1Links, group1Nodes);
//    vector<Hash> path;
//    if(!pathByHand.empty())
//        for(size_t i=0; i<pathByHand.size()-1; i++){
//            auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
//            path = Graph::concatPath(path, pathi);
//        }

    if(!pathByHand.empty())
    {
        // cwd json
        ofstream f;
        string fileName;
        fileName = "cwd6.json";
        f.open(outputAPIPath/fileName, ios::out);
        if(!f.good()){
            DEBUG
            cerr << "open file '" << outputAPIPath/fileName << "' error!" << endl;
            exit(1);
        }
        cout << "output to '" << outputAPIPath/fileName << "'" << endl;

        f << graphToJson_cwd(group1Links, group1Nodes,
                             pathByHand,
                             group1Graph.getNodeStatistics(centers)
        );
    }
    {// echarts
        ofstream f;
        string fileName = name + ".json";
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
                                 set<Hash>{});
        f.close();
    }
}
void myGroup7(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
        Hash("3c38a19e9514a0d470b78a4f909bf22833dfc0c0945798acfab847c6dcf9990f"), // IP
        Hash("68f8ecdd1aa27c4eccc48602e3a96b6194580c4812c43753e2f84f3356b4add1"), // IP
        Hash("19d1bf00a8f0bdfb56a38776f4b165f50d87b4860b7b85de86dd9a8a3acfd371"), // IP
        Hash("e3fec567abcb373076b295c67a2e6eb75a80b54ae5799d0e900e1b56b19f72f7"), // IP
        Hash("fe8886babbd39f799bf57ccc0a8cf092b160cd66feac38852d3e5517cd8812f2"), // IP
        Hash("9b092099bab5b41b3b99a53468fa21a6c3b1271b062b89bcabba092d98a0ca99"), // IP
    };
    vector<Hash> clues = {
        Hash("3ae89f055a3a8337a9955e716562b536611c2e6e903c97dc8e6d52cf850f1282"),
    };
    vector<Hash> exceptButInclude = {

    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
            Hash("9b092099bab5b41b3b99a53468fa21a6c3b1271b062b89bcabba092d98a0ca99"), // IP
            Hash("fe8886babbd39f799bf57ccc0a8cf092b160cd66feac38852d3e5517cd8812f2"), // IP
            Hash("e3fec567abcb373076b295c67a2e6eb75a80b54ae5799d0e900e1b56b19f72f7"), // IP
            Hash("19d1bf00a8f0bdfb56a38776f4b165f50d87b4860b7b85de86dd9a8a3acfd371"), // IP
            Hash("68f8ecdd1aa27c4eccc48602e3a96b6194580c4812c43753e2f84f3356b4add1"), // IP
            Hash("3c38a19e9514a0d470b78a4f909bf22833dfc0c0945798acfab847c6dcf9990f"), // IP
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 5);
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
    int maxNodes = 400;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
    auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
    Graph group1Graph;
    group1Graph.insert(group1Links, group1Nodes);
    vector<Hash> path;
    if(!pathByHand.empty())
        for(size_t i=0; i<pathByHand.size()-1; i++){
            auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
            path = Graph::concatPath(path, pathi);
        }
    if(!pathByHand.empty())
    {
        // cwd json
        ofstream f;
        string fileName = "cwd7.json";
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
        string fileName = name + ".json";
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
void myGroup8(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {
//        Hash("7253f0c97f9bd9a27b785a291488f7b12bf0f2abd409d1c9a68d3f30ba404d15"),
        Hash("e3e0dc1b65f0833ed5d5a2a84938bec92f8f4a2d4b10012ced7ffc6108ddcd77"),
        Hash("62d6f821ea2bf7959c5faec5836a5a6aa34bf32d3e2ab3f0773cd04d440a82d3"),
        Hash("44e926804186108faeabfa910fdfaa45c8b7f969f8513f48b7a482255edc395f"),
        Hash("e7916a8200db9bd07d84a33a8b7d8bf929f4bea78ace38daca34489ebaf578b8"), // IP
        Hash("0a0419f8d9a4828913d808dd5d00f725704cbc3441046c7cd9fa6888f4b01a9b"), // IP
        Hash("687a0f850dd4f0b13a6e5e6a1c7aaf754504dd29ffe47735813262b2104a8789"), // IP
        Hash("65a10579c3273bffef35f0e4aeeb607c6dfa32b4bffee0b30bd679e2a4600e7b"), // IP
        Hash("879d496221fbe8c831221f68ef3ed437d04b3cdfa9adf1c72fb530e28b2f6691"), // IP
        Hash("193620a8481cd9206b7cf822190e840e34736550f1e61dd8de4ab7f6b5806fb9"), // IP
        Hash("7ff973ba493d2024b72fb9590ba7f972083492d8b2e4f4c4e3d9c622814b690e"), // name 吴xxxxx龙
    };
    vector<Hash> clues = {

    };
    vector<Hash> exceptButInclude = {
        Hash("d23d7b6560877ae90a2432f9018aa316c12bbae758102e8c5ce07e0fa0772436"), // ASN
        Hash("f7d37c728a19eebd8cda83a216bfcae62f83e01cc4523de279c725797ef591d1"), // cert

    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {
        Hash("f7d37c728a19eebd8cda83a216bfcae62f83e01cc4523de279c725797ef591d1"),
        Hash("687a0f850dd4f0b13a6e5e6a1c7aaf754504dd29ffe47735813262b2104a8789"), // IP
        Hash("7ff973ba493d2024b72fb9590ba7f972083492d8b2e4f4c4e3d9c622814b690e"), // name 吴xxxxx龙
    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 4);
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
    int maxNodes = 300;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
    auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
    Graph group1Graph;
    group1Graph.insert(group1Links, group1Nodes);
    vector<Hash> path;
    DEBUG
    if(!pathByHand.empty())
        for(size_t i=0; i<pathByHand.size()-1; i++){
            auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
            path = Graph::concatPath(path, pathi);
        }
    DEBUG
    if(!pathByHand.empty())
    {
        // cwd json
        ofstream f;
        string fileName = "cwd8.json";
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
        string fileName = name + ".json";
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
void myGroup9(bool buildFromScratch){
    string name = __FUNCTION__;
    vector<Hash> centers = {

    };
    vector<Hash> clues = {

    };
    vector<Hash> exceptButInclude = {

    };
    vector<Hash> excepts = {

    };
    vector<Hash> exceptAll(excepts.cbegin(), excepts.cend());
    exceptAll.insert(exceptAll.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    vector<Hash> pathByHand = {

    };
    Graph subGraph;
    map<Hash, ItemType > subNodes;
    vector<tuple<string, Hash, Hash>> subLinks;
    if(buildFromScratch) {
        auto links = linkReader(sourceLinkName.string());
        auto nodes = nodeReader(sourceNodeName.string());
        Graph graph;
        graph.insert(links, nodes);
        auto subGraphVec = graph.divideSubGraphByDepth(centers, 4);
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
    int maxNodes = 300;// 150 is ok
    auto group1Vec = subGraph.divideSubGraphByCenters(centers, maxNodes, [](uint32_t edgeWeight, bool hasBlack,
                                                                            uint32_t depth) -> int {
//            return -(int)edgeWeight - 10 * hasBlack + 25*(int)depth;
        return 25 * (int) depth;
    }, exceptAll);
    group1Vec.insert(group1Vec.end(), exceptButInclude.cbegin(), exceptButInclude.cend());
    group1Vec.insert(group1Vec.end(), clues.cbegin(), clues.cend());
    auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
    Graph group1Graph;
    group1Graph.insert(group1Links, group1Nodes);
    vector<Hash> path;
    DEBUG
    if(!pathByHand.empty())
        for(size_t i=0; i<pathByHand.size()-1; i++){
            auto pathi = group1Graph.getShortestPathBetween(pathByHand[i], pathByHand[i+1]);
            path = Graph::concatPath(path, pathi);
        }
    DEBUG
    if(!pathByHand.empty())
    {
        // cwd json
        ofstream f;
        string fileName = "cwd9.json";
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
        string fileName = name + ".json";
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
//    myGroup1(0);
//    myGroup2(0);
//    myGroup3(0);
    myGroup9(1);
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