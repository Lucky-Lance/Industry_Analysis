#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "parser.h"
#include <map>
#include <queue>
#include <random>
#include <algorithm>
#include <ctime>
#include <array>
#include <sstream>
#include <iomanip>
#include <json/json.h>
using namespace std;
const vector<string> nodeVisTypeToStr = {
        "IMPORTANT_NODE",
        "MATTER_NODE",
        "NORMAL_NODE",
        "INSIGNIFICANT_NODE",
        "USELESS_NODE",
        "UNVISITED_NODE"
};
array<int, 100> edgeWeightCount = {};
void print_edgeWeightCount(){
    cout << "x = [";
    for(int i=0; i<edgeWeightCount.size(); i++){
        cout << i << ", ";
    }
    cout << "]\n";

    cout << "y = [";
    for(auto x: edgeWeightCount){
        cout << x << ", ";
    }
    cout << "]\n";
}

template<class T>
struct Edge{
	T from, to;
	int weight;
	void print_edge()const noexcept{
		std::cout << from << " -> " << to << "  w = " <<  weight << '\n';
	}
	Edge(T f, T t,int w):from(f),to(t),weight(w){}
};
template<class T>
struct Edge_Sort{
	bool operator()(const Edge<T>& a,const Edge<T>& b){
		return a.weight < b.weight;
	}
};

constexpr const uint32_t UNMATCHED = -1;
class Graph{
private:
    map<Hash, vector<Edge<Hash>> > edges;// edge<T> 存的[结点原始编号]
    set<Hash> raw_node;// 存放所有结点,存的[结点原始编号]
	map<Hash, uint32_t> raw_to_mapped;// [结点原始编号] -> [连续化结点编号]
	map<uint32_t, Hash> mapped_to_raw;// [连续化结点编号] -> [结点原始编号]
    map<uint32_t , vector<char>> mapped_to_black;
	vector<int> nodeDegreeWeight;
public:
    Graph(){}
    [[nodiscard]]size_t numNodes() const {
        return raw_node.size();
    }
    void insert(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes){
        // string relation; ItemType from, to;
        nodeDegreeWeight.reserve(nodeDegreeWeight.size()+nodes.size());
        for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
            const Hash h = iter->first;
            if(raw_node.insert(h).second){
                uint32_t mapped_id = raw_node.size() - 1U;
                raw_to_mapped[h] = mapped_id;
                mapped_to_raw[mapped_id] = h;
                mapped_to_black[mapped_id] = iter->second.industry;
                nodeDegreeWeight.emplace_back(0U);
                assert(nodeDegreeWeight.size() == raw_node.size());
            }
        }
        for(const LinkItemType& link: links){
            const auto& [relation, from, to] = link;
            assert(raw_node.find(from) != raw_node.cend());
            assert(raw_node.find(to) != raw_node.cend());
            const vector<char>& industry_from = nodes.at(from).industry;
            const vector<char>& industry_to = nodes.at(to).industry;
            int W_from_2_to = 10 * (industry_from.size() > 0) + 2 * industry_from.size();
            int W_to_2_from = 10 * (industry_to.size() > 0) + 2 * industry_to.size();
            int weight = 1 + W_from_2_to + W_to_2_from;
            edgeWeightCount[weight] += 1;
//			if(weight == 1){
//				continue;
//			}
            uint32_t mapped_from = raw_to_mapped[from];
            uint32_t mapped_to = raw_to_mapped[to];
            nodeDegreeWeight[mapped_from] += W_to_2_from;
            nodeDegreeWeight[mapped_to] += W_from_2_to;
            {//  无向边 from -> to
                Edge<Hash> e(from, to, W_from_2_to);
                auto result = edges.insert(make_pair(from, vector<Edge<Hash>>{e}));
                if(!result.second){
                    (result.first->second).emplace_back(e);
                }
            }
            {//  无向边 to -> from
                Edge<Hash> e(to, from, W_to_2_from);
                auto result = edges.insert(make_pair(to, vector<Edge<Hash>>{e}));
                if(!result.second){
                    (result.first->second).emplace_back(e);
                }
            }
        }
    }

