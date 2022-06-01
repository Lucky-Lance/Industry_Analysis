#ifndef _OUTPUTAPI_H_
#define _OUTPUTAPI_H_
#include "parser.h"
#include <regex>
#include <json/json.h>
[[nodiscard]]string graphToJson_cwd(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes,
             const vector<vector<Hash>>& paths,
             const map<Hash, tuple<uint32_t, NodeAssetType, uint32_t>>& nodeStatistics){
    // check legal
    {
        for(const auto& p: paths){
            for(const Hash& h: p){
                assert(nodes.find(h) != nodes.cend());
            }
        }
    }
    map<Hash, uint32_t> raw_to_mapped;
    vector<map<uint32_t, uint32_t>> mapped_mapped_to_mapped_edge;
    mapped_mapped_to_mapped_edge.resize(nodes.size(), {});
    Json::Value root_json;
    {// nodes
        Json::Value nodes_json = Json::ValueType::objectValue;
        uint32_t count = 0;
        const static map<uint32_t,string> reachUint_to_colorString = {
                {0, /*"Top"*/"#dc143c" }, {1, /*"Mid"*/"#6495ed"}, {2, /*"Bot"*/"#0000cd"},
        };
        const static map<uint32_t, uint32_t> reachUint_to_nodeSize = {
                {0, /*"Top"*/16 }, {1, /*"Mid"*/13}, {2, /*"Bot"*/10},
        };
        for (auto iter = nodes.cbegin(); iter != nodes.cend(); iter++) {
            const Hash h = iter->first;
            raw_to_mapped[h] = count;
            count++;
            Json::Value nodeItem_json;
            uint32_t nodeReach = get<2>(nodeStatistics.at(h));
            nodeItem_json["name"] = "N" + to_string(count);
            nodeItem_json["icon"] = AssetTypeToIcon.at(iter->second.typeClass);
            nodeItem_json["size"] = reachUint_to_nodeSize.at(nodeReach);
            nodeItem_json["color"] = reachUint_to_colorString.at(nodeReach);

            nodes_json["node" + to_string(count)] = move(nodeItem_json);
        }
        root_json["nodes"] = move(nodes_json);
    }
    {// edges
        Json::Value edges_json = Json::ValueType::objectValue;
        uint32_t count = 0;
        for (const auto& edge: links) {
            uint32_t sourceID = raw_to_mapped.at(get<1>(edge));
            uint32_t targetID = raw_to_mapped.at(get<2>(edge));
            mapped_mapped_to_mapped_edge.at(sourceID)[targetID] = count;
            mapped_mapped_to_mapped_edge.at(targetID)[sourceID] = count;
            count++;
            Json::Value edgeItem_json;
            edgeItem_json["source"] = "node"+ to_string(sourceID+1);
            edgeItem_json["target"] = "node"+ to_string(targetID+1);
            edgeItem_json["label"] = to_string(sourceID+1) + '-' + to_string(targetID+1);
            edgeItem_json["width"] = 3;// TODO
            edgeItem_json["color"] = "#d8d8d8";
            edges_json["edge"+ to_string(count)] = move(edgeItem_json);
        }
        root_json["edges"] = move(edges_json);
    }
    {// paths
        Json::Value paths_json=Json::ValueType::objectValue;
        uint32_t count = 0;
        for(const auto& path: paths){
            count++;
            Json::Value pathn_json=Json::ValueType::objectValue;
            Json::Value pathItem_json=Json::ValueType::objectValue;
            // pathEdges_json
            Json::Value pathEdges_json=Json::ValueType::arrayValue;
            assert(path.size()>1);
            for(size_t i=0; i<path.size()-1; i++){
                uint32_t sourceID = raw_to_mapped.at(path[i]);
                uint32_t targetID = raw_to_mapped.at(path[i+1]);
                uint32_t edgeID = mapped_mapped_to_mapped_edge.at(sourceID).at(targetID);
                pathEdges_json.append("edge"+ to_string(edgeID+1));
            }
            pathItem_json["edges"] = move(pathEdges_json);
            pathItem_json["active"] = false;
            pathItem_json["width"] = 10;
            paths_json["path"+to_string(count)] = move(pathItem_json);
        }
        root_json["paths"] = move(paths_json);
    }
    {// pathNodes
        Json::Value pathNodes_json= Json::ValueType::arrayValue;
        for(const auto& path: paths){
            Json::Value pNodes_json = Json::ValueType::arrayValue;
            for(const auto& h: path){
                uint32_t nodeID = raw_to_mapped.at(h);
                pNodes_json.append("node" + to_string(nodeID+1));
            }
            pathNodes_json.append(move(pNodes_json));
        }
        root_json["pathNodes"] = move(pathNodes_json);
    }
    {// layouts
        Json::Value layouts_json = Json::ValueType::objectValue;
        root_json["layouts"] = move(layouts_json);
    }
    {// nodesProp
        Json::Value nodesProp_json = Json::ValueType::objectValue;
        const static map<uint32_t,string> uint_to_reachString = {
                {0, "Top"}, {1, "Mid"}, {2, "Bot"},
        };
        for(auto iter=nodes.cbegin(); iter!=nodes.cend(); iter++){
            Hash h = iter->first;
            Json::Value nodesPropItem_json = Json::ValueType::objectValue;
            uint32_t nodeID = raw_to_mapped.at(h);
            auto nodeStatistic = nodeStatistics.at(h);
            nodesPropItem_json["degree"] = get<0>(nodeStatistic);
            nodesPropItem_json["type"] = AssetTypeToString.at(get<1>(nodeStatistic));
            nodesPropItem_json["pos"] = uint_to_reachString.at(get<2>(nodeStatistic));
            nodesProp_json["node"+ to_string(nodeID+1)] = move(nodesPropItem_json);
        }
        root_json["nodesProp"] = move(nodesProp_json);
    }
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, root_json);
}
[[nodiscard]]string graphToJson_echarts(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes,
    const set<Hash>& centers, const set<Hash>& exceptNodes = {}, const set<Hash>& clueNodes = {},
    const set<Hash>& paths = {}){
    // do some test
    {
        for(const auto& h: centers) assert(nodes.find(h) != nodes.cend());
        for(const auto& h: exceptNodes) assert(nodes.find(h) != nodes.cend());
        for(const auto& h: clueNodes) assert(nodes.find(h) != nodes.cend());
        for(const auto& h: paths) assert(nodes.find(h) != nodes.cend());
    }
    map<Hash, uint32_t> raw_to_mapped;
    Json::Value root_json = Json::ValueType::objectValue;
    Json::Value categories_json = Json::ValueType::arrayValue;
    Json::Value nodes_json = Json::ValueType::arrayValue;
    Json::Value links_json = Json::ValueType::arrayValue;
    uint32_t count = 0;
    for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
        const Hash h = iter->first;
        raw_to_mapped[h] = count;
        auto typeClass = nodes.find(h)->second.typeClass;
        Json::Value node_json;
        if(iter->second.typeClass == IP_ASS || iter->second.typeClass == PHONE_ASS){
            node_json["name"] = "[" + iter->second.type + "]: " + iter->second.id_str;
        }
        else {
            node_json["name"] = "[" + iter->second.type + "]: " + iter->second.name;
        }
        node_json["value"] = 1;
        if(centers.find(h) != centers.cend()){
            // root
            node_json["category"] = 0;
        } else if(exceptNodes.find(h) != exceptNodes.cend()){
            clog << h.str << endl;
            node_json["category"] = 3;
        } else if(clueNodes.find(h) != clueNodes.cend()){
            node_json["category"] = 4;
        }else if(paths.find(h) != paths.cend()){
            node_json["category"] = 6;
        }else if(typeClass == IP_ASS || typeClass == CERT_ASS){
            node_json["category"] = 5;
        }
        else if(iter->second.industry.empty()) {
            // normal
            node_json["category"] = 1;
        } else{
            // black
            node_json["category"] = 2;
        }
        nodes_json.append(move(node_json));
        count++;
    }
    for(const LinkItemType& link: links){
        const auto& [relation, from, to] = link;
        uint32_t from_id = raw_to_mapped[get<1>(link)];
        uint32_t to_id = raw_to_mapped[get<2>(link)];
        Json::Value link_json = Json::ValueType::objectValue;
        link_json["source"] = from_id;
        link_json["target"] = to_id;
        links_json.append(move(link_json));
    }
    {// categories
        vector<string> cats = {"root", "normal", "black", "excepts", "clues", "ip or cert", "path node"};
        for(const auto &cat_name : cats){
            Json::Value cat = Json::ValueType::objectValue;
            Json::Value empty = Json::ValueType::objectValue;
            cat["name"] = cat_name;
            cat["keyword"] = empty;
            cat["base"] = "graph";
            categories_json.append(cat);
        }

    }
    root_json["type"] = "force";
    root_json["categories"] = move(categories_json);
    root_json["nodes"] = move(nodes_json);
    root_json["links"] = move(links_json);
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, root_json);
}
[[nodiscard]]string graphToJson_lxd(const vector<array<uint32_t, 8>>& statistics){
    uint32_t totalCount = 0;
    for(const auto s: statistics){
        totalCount += s.at(0);
    }
    Json::Value freqData = Json::ValueType::arrayValue;
    uint32_t sizeRemain = 0, prevDegree = 0;
    array<uint32_t, 8> s = {};
    for(uint32_t degree = 0; degree < statistics.size(); degree++){
        sizeRemain += statistics.at(degree).at(0);
        for(size_t i=0; i<s.size(); i++){
            s.at(i) += statistics.at(degree).at(i);
        }
        if(sizeRemain > totalCount / 5 || degree + 1 == statistics.size()){
            Json::Value freqItem = Json::ValueType::objectValue;
            freqItem["State"] = to_string(prevDegree) + " - " + to_string(degree);
            freqItem["Count"] = sizeRemain;
            {
                Json::Value freq = Json::ValueType::objectValue;
                freq["Domain"] = s[1];
                freq["IP"] = s[2];
                freq["Cert"] = s[3];
                freq["Other"] = s[4];
                freqItem["freq"] = freq;
            }
            {
                Json::Value area = Json::ValueType::objectValue;
                area["Top"] = s[5];
                area["Mid"] = s[6];
                area["Bot"] = s[7];
                freqItem["area"] = area;
            }
            freqData.append(freqItem);
            prevDegree = degree+1;
            sizeRemain = 0;
            s = {};
        }
    }
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, freqData);
}
[[nodiscard]]string graphToJson_cwh(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes){
    // string relation; ItemType from, to;
    map<Hash, uint32_t> raw_to_mapped;
    Json::Value root_json = Json::ValueType::objectValue;
    Json::Value categories_json = Json::ValueType::arrayValue;
    Json::Value nodes_json = Json::ValueType::arrayValue;
    Json::Value links_json = Json::ValueType::arrayValue;
    uint32_t count = 0;
    // build graph
    for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
        const Hash h = iter->first;
        raw_to_mapped[h] = count;
        Json::Value node_json;
        node_json["name"] = to_string(count);
        node_json["value"] = 1;
        if(iter->second.industry.empty()) {
            node_json["category"] = 0;
        } else{
            node_json["category"] = 1;
        }
        nodes_json.append(move(node_json));
        count++;
    }
    for(const LinkItemType& link: links){
        const auto& [relation, from, to] = link;
        uint32_t from_id = raw_to_mapped[get<1>(link)];
        uint32_t to_id = raw_to_mapped[get<2>(link)];
        Json::Value link_json = Json::ValueType::objectValue;
        link_json["source"] = from_id;
        link_json["target"] = to_id;
        links_json.append(move(link_json));
    }
    {// categories
        vector<string> cats = {"normal", "black"};
        for(const auto &cat_name : cats){
            Json::Value cat = Json::ValueType::objectValue;
            Json::Value empty = Json::ValueType::objectValue;
            cat["name"] = cat_name;
            cat["keyword"] = empty;
            cat["base"] = "graph";
            categories_json.append(cat);
        }

    }
    root_json["type"] = "force";
    root_json["categories"] = move(categories_json);
    root_json["nodes"] = move(nodes_json);
    root_json["links"] = move(links_json);
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, root_json);
}
[[nodiscard]] tuple<uint32_t,uint32_t,uint32_t> subgraphCount_lxd(const map<Hash, ItemType>& nodes){
    map<string, uint32_t &> typeToItem;
    uint32_t certCount = 0, ipCount = 0, domainCount = 0;
    typeToItem.insert(pair<string, uint32_t &>("Cert", certCount));
    typeToItem.insert(pair<string, uint32_t &>("IP", ipCount));
    typeToItem.insert(pair<string, uint32_t &>("Domain", domainCount));
    for(const auto& node: nodes){
        auto ptr = typeToItem.find(node.second.type);
        assert(ptr != typeToItem.end());
        (ptr->second) += 1;
    }
    return make_tuple(certCount, ipCount, domainCount);
}
[[nodiscard]] tuple<string, string> graphToJson_lxd(string folderName){
    // TODO:
    assert(!folderName.empty());
    if(folderName.back() != '/' || folderName.back() != '\\'){
        folderName += '/';
    }
    auto folderPath = filesystem::path(folderName);
    regex nodeRegex("(\\d{8})-Node-(\\d{8}).csv");
    uint32_t count = 0;
    Json::Value nodes_json = Json::ValueType::arrayValue;
    Json::Value links_json = Json::ValueType::arrayValue;
    for(auto& file: filesystem::directory_iterator(folderPath)){
        auto fileName = file.path().string();
        smatch matches;
        if(regex_match(fileName, matches, nodeRegex)){
            assert(matches.size() == 3);
            count++;
            int graphID = stoi(matches[1]);
            int graphSize = stoi(matches[2]);
            stringstream linkNameSS, nodeNameSS;
            linkNameSS << folderName << setw(8) << graphID << "-Link-"
                << graphSize << ".csv";
            nodeNameSS << folderName << setw(8) << graphID << "-Node-"
                << graphSize << ".csv";
            vector<LinkItemType> links = linkReader(linkNameSS.str());
            map<Hash, ItemType> nodes  = nodeReader(nodeNameSS.str());
            auto [certCount, ipCount, domainCount] = subgraphCount_lxd(nodes);
            map<Hash, uint32_t> raw_to_mapped;
            Json::Value node_json = Json::ValueType::objectValue;
            Json::Value link_json = Json::ValueType::objectValue;
            for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
                const Hash h = iter->first;
                raw_to_mapped[h] = count;
                Json::Value node_json;
                node_json["cert"] = certCount;
                node_json["ip"] = ipCount;
                node_json["domain"] = domainCount;
                node_json["category"] = 0;
                nodes_json.append(move(node_json));
            }
            for(const LinkItemType& link: links){
                const auto& [relation, from, to] = link;
                uint32_t from_id = raw_to_mapped[get<1>(link)];
                uint32_t to_id = raw_to_mapped[get<2>(link)];
                Json::Value link_json = Json::ValueType::objectValue;
                link_json["source"] = from_id;
                link_json["target"] = to_id;
                links_json.append(move(link_json));
            }
            {
                Json::Value cat01 = Json::ValueType::objectValue;
                Json::Value empty = Json::ValueType::objectValue;
                cat01["name"] = "subgraph01";
                cat01["keyword"] = empty;
                cat01["base"] = "graph";
            }
        }
    }
    const vector<LinkItemType> links; const map<Hash, ItemType> nodes;

    Json::StreamWriterBuilder builder1, builder2;
    return make_tuple(Json::writeString(builder1, nodes_json),
                      Json::writeString(builder2, links_json));
}
#endif