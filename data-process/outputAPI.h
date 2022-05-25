#ifndef _OUTPUTAPI_H_
#define _OUTPUTAPI_H_
#include "parser.h"
#include <regex>
#include <json/json.h>

[[nodiscard]]string graphToJson_echarts(const vector<LinkItemType>& links, const map<Hash, ItemType>& nodes,
    const set<Hash>& centers){
    // string relation; ItemType from, to;
    map<Hash, uint32_t> raw_to_mapped;
    Json::Value root_json = Json::ValueType::objectValue;
    Json::Value categories_json = Json::ValueType::arrayValue;
    Json::Value nodes_json = Json::ValueType::arrayValue;
    Json::Value links_json = Json::ValueType::arrayValue;
    uint32_t count = 0;
    for(auto iter = nodes.cbegin(); iter != nodes.cend(); iter++){
        const Hash h = iter->first;
        raw_to_mapped[h] = count;
        Json::Value node_json;
        node_json["name"] = "["+iter->second.type+"]: "+iter->second.name;
        node_json["value"] = 1;
        if(centers.find(h) != centers.cend()){
            // root
            node_json["category"] = 0;
        } else if(iter->second.industry.empty()) {
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
        vector<string> cats = {"root", "normal", "black"};
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