private:
	vector<vector<uint32_t>> dfsGroup;
	vector<uint32_t> dfsVers;
	void _dfsDivide(uint32_t v, uint32_t verNum){
	    dfsVers[v] = verNum;
	    dfsGroup[verNum].push_back(v);
	    Hash h = mapped_to_raw[v];
	    for(const auto& edge: edges[h]){
            if(edge.weight <= 1) continue;
            uint32_t u = raw_to_mapped[edge.to];
            if(dfsVers[u] == UNMATCHED){
                _dfsDivide(u, verNum);
            }
	    }
	}

public:
    struct NodeDegreeType{
	    uint32_t mapped_id;
	    int degreeWeight;
        NodeDegreeType(uint32_t id, int w):mapped_id(id), degreeWeight(w){}
	    bool operator<(const NodeDegreeType& other) const {
	        return degreeWeight < other.degreeWeight;
	    }
	    bool operator>(const NodeDegreeType& other) const {
            return degreeWeight > other.degreeWeight;
        }
	};
    enum NodeVisType{
        IMPORTANT_NODE = 0,
        MATTER_NODE = 1,
        NORMAL_NODE = 2,
        INSIGNIFICANT_NODE = 3,
        USELESS_NODE = 4,
        UNVISITED_NODE = 5,
    };

