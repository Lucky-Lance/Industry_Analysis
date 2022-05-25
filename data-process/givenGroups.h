#ifndef _GIVEN_GROUPS_H_
#define _GIVEN_GROUPS_H_
#include "path.h"
#include "graph.h"
void outputsubsubForFTC(){
    auto subLinkName = outputPath / "subsubbig-Link.csv";
    auto subNodeName = outputPath / "subsubbig-Node.csv";
    auto subLinks = linkReader(subLinkName.string());
    auto subNodes = nodeReader(subNodeName.string());
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
                tuple<string, string>((outputPath / "subsubsubbig-Node.csv").string(),
                                      (outputPath / "subsubsubbig-Link.csv").string()));
    ofstream f;
    f.open(outputPath/"subsubsubbig.json", ios::out);
    if(!f.good()){
        cerr << "open file '" << outputPath/"subsubsubbig.json" << "' error!" << endl;
        exit(1);
    }
    f << graphToJson_echarts(subSubLinks, subSubNodes, {});
    f.close();
}
void group1(bool buildFromScratch){
    Hash h1 = Hash("c58c149eec59bb14b0c102a0f303d4c20366926b5c3206555d2937474124beb9");
    Hash h2 = Hash("f3554b666038baffa5814c319d3053ee2c2eb30d31d0ef509a1a463386b69845");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2}, 4);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / "group1-Node.csv").string(),
                (outputPath / "group1-Link.csv").string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / "group1-Link.csv";
        auto subNodeName = outputPath / "group1-Node.csv";
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG
    // 400
//    for(auto maxNodes: vector<int>{30, 40, 50, 60, 70, 80, 90, 100, 150, 200, 250, 300, 350,400, 500,600}){
        for(auto maxNodes: vector<int>{1000, 1500, 2000, 2500,3000,3500, 4000}){
        ofstream f;
        string fileName = "group1-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2});
        f.close();
    }

}
void group2(bool buildFromScratch){
    Hash h1 = Hash("400c19e584976ff2a35950659d4d148a3d146f1b71692468132b849b0eb8702c");
    Hash h2 = Hash("b10f98a9b53806ccd3a5ee45676c7c09366545c5b12aa96955cde3953e7ad058");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2}, 5);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / "group2-Node.csv").string(),
                (outputPath / "group2-Link.csv").string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / "group2-Link.csv";
        auto subNodeName = outputPath / "group2-Node.csv";
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG

    for(auto maxNodes: vector<int>{300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800}){
        ofstream f;
        string fileName = "group2-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2});
        f.close();
    }

}
void group3(bool buildFromScratch){
    Hash h1 = Hash("24acfd52f9ceb424d4a2643a832638ce1673b8689fa952d9010dd44949e6b1d9");
    Hash h2 = Hash("9c72287c3f9bb38cb0186acf37b7054442b75ac32324dfd245aed46a03026de1");
    Hash h3 = Hash("717aa5778731a1f4d6f0218dd3a27b114c839213b4af781427ac1e22dc9a7dea");
    Hash h4 = Hash("8748687a61811032f0ed1dcdb57e01efef9983a6d9c236b82997b07477e66177");
    Hash h5 = Hash("f4a84443fb72da27731660695dd00877e8ce25b264ec418504fface62cdcbbd7");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2, h3, h4, h5}, 5);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / "group3-Node.csv").string(),
                (outputPath / "group3-Link.csv").string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / "group3-Link.csv";
        auto subNodeName = outputPath / "group3-Node.csv";
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG

    for(auto maxNodes: vector<int>{300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800}){
        ofstream f;
        string fileName = "group3-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2, h3, h4, h5}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2, h3, h4, h5});
        f.close();
    }

}
void group4(bool buildFromScratch){
    Hash h1 = Hash("7e730b193c2496fc908086e8c44fc2dbbf7766e599fabde86a4bcb6afdaad66e");
    Hash h2 = Hash("6724539e5c0851f37dcf91b7ac85cb35fcd9f8ba4df0107332c308aa53d63bdb");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2}, 5);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / "group4-Node.csv").string(),
                (outputPath / "group4-Link.csv").string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / "group4-Link.csv";
        auto subNodeName = outputPath / "group4-Node.csv";
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG

    for(auto maxNodes: vector<int>{2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000}){
        ofstream f;
        string fileName = "group4-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2});
        f.close();
    }
}
void group5(bool buildFromScratch){
    Hash h1 = Hash("fd0a3f6712ff520edae7e554cb6dfb4bdd2af1e4a97a39ed9357b31b6888b4af");
    Hash h2 = Hash("21ce145cae6730a99300bf677b83bbe430cc0ec957047172e73659372f0031b8");
    Hash h3 = Hash("7939d01c5b99c39d2a0f2b418f6060b917804e60c15309811ef4059257c0818a");
    Hash h4 = Hash("587da0bac152713947db682a5443ef639e35f77a3b59e246e8a07c5eccae67e5");
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
        auto subGraphVec = graph.divideSubGraphByDepth(vector<Hash>{h1, h2, h3, h4}, 5);
        tie(subNodes, subLinks) = getSubGraph(nodes, links, subGraphVec);
        outputGraph(subNodes, subLinks, tuple<string, string>(
                (outputPath / "group5-Node.csv").string(),
                (outputPath / "group5-Link.csv").string()
        ));
        cout << "node size = " << subNodes.size() << endl;
        subGraph.insert(subLinks, subNodes);
    }
    else{
        auto subLinkName = outputPath / "group5-Link.csv";
        auto subNodeName = outputPath / "group5-Node.csv";
        subLinks = linkReader(subLinkName.string());
        subNodes = nodeReader(subNodeName.string());
        subGraph.insert(subLinks, subNodes);
    }
    DEBUG

    for(auto maxNodes: vector<int>{2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000}){
        ofstream f;
        string fileName = "group5-" + to_string(maxNodes) + ".json";
        f.open(outputPath/fileName, ios::out);
        if(!f.good()){
            cerr << "open file error!" << endl;
            DEBUG;
            exit(1);
        }
        auto group1Vec = subGraph.divideSubGraphByCenters(vector<Hash>{h1, h2, h3, h4}, maxNodes);
        auto [group1Nodes, group1Links] = getSubGraph(subNodes, subLinks, group1Vec);
        f << graphToJson_echarts(group1Links, group1Nodes, set<Hash>{h1, h2, h3, h4});
        f.close();
    }
}
#endif