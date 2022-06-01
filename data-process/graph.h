#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "parser.h"
#include <map>
#include <queue>
#include <random>
#include <algorithm>
#include <functional>
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

constexpr const uint32_t UNMATCHED = -1;
class Graph{
public:
    map<uint32_t , vector<Edge<uint32_t>> > edges;// edge<T> 存的[结点原始编号]
    set<Hash> raw_node;// 存放所有结点,存的[结点原始编号]
	map<Hash, uint32_t> raw_to_mapped;// [结点原始编号] -> [连续化结点编号]
	map<uint32_t, Hash> mapped_to_raw;// [连续化结点编号] -> [结点原始编号]
    map<uint32_t , vector<char>> mapped_to_black;
	vector<int> nodeDegreeWeight;
    map<uint32_t, NodeAssetType> mapped_to_assetType;
public:
    Graph() = default;
    Graph(const Graph&) = delete;
    [[nodiscard]] uint32_t hashToMapped(Hash h) const {
        return raw_to_mapped.at(h);
    }
    [[nodiscard]]size_t numNodes() const {
        return raw_node.size();
    }
    [[nodiscard]]size_t numEdges() const {
        return edges.size();
    }
    [[nodiscard]] map<Hash, tuple<uint32_t, NodeAssetType, uint32_t>> getNodeStatistics(const vector<Hash>& centers) const {
        // return: map[Hash] -> (degree, NodeAssetType, (top: 0, mid: 1, bottom: 2))
        // for cwd
        map<Hash, tuple<uint32_t, NodeAssetType, uint32_t>> result;
        {// get top, mid & bottom
            vector<uint32_t> reaches;// top mid & bottom
            reaches.resize(numNodes(), UNMATCHED);
            vector<tuple<NodeIdType, DepthType>> depthInput;
            depthInput.reserve(centers.size());
            for (const auto& nodeHash: centers) {
                auto nodeID = raw_to_mapped.at(nodeHash);
                depthInput.emplace_back(make_tuple(nodeID, 0U));
                reaches[nodeID] = 0;
            }
            {
                const auto depthVec = getDepth(depthInput);
                assert(depthVec.size() == numNodes());
                // 0: top,  1: mid, 2: bottom
                for (uint32_t nodeID = 0; nodeID < numNodes(); nodeID++) {
                    auto degree = edges.at(nodeID).size();
                    if (reaches[nodeID] == UNMATCHED) {
                        if (degree == 1) {
                            reaches[nodeID] = 2;
                        } else if (depthVec.at(nodeID) < 2) {
                            reaches[nodeID] = 1;
                        } else {
                            reaches[nodeID] = 2;
                        }
                    }
                }
            }
            for(uint32_t nodeID=0; nodeID<numNodes(); nodeID++){
                Hash h = mapped_to_raw.at(nodeID);
                auto insertSucc = result.insert(make_pair(h,
                    make_tuple(
                        /*degree*/ edges.at(nodeID).size(),
                        /*NodeAssetType*/ mapped_to_assetType.at(nodeID),
                        /*reach*/reaches.at(nodeID)
                    ))).second;
                assert(insertSucc);
            }
        }
        return result;
    }
    [[nodiscard]]vector< array<uint32_t,8> > getStatistics(const vector<Hash>& centers) const {
        // this function is for lxd
        // array:
        // [0] count; [1] domain; [2] ip; [3] cert;
        // [4] others; [5] top; [6] mid; [7] bottom;
        vector<array<uint32_t,8>> statistic;
        for(uint32_t nodeID=0; nodeID<numNodes(); nodeID++){
            auto degree = edges.at(nodeID).size();
            if(degree >= statistic.size()){
                statistic.resize(degree+1, {0,0,0,0,0,0,0,0});
            }
            statistic[degree][0] += 1;
            switch(mapped_to_assetType.at(nodeID)){
                case DOMAIN_ASS: statistic[degree][1] += 1; break;
                case IP_ASS: statistic[degree][2] += 1; break;
                case CERT_ASS: statistic[degree][3] += 1; break;
                case OTHERS_ASS: statistic[degree][4] += 1; break;
                default: assert(0);
            }
        }
        vector<uint32_t> reaches;
        reaches.resize(numNodes(), UNMATCHED);
        {// get top, mid & bottom
            vector<tuple<NodeIdType, DepthType>> depthInput;
            depthInput.reserve(centers.size());
            for (const auto& nodeHash: centers) {
                auto nodeID = raw_to_mapped.at(nodeHash);
                depthInput.emplace_back(make_tuple(nodeID, 0U));
                reaches[nodeID] = 0;
            }
            {
                const auto depthVec = getDepth(depthInput);
                assert(depthVec.size() == numNodes());
                // 0: top,  1: mid, 2: bottom
                for (uint32_t nodeID = 0; nodeID < numNodes(); nodeID++) {
                    auto degree = edges.at(nodeID).size();
                    if (reaches[nodeID] == UNMATCHED) {
                        if (statistic.at(degree)[0] == 1) {
                            reaches[nodeID] = 2;
                        } else if (depthVec.at(nodeID) < 2) {
                            reaches[nodeID] = 1;
                        } else {
                            reaches[nodeID] = 2;
                        }
                    }
//                    cout << 5 + reaches[nodeID] << endl;
                    statistic.at(degree)[5 + reaches[nodeID]] += 1;
                }
            }
        }
        return statistic;
    }
    void insert(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes){
        size_t prevSize = numNodes();
        nodeDegreeWeight.reserve(nodeDegreeWeight.size()+nodes.size());
        stringToAssetType.at("Domain");
        for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
            const Hash h = iter->first;
            if(raw_node.insert(h).second){
                uint32_t mapped_id = raw_node.size() - 1U;
                raw_to_mapped[h] = mapped_id;
                mapped_to_raw[mapped_id] = h;
                mapped_to_black[mapped_id] = iter->second.industry;
                mapped_to_assetType[mapped_id] = stringToAssetType.at(iter->second.type);
                nodeDegreeWeight.emplace_back(0U);
                assert(nodeDegreeWeight.size() == raw_node.size());
            }
        }
        for(uint32_t nodeID=prevSize; nodeID<numNodes(); nodeID++){
            edges.insert(make_pair(nodeID, vector<Edge<uint32_t>>{}));
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

            uint32_t mapped_from = raw_to_mapped[from];
            uint32_t mapped_to = raw_to_mapped[to];
//            nodeDegreeWeight[mapped_from] += W_to_2_from;
//            nodeDegreeWeight[mapped_to] += W_from_2_to;
            nodeDegreeWeight[mapped_from] += weight;
            nodeDegreeWeight[mapped_to] += weight;
            {//  无向边 from -> to
//                Edge<uint32_t> e(mapped_from, mapped_to, W_from_2_to);
                Edge<uint32_t> e(mapped_from, mapped_to, weight);
                edges[mapped_from].emplace_back(e);
            }
            {//  无向边 to -> from
//                Edge<uint32_t> e(mapped_to, mapped_from, W_to_2_from);
                Edge<uint32_t> e(mapped_to, mapped_from, weight);
                edges[mapped_to].emplace_back(e);
            }
        }
    }