public:
    [[nodiscard]]vector<uint32_t> getDepth(uint32_t root) const {
        vector<uint32_t> result;
        result.resize(numNodes(), UNMATCHED);
        queue<uint32_t> bfsQueue;
        result[root] = 0;
        bfsQueue.emplace(root);
        while(!bfsQueue.empty()){
            uint32_t node = bfsQueue.front();
            bfsQueue.pop();
            uint32_t nextDepth = result[node] + 1;
            const vector<Edge<Hash>>& rootEdges = edges.at(mapped_to_raw.at(node));
            for(const auto& edge: rootEdges){
                uint32_t nextNode = raw_to_mapped.at(edge.to);
                if(result[nextNode] == UNMATCHED){
                    result[nextNode] = nextDepth;
                    bfsQueue.emplace(nextNode);
                }
            }
        }
        return result;
    }
    vector<vector<Hash>> bfsAnalyseGraphs() const {
        vector<NodeVisType> nodeStatus;
        vector<NodeDegreeType> nodeWeights;
        nodeWeights.reserve(numNodes());
        nodeStatus.resize(numNodes(), UNVISITED_NODE);
        for(uint32_t id=0; id< nodeDegreeWeight.size(); id++){
            nodeWeights.emplace_back(NodeDegreeType(id, nodeDegreeWeight[id]));
        }
        sort(nodeWeights.begin(), nodeWeights.end(), greater<NodeDegreeType>());
        vector<vector<Hash>> subGraphs;
        for(const auto& nodeWeight: nodeWeights) {
            if(nodeWeight.degreeWeight < 800) break;
            array<vector<uint32_t>, UNVISITED_NODE> nodeTypeRecord;
            uint32_t root = nodeWeight.mapped_id;
            if(nodeStatus[root] != UNVISITED_NODE) continue;
            cout << "Analyse root!" << endl;

            auto depth = getDepth(root);
            queue<tuple<uint32_t, NodeVisType>> bfsQueue;
            bfsQueue.emplace(make_tuple(root, IMPORTANT_NODE));
            while (!bfsQueue.empty()) {
                auto [nodeID, currentStatus] = bfsQueue.front();
                bfsQueue.pop();
                if (nodeStatus[nodeID] != UNVISITED_NODE) continue;
                if (currentStatus < NORMAL_NODE && nodeDegreeWeight[nodeID] < 50) {
//                cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought down from " << nodeVisTypeToStr[currentStatus] << " to NORMAL!" << endl;
                    currentStatus = NORMAL_NODE;
                }
                if (depth[nodeID] < 4) {
                    if (currentStatus > MATTER_NODE && nodeDegreeWeight[nodeID] > 800) {
//                    cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought up to MATTER!" << endl;
                        currentStatus = MATTER_NODE;
                    } else if (currentStatus > NORMAL_NODE && nodeDegreeWeight[nodeID] > 300) {
//                    cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought up to NORMAL!" << endl;
                        currentStatus = NORMAL_NODE;
                    }
                    if (depth[nodeID] >= 2 && nodeDegreeWeight[nodeID] > 800 && currentStatus <= MATTER_NODE) {
//                    cerr << "WARNING!!" << endl;
                        currentStatus = USELESS_NODE;
                    }
                } else {
                    if (currentStatus < INSIGNIFICANT_NODE && nodeDegreeWeight[nodeID] < 100) {
//                    cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought down to INSIGNIFICANT!" << endl;
                        currentStatus = INSIGNIFICANT_NODE;
                    } else if (nodeDegreeWeight[nodeID] < 50) {
//                    cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought down to USELESS!" << endl;
                        currentStatus = USELESS_NODE;
                    }
                }
                if (mapped_to_black.at(nodeID).empty() && currentStatus >= NORMAL_NODE) {
//                cout << "node weight " << nodeDegreeWeight[nodeID] << " is brought down to USELESS!" << endl;
                    currentStatus = USELESS_NODE;
                }
                nodeStatus[nodeID] = currentStatus;
                nodeTypeRecord[(size_t) currentStatus].emplace_back(nodeID);

                NodeVisType nextStatus = static_cast<NodeVisType>(currentStatus + 1);
                if (nextStatus >= USELESS_NODE) continue;
                const vector<Edge<Hash>> &rootEdges = edges.at(mapped_to_raw.at(nodeID));
                for (const auto &edge: rootEdges) {
                    uint32_t u = raw_to_mapped.at(edge.to);
                    if (nodeStatus[u] == UNVISITED_NODE) {
                        bfsQueue.emplace(make_tuple(u, nextStatus));
                    }
                }
            }
            vector<Hash> subGraph;
            for (uint32_t depth = IMPORTANT_NODE; depth < USELESS_NODE; depth++) {
                cout << "=================================== " << "depth = " << depth
                     << " ===================================" << endl;
                for (auto n: nodeTypeRecord[depth]) {
                    cout << nodeDegreeWeight[n] << ' ';
                    subGraph.emplace_back(mapped_to_raw.at(n));
                }
                cout << endl;
            }
            subGraphs.emplace_back(move(subGraph));
            for(auto iter=nodeStatus.begin(); iter!=nodeStatus.end();iter++){
                if(*iter == USELESS_NODE) *iter = UNVISITED_NODE;
            }
        }
        return subGraphs;
    }
public:
	[[nodiscard]] vector<vector<Hash>> divideSubGraph(){
	    int verNum = 0;
        dfsVers.resize(raw_node.size(), UNMATCHED);
	    DEBUG
	    for(uint32_t v = 0; v<dfsVers.size(); v++){
            if(dfsVers[v] != UNMATCHED) {
                continue;
            }
            dfsGroup.emplace_back(vector<uint32_t>{});
            _dfsDivide(v, verNum);
            verNum++;
	    }
	    DEBUG
	    vector<vector<Hash>> subGraphs;
	    for(const auto& group: dfsGroup){
	        vector<Hash> subGraph;
            subGraph.reserve(group.size());
	        for(uint32_t v: group){
	            subGraph.emplace_back(mapped_to_raw[v]);
	        }
	        subGraphs.emplace_back(move(subGraph));
	    }
        return subGraphs;
	}

};


#endif