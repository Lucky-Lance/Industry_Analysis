#ifndef _RANDOM_OUTPUT_API_H_
#define _RANDOM_OUTPUT_API_H_

#include <json/json.h>
#include <iostream>
#include <string>
#include <random>
#include <assert.h>
#include <algorithm>
using namespace std;

[[nodiscard]] string random_graphToJson_cwh(uint32_t graphSize){
    assert(graphSize > 1);
    default_random_engine randomEngine;
    static const vector<string> icons = {
            "&#xe320", "&#xe328", "&#xe331", "&#xe2bd", "&#xf0e2", "&#xea75"
    };
    uniform_int_distribution<int> iconSampler(0, icons.size()-1);
    Json::Value root_json = Json::ValueType::objectValue;

    {// nodes
        Json::Value nodes_json = Json::ValueType::objectValue;
        for (uint32_t i = 1; i <= graphSize; i++) {
            Json::Value node_json = Json::ValueType::objectValue;
            string keyName = "node" + to_string(i);
            string name = "N" + to_string(i);
            string icon = icons[iconSampler(randomEngine)];
            node_json["name"] = name;
            node_json["icon"] = icon;
            nodes_json[keyName] = node_json;
        }
        root_json["nodes"] = nodes_json;
    }

    vector<int> nextNode, nodeDepth;
    {// edges
        Json::Value edges_json = Json::ValueType::objectValue;
        nextNode.resize(graphSize + 1, 0);
        nodeDepth.resize(graphSize + 1, 0);
        nodeDepth[1] = 1;
        for (uint32_t source = 2; source <= graphSize; source++) {
            Json::Value edge_json = Json::ValueType::objectValue;
            string keyName = "edge" + to_string(source);
            string sourceName = "node" + to_string(source);
            uint32_t target = 1 + randomEngine() % (source - 1);
            nextNode[source] = target;
            nodeDepth[source] = nodeDepth[target] + 1;
            string targetName = "node" + to_string(target);
            edge_json["source"] = sourceName;
            edge_json["target"] = targetName;
            edge_json["label"] = to_string(source) + '-' + to_string(target);
            edges_json[keyName] = edge_json;
        }
        root_json["edges"] = edges_json;
    }

    {// paths and pathNodes
        Json::Value pathNodes_json = Json::ValueType::objectValue;
        Json::Value paths_json = Json::ValueType::objectValue;
        Json::Value path1Nodes_json = Json::ValueType::arrayValue;
        Json::Value path1_json = Json::ValueType::objectValue;
        path1_json["width"] = 0;
        Json::Value path1_edges_json = Json::ValueType::arrayValue;
        uint32_t deepestNode = max_element(nodeDepth.cbegin(), nodeDepth.cend()) - nodeDepth.cbegin();
        for (uint32_t currentNode = deepestNode; currentNode != 1U; currentNode = nextNode[currentNode]) {
            path1Nodes_json.append("node" + to_string(currentNode));
            path1_edges_json.append("edge" + to_string(currentNode));
        }
        path1Nodes_json.append("node" + to_string(1));
        pathNodes_json["path1"] = path1Nodes_json;
        path1_json["edges"] = path1_edges_json;
        paths_json["path1"] = path1_json;
        root_json["paths"] = paths_json;
        root_json["pathNodes"] = pathNodes_json;
    }
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, root_json);
}

#endif