private:


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
    using NodeIdType = uint32_t;
    using DepthType = uint32_t;
    [[nodiscard]] vector<Hash> getShortestPathBetween(Hash node1, Hash node2) const {
        // do some check
        {
            if(raw_to_mapped.find(node1) == raw_to_mapped.cend()){
                cerr << node1.str << endl;
                exit(1);
            }
            if(raw_to_mapped.find(node2) == raw_to_mapped.cend()){
                cerr << node2.str << endl;
                exit(1);
            }
        }
        uint32_t node1ID = raw_to_mapped.at(node1);
        uint32_t node2ID = raw_to_mapped.at(node2);
        auto pathMapped = getShortestPathBetween(node1ID, node2ID);
        vector<Hash> result;
        for(uint32_t nodeID: pathMapped){
            result.emplace_back(mapped_to_raw.at(nodeID));
        }
        return result;
    }
    [[nodiscard]] vector<uint32_t> getShortestPathBetween(uint32_t node1, uint32_t node2) const {
        // return path from node1 to node2
        // bfsQueue<nodeID, prevNodeID, nodeDepth>
        queue<tuple<NodeIdType, NodeIdType, DepthType>> bfsQueue;
        // result<prevNodeID, nodeDepth>
        vector<tuple<NodeIdType, DepthType>> result;
        result.resize(numNodes(), make_tuple(UNMATCHED, UNMATCHED));
        bfsQueue.emplace(make_tuple(node1, UNMATCHED, 0));
        while (!bfsQueue.empty()){
            auto [nodeID, prevNodeID, nodeDepth] = bfsQueue.front();
            bfsQueue.pop();
            if(nodeDepth >= get<1>(result[nodeID])) continue;
            result[nodeID] = make_tuple(prevNodeID, nodeDepth);
            uint32_t nextDepth = nodeDepth+1;
            const auto& rootEdges = edges.at(nodeID);
            for(const auto& edge: rootEdges){
                uint32_t nextNode = edge.to;
                if(nextDepth < get<1>(result[nextNode])){
                    bfsQueue.emplace(make_tuple(nextNode, nodeID, nextDepth));
                }
            }
        }
        vector<uint32_t> path;
        for(uint32_t nodeID = node2; nodeID!=UNMATCHED; nodeID = get<0>(result[nodeID])){
            path.emplace_back(nodeID);
            cout << "ID = " << nodeID << ", depth = " << get<1>(result[nodeID]) << endl;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    [[nodiscard]]vector<uint32_t> getDepth(const vector<tuple<NodeIdType, DepthType>>& centers) const {
        set<NodeIdType> centerSet;
        for(const auto& c: centers){
            centerSet.insert(get<0>(c));
        }
        vector<uint32_t> result;
        result.resize(numNodes(), UNMATCHED);
        queue<tuple<NodeIdType, DepthType>> bfsQueue;
        for(const auto & center: centers){
            bfsQueue.emplace(center);
        }
        while(!bfsQueue.empty()){
            auto [nodeID, nodeDepth] = bfsQueue.front();
            bfsQueue.pop();
            if(nodeDepth >= result[nodeID]) continue;
            if(centerSet.find(nodeID) == centerSet.cend()){
                result[nodeID] = nodeDepth;
            }
            else{
                if(result[nodeID] == UNMATCHED){
                    result[nodeID] = nodeDepth;
                }
                else continue;
            }
            uint32_t nextDepth = nodeDepth + 1;
            const auto& rootEdges = edges.at(nodeID);
            for(const auto& edge: rootEdges){
                uint32_t nextNode = edge.to;
                if(nextDepth < result[nextNode]){
                    bfsQueue.emplace(make_tuple(nextNode, nextDepth));
                }
            }
        }
        return result;
    }
    [[nodiscard]] vector<tuple<NodeIdType, DepthType>> getMainAssetNodes(uint32_t center, uint32_t maxDepth) const {
        uint32_t centerWeight = nodeDegreeWeight[center];
        map<NodeIdType, DepthType> mainAssetMap;
        vector<uint32_t> depth;
        depth.resize(numNodes(), UNMATCHED);
        // <nodeID, depth_i>
        queue<tuple<NodeIdType, DepthType>> bfsQueue;
        {// process root
            bfsQueue.emplace(make_tuple(center, 0U));
        }
        while (!bfsQueue.empty()) {
            auto [nodeID, currentDepth] = bfsQueue.front();
            bfsQueue.pop();
            if (currentDepth >= depth[nodeID]) continue;
            depth[nodeID] = currentDepth;
            NodeAssetType nodeAssetType = mapped_to_assetType.at(nodeID);
            if(currentDepth <= maxDepth && (nodeAssetType == CERT_ASS || nodeAssetType == IP_ASS)){
//                uint32_t weight = currentDepth == 0? 0U : 1U;
                uint32_t weight = currentDepth;
                auto ptr = mainAssetMap.insert(make_pair(nodeID, weight));
                if(!ptr.second){
                    ptr.first->second = weight;
                }
            }

            const auto &rootEdges = edges.at(nodeID);
            uint32_t nextDepth = currentDepth + 1;
            if(nextDepth > maxDepth) continue;
            for (const auto &edge: rootEdges) {
                uint32_t u = edge.to;
                bfsQueue.emplace(make_tuple(u, nextDepth));
            }
        }
        vector<tuple<NodeIdType, DepthType>> result;
        result.reserve(mainAssetMap.size());
        for(const auto& ptr: mainAssetMap){
            result.emplace_back(make_tuple(ptr.first, ptr.second));
        }
        return result;
    }
    [[nodiscard]] vector<Hash> bfsAnalyseGraph() const {
        // main function
        vector<NodeVisType> nodeStatus;
        vector<NodeDegreeType> nodeWeights;
        nodeWeights.reserve(numNodes());
        nodeStatus.resize(numNodes(), UNVISITED_NODE);
        for(uint32_t id=0; id< nodeDegreeWeight.size(); id++){
            nodeWeights.emplace_back(NodeDegreeType(id, nodeDegreeWeight[id]));
        }
        sort(nodeWeights.begin(), nodeWeights.end(), greater<>());
        uint32_t root = nodeWeights[0].mapped_id;
        auto mainAssetNodes = getMainAssetNodes(root, 2U);

        for(auto ptr=mainAssetNodes.begin();ptr!=mainAssetNodes.end();ptr++){
            uint32_t nodeID = get<0>(*ptr);
            if(nodeID == root){
                cout << "root depth = " << get<1>(*ptr) << endl;
                *ptr = make_tuple(nodeID, 0);
                continue;
            }
            if(getNeighborBlack(nodeID)<0.2){
                cout << "------------------" << endl;
                *ptr = make_tuple(nodeID, 4);
                continue;
            }
            auto maxFlow = getMaxFlow(root, nodeID);
            cout << maxFlow << " / " << nodeDegreeWeight[nodeID] << endl;
            if((double)maxFlow < nodeDegreeWeight[nodeID] * 0.5){
                *ptr = make_tuple(nodeID, 4);
                cout << "set to 4" << endl;
            }
            else if((double)maxFlow < nodeDegreeWeight[nodeID] * 0.8){
                *ptr = make_tuple(nodeID, 2);
                cout << "set to 2" << endl;
            }
            else if((double)maxFlow > nodeDegreeWeight[nodeID] * 0.9){
                *ptr = make_tuple(nodeID, 1);
            }
        }
        auto depth = getDepth(mainAssetNodes);
        for(auto mainAssetNode: mainAssetNodes){
            auto [nodeID, nodeDepth] = mainAssetNode;
            assert(depth[nodeID] == nodeDepth);
        }
        vector<Hash> result;
        for(uint32_t nodeID=0; nodeID<depth.size();nodeID++){
            if(depth[nodeID]<=2){
                result.emplace_back(mapped_to_raw.at(nodeID));
            }
        }
        cout << result.size() << endl;
        return result;
    }
    [[nodiscard]] vector<vector<Hash>> bfsAnalyseGraphs() const {
        vector<NodeVisType> nodeStatus;
        vector<NodeDegreeType> nodeWeights;
        nodeWeights.reserve(numNodes());
        nodeStatus.resize(numNodes(), UNVISITED_NODE);
        for(uint32_t id=0; id< nodeDegreeWeight.size(); id++){
            nodeWeights.emplace_back(NodeDegreeType(id, nodeDegreeWeight[id]));
        }
        sort(nodeWeights.begin(), nodeWeights.end(), greater<>());
        vector<vector<Hash>> subGraphs;
        for(const auto& nodeWeight: nodeWeights) {
            if(nodeWeight.degreeWeight < 800) break;
            array<vector<uint32_t>, UNVISITED_NODE> nodeTypeRecord;
            uint32_t root = nodeWeight.mapped_id;
            if(nodeStatus[root] != UNVISITED_NODE) continue;
            cout << "Analyse root!" << endl;
            auto centers = getMainAssetNodes(root, 2);
            auto depth = getDepth(centers);
            queue<tuple<uint32_t, NodeVisType>> bfsQueue;
            bfsQueue.emplace(make_tuple(root, IMPORTANT_NODE));
            while (!bfsQueue.empty()) {
                auto [nodeID, currentStatus] = bfsQueue.front();
                bfsQueue.pop();
                if (nodeStatus[nodeID] <= currentStatus) continue;
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
                if (nodeStatus[nodeID] <= currentStatus) continue;
                nodeStatus[nodeID] = currentStatus;
                nodeTypeRecord[(size_t) currentStatus].emplace_back(nodeID);

                auto nextStatus = static_cast<NodeVisType>(currentStatus + 1);
                if (nextStatus >= USELESS_NODE) continue;
                const auto &rootEdges = edges.at(nodeID);
                for (const auto &edge: rootEdges) {
                    uint32_t u = edge.to;
                    bfsQueue.emplace(make_tuple(u, nextStatus));
                }
            }
            vector<Hash> subGraph;
            for (uint32_t d = IMPORTANT_NODE; d < USELESS_NODE; d++) {
                cout << "=================================== " << "depth = " << d
                     << " ===================================" << endl;
                for (auto n: nodeTypeRecord[d]) {
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
    [[nodiscard]] vector<Hash> divideSubGraphByCenters(const vector<Hash>& centers, int maxNodes,
        const function<int(uint32_t/*edge weight*/, bool/*whether black*/, uint32_t/*depth*/)>& weightFunction,
        const vector<Hash>& exceptNodes = {}) const {
        /*
         * @weightFunction (
         *      @1 uint32_t: edge weight
         *      @2 bool:     whether black
         *      @3 uint32_t: depth
         * )
         */
        // make some tests
        for(const auto& h: centers){
            assert(raw_to_mapped.find(h) != raw_to_mapped.cend());
        }
        for(const auto& h: exceptNodes){
            assert(raw_to_mapped.find(h) != raw_to_mapped.cend());
        }
        vector<tuple<NodeIdType, DepthType>> depthInput;
        depthInput.reserve(centers.size()+exceptNodes.size());
        auto cmp = [](const tuple<uint32_t, int>&a, const tuple<uint32_t, int>&b)->bool{
            return get<1>(a) > get<1>(b);
        };
        priority_queue<tuple<uint32_t, int>, vector<tuple<uint32_t, int>>, decltype(cmp)> bfsQueue(cmp);
        for(const auto& center: centers){
            uint32_t mappedId = raw_to_mapped.at(center);
            depthInput.emplace_back(make_tuple(mappedId, 0U));
            bfsQueue.emplace(make_tuple(mappedId, -100000));
        }
        for(const auto& exceptNode: exceptNodes){
            uint32_t mapped_id = raw_to_mapped.at(exceptNode);
            depthInput.emplace_back(make_tuple(mapped_id, (uint32_t)1e5));
        }
        auto depth = getDepth(depthInput);
        set<uint32_t> resultSet;
        while (resultSet.size() < maxNodes){
            auto [nodeID, _] = bfsQueue.top();
            bfsQueue.pop();
            if(resultSet.insert(nodeID).second){
                auto edge = edges.at(nodeID);
                for(const auto& n: edge){
                    bfsQueue.emplace(make_tuple(n.to,
                        weightFunction(n.weight, mapped_to_black.at(n.to).empty(), depth.at(n.to))));
//                    -n.weight+ 10 * mapped_to_black.at(n.to).empty() + (int)25*depth[n.to];
//                    bfsQueue.emplace(make_tuple(n.to, -n.weight + (int)25*depth[n.to]));
                }
            }
        }
        vector<Hash> result;
        result.reserve(resultSet.size());
        for(auto nodeID: resultSet){
            result.emplace_back(mapped_to_raw.at(nodeID));
        }
        return result;
    }
    [[nodiscard]] vector<Hash> divideSubGraphByDepth(const vector<Hash>& centers, int maxDepth) const {
        vector<tuple<NodeIdType, DepthType>> depthInput;
        depthInput.reserve(centers.size());
        for(const auto& center: centers){
            uint32_t mappedId = raw_to_mapped.at(center);
            depthInput.emplace_back(make_tuple(mappedId, 0U));
        }
        auto depth = getDepth(depthInput);
        vector<Hash> result;
        for(uint32_t nodeID = 0; nodeID<depth.size(); nodeID++){
            if(depth[nodeID] <= maxDepth){
                result.emplace_back(mapped_to_raw.at(nodeID));
            }
        }
        return result;
    }
	[[nodiscard]] vector<vector<Hash>> divideSubGraph() const {
        vector<vector<uint32_t>> dfsGroup;
        vector<uint32_t> dfsVers;
        function<void(uint32_t, uint32_t)> _dfsDivide =
                [&dfsVers, &dfsGroup, &_dfsDivide, this](uint32_t v, uint32_t verNum){
            dfsVers[v] = verNum;
            dfsGroup[verNum].push_back(v);
            for(const auto& edge: edges.at(v)){
                if(edge.weight <= 1) continue;
                uint32_t u = edge.to;
                if(dfsVers[u] == UNMATCHED){
                    _dfsDivide(u, verNum);
                }
            }
        };
	    uint32_t verNum = 0;
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
	            subGraph.emplace_back(mapped_to_raw.at(v));
	        }
	        subGraphs.emplace_back(move(subGraph));
	    }
        return subGraphs;
	}
    [[nodiscard]] double getNeighborBlack (uint32_t nodeID) const {
        const auto& edge = edges.at(nodeID);
        size_t totalCount = 0;
        size_t blackCount = 0;
        for(const auto& e: edge){
            if(mapped_to_assetType.at(e.to) != DOMAIN_ASS) continue;
            totalCount += 1;
            const auto& black = mapped_to_black.at(e.to);
            if(!black.empty()) blackCount += 1;
        }
        if(totalCount == 0) return 0;
        return double(blackCount) / totalCount;
    }
    [[nodiscard]] vector<Hash> getBorderNodes()const{
        vector<Hash> result;
        for(const auto& nodeBlack: mapped_to_black){
            auto nodeID = nodeBlack.first;
            const auto& nodeBlackList = nodeBlack.second;
//            if(mapped_to_assetType.at(nodeID) != DOMAIN_ASS) continue;
//            if(!nodeBlackList.empty()) continue;
            if(edges.at(nodeID).size() <= 1){
                result.emplace_back(mapped_to_raw.at(nodeID));
            }
        }
        cout << "border nodes size = " << result.size() << endl;
        return result;
    }
    [[nodiscard]] int64_t getMaxFlow(uint32_t mapped_from, uint32_t mapped_to) const {
        assert(mapped_from != mapped_to);
        static const constexpr int64_t MAX_LLD = numeric_limits<int64_t>::max();
        uint32_t n = numNodes();
        uint32_t m = numEdges();
        uint32_t s = mapped_from;
        uint32_t t = mapped_to;
        vector<bool> isVisit;
        vector<uint32_t> depth, to, next, first;
        vector<int64_t> weight;
        isVisit.resize(n+1, false);
        depth.resize(n+1, 0);
        to.reserve(2*m);
        next.reserve(2*m);
        first.resize(n+1, UNMATCHED);
        weight.reserve(2*m);

        auto addEdge = [&to, &weight, &first, &next]
                (uint32_t u, uint32_t v, int64_t w){
            next.emplace_back(first[u]);
            first[u] = to.size();
            to.emplace_back(v);
            weight.emplace_back(w);
        };
        const function<bool()> bfs = [&to, &weight, &first, &next, &depth, n, s, t]()->bool{
            uint32_t l, r;
            vector<uint32_t> q;
            fill(depth.begin(), depth.end(), 0);
            q.resize(n+1, 0);
            depth[s] = 1;
            q[l=r=1] = s;
            while(l<=r){
                uint32_t u=q[l++];
                for(uint32_t p = first[u]; p != UNMATCHED; p = next[p]) {
                    uint32_t v = to[p];
                    if(weight[p] and !depth[v]) {//按照有残量的边搜过去
                        depth[v] = depth[u] + 1;
                        q[++r] = v;
                    }
                }
            }
            return depth[t];
        };
        const function<int64_t(uint32_t , int64_t)> dfs =
                [&dfs, &weight, &first, &next, &to, &depth, n, s, t](uint32_t u, int64_t in_w)-> int64_t {
            if(u == t) return in_w;
            int64_t out_w = 0;
            for(uint32_t p = first[u]; p != UNMATCHED and in_w; p = next[p]) {
                uint32_t v = to[p];
                if(weight[p] and depth[v] == depth[u] + 1) {
                    int64_t res = dfs(v, min(weight[p], in_w));
                    weight[p] -= res;
                    weight[p ^ 1] += res;
                    in_w -= res;
                    out_w += res;
                }
            }
            if(out_w == 0) depth[u] = 0;
            return out_w;
        };
        for(const auto& edge: edges){
            for(const auto& e: edge.second){
                addEdge(e.from, e.to, e.weight);
                addEdge(e.to, e.from, 0);
            }
        }
        int64_t ans = 0;
        while(bfs()){
            ans += dfs(s, MAX_LLD);
        }
        return ans;
    }
    static vector<uint32_t> concatPath(const vector<uint32_t>& path1, const vector<uint32_t>& path2){
        if(path1.empty()) return path2;
        if(path2.empty()) return path1;
        assert(path1.back() == path2.at(0));
        vector<uint32_t> result;
        result.insert(result.end(), path1.cbegin(), path1.cend());
        result.insert(result.end(), path2.cbegin()+1, path2.cend());
        return result;
    }
    static vector<Hash> concatPath(const vector<Hash>& path1, const vector<Hash>& path2){
        if(path1.empty()) return path2;
        if(path2.empty()) return path1;
        assert(path1.back() == path2.at(0));
        vector<Hash> result;
        result.insert(result.end(), path1.cbegin(), path1.cend());
        result.insert(result.end(), path2.cbegin()+1, path2.cend());
        return result;
    }
};


#endif