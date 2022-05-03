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
using namespace std;
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
void outputSubGraph(const map<Hash, ItemType>& nodes, const vector<LinkItemType>& linkItems, const vector<vector<Hash>>& subGraphs){
    DEBUG
    map<Hash, vector<LinkItemType>> links;
    for(const LinkItemType& linkItem: linkItems){
        Hash from_hash = get<1>(linkItem);
        auto pos = links.find(from_hash);
        if(pos == links.end()){
            links.emplace(from_hash, vector<LinkItemType>{linkItem});
        }
        else{
            pos->second.emplace_back(linkItem);
        }
    }
    int count = 0;
    for(const vector<Hash>& subGraph: subGraphs){
        {// output Node.csv
            ofstream f;
            stringstream ss;
            ss << "./output/" << setw(8) << setfill('0') << count << "-Node-" << setw(8) << setfill('0') << subGraph.size() <<".csv";
            f.open(ss.str(), ios::out);
            if(!f.good()){
                cerr << "try to write '" << ss.str() << "', but failed!\n";
                exit(1);
            }
            f << "id,name,type,industry\n";
            for(const Hash& h: subGraph){
                const ItemType& item = nodes.at(h);
                outputItemType(f, item) << '\n';
            }
            f.close();
        }
        {
            ofstream f;
            stringstream ss;
            ss << "./output/" << setw(8) << setfill('0') << count << "-Link-"
                << setw(8) << setfill('0') << subGraph.size() <<".csv";
            f.open(ss.str(), ios::out);
            if(!f.good()){
                cerr << "try to write '" << ss.str() << "', but failed!\n";
                exit(1);
            }
            f << "relation,source,target\n";
            set<Hash> subNodes;
            subNodes.insert(subGraph.cbegin(), subGraph.cend());
            for(const Hash& h: subGraph){
                auto pos = links.find(h);
                if(pos == links.cend()){
                    continue;
                }
                const vector<LinkItemType>& l = pos->second;
                for(const LinkItemType& li: l){
                    if(subNodes.find(get<2>(li)) == subNodes.cend()){
                        continue;
                    }
                    f << get<0>(li) << ',' << nodes.at(get<1>(li)).id_str << ','
                        << nodes.at(get<2>(li)).id_str << '\n';
                }
            }
            f.close();
        }
        count++;
    }
}
constexpr const uint32_t UNMATCHED = -1;
class Graph{
private:
    map<Hash, vector<Edge<Hash>> > edges;// edge<T> 存的[结点原始编号]
    set<Hash> raw_node;// 存放所有结点,存的[结点原始编号]
	map<Hash, uint32_t> raw_to_mapped;// [结点原始编号] -> [连续化结点编号]
	map<uint32_t, Hash> mapped_to_raw;// [连续化结点编号] -> [结点原始编号]

	vector<int> nodeDegreeWeight;
public:
    Graph(){}
    [[nodiscard]]size_t numNodes(){
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

private:
    vector<bool> _dfsAnalyseVisit;
    array<vector<NodeDegreeType>, 5> _analyseArrangement;
    void _dfsAnalyse(uint32_t root, int depth){
        if(depth >= _analyseArrangement.size() || _dfsAnalyseVisit[root]) return;
        if(nodeDegreeWeight[root] <= 10) return;
        if(depth>=3 && nodeDegreeWeight[root] <= 20) return;
        _dfsAnalyseVisit[root] = true;
        _analyseArrangement[depth].push_back(NodeDegreeType(root, nodeDegreeWeight[root]));
        const vector<Edge<Hash>>& rootEdges = edges[mapped_to_raw[root]];
        for(const auto& edge: rootEdges){
            uint32_t u = raw_to_mapped[edge.to];
            _dfsAnalyse(u, depth+1);
        }
    }

public:
	vector<NodeDegreeType> analyseDegree(){
        vector<NodeDegreeType> nodeWeights;
        nodeWeights.reserve(numNodes());
        _dfsAnalyseVisit.clear();
        _dfsAnalyseVisit.resize(numNodes(), false);
        for(uint32_t id=0; id< nodeDegreeWeight.size(); id++){
            nodeWeights.emplace_back(NodeDegreeType(id, nodeDegreeWeight[id]));
        }
        sort(nodeWeights.begin(), nodeWeights.end(), greater<NodeDegreeType>());
//        cout << "Analyse all!" << endl;
//        for(NodeDegreeType n: nodeWeights){
//            cout << n.degreeWeight << ' ';
//        }
//        cout << endl;
//        return;

        cout << "Analyse root!" << endl;
        uint32_t root = nodeWeights[0].mapped_id;
        _dfsAnalyse(root, 0);
        for(uint32_t depth=0; depth<_analyseArrangement.size(); depth++){
            cout << "=================================== " << "depth = " << depth << " ===================================" << endl;
            vector<NodeDegreeType>& arrange = _analyseArrangement[depth];
            sort(arrange.begin(), arrange.end(), greater<NodeDegreeType>());
            for(NodeDegreeType n: arrange){
                cout << n.degreeWeight << ' ';
            }
            cout << endl;
        }
        return nodeWeights;
	}
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