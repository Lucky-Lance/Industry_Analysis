#define ENABLE_TEST
#include "filesystem"
#include "graph.h"
#include "parser.h"
using namespace std;

int main(){
    auto testFolder = filesystem::path("./tmp/");
    if(!filesystem::is_directory(testFolder)){
        if(!filesystem::create_directory(testFolder)){
            cerr << "try to mkdir '" << testFolder << "', but failed!" << endl;
            exit(1);
        }
    }
    string linkName = "/media/ftc/DATA/output/00000055-Link-00172977.csv";
    string nodeName = "/media/ftc/DATA/output/00000055-Node-00172977.csv";
    vector<LinkItemType> links = linkReader(linkName);
    map<Hash, ItemType> nodes  = nodeReader(nodeName);
    Graph graph;
    graph.insert(links, nodes);
    auto subGraph = graph.bfsAnalyseGraph();
    outputSubGraph(nodes, links, subGraph, make_tuple("./tmp/subbig-Node-1.csv", "./tmp/subbig-Link-1.csv"));
    outputSubGraph_forTest(nodes, links, subGraph, make_tuple("./tmp/subbig-Node-2.csv", "./tmp/subbig-Link-2.csv"));
    bool ok = true;
    {
        string linkName1 = "./tmp/subbig-Link-1.csv";
        string linkName2 = "./tmp/subbig-Link-2.csv";
        auto link1 = linkReader(linkName1);
        auto link2 = linkReader(linkName2);
        if(testEqual(link1, link2)){
            cout << "'links output' test passed!" << endl;
        }
        else{
            ok = false;
            cout << "'links output' test failed" << endl;
        }
    }
    {
        string nodeName1 = "./tmp/subbig-Node-1.csv";
        string nodeName2 = "./tmp/subbig-Node-2.csv";
        auto nodes1 = nodeReader(nodeName1);
        auto nodes2 = nodeReader(nodeName2);
        if(testEqual(nodes1, nodes2)){
            cout << "'nodes output' test passed!" << endl;
        }
        else{
            ok = false;
            cout << "'nodes output' test failed" << endl;
        }
    }
    return ok? 0:1;
}