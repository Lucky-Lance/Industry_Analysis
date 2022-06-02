import json
from math import ceil

mapping = {
    "A": "黄色",
    "B": "赌博",
    "C": "诈骗",
    "D": "贩毒",
    "E": "贩枪",
    "F": "黑客",
    "G": "非法交易平台",
    "H": "非法支付平台",
    "I": "其他"
}

def get_abstract_data(file_name):
    with open(file_name) as f:
        all_data = json.load(f)

    abstract_data = {}
    
    sizes = sorted([len(all_data[graph_id]["nodes"]) for graph_id in all_data])
    rank = {size: i for i, size in enumerate(sizes)}

    for graph_id in all_data:
        cur_graph = {
            "nodeNum": 0,
            "edgeNum": 0,
            "size": 0,
            "darkPercent": 0,
            "corePercent": 0,
            "coreService": "",
            "type": ""
        }

        cur_graph["nodeNum"] = len(all_data[graph_id]["nodes"])
        cur_graph["edgeNum"] = len(all_data[graph_id]["edges"])
        cur_graph["size"] = ceil((rank[cur_graph["nodeNum"]] + 1) / len(all_data) * 100)

        props = all_data[graph_id]["nodesProp"]
        num_dark, num_core = 0, 0
        dark = {}
        for node in props.values():
            if node["black"] != "":
                num_dark += 1
            if node["pos"] == "Top":
                num_core += 1
            for c in node["black"]:
                dark[c] = dark.get(c, 0) + 1
            
        cur_graph["darkPercent"] = ceil(num_dark / cur_graph["nodeNum"] * 100)
        cur_graph["corePercent"] = ceil(num_core / cur_graph["nodeNum"] * 1000)

        if cur_graph["nodeNum"] < 200:
            cur_graph["type"] = "小型"
        elif cur_graph["nodeNum"] < 600:
            cur_graph["type"] = "中型"
        else:
            cur_graph["type"] = "大型"
        
        dark = [(k, v) for k, v in dark.items()]
        dark = sorted(dark, key=lambda x: - x[1])

        cur_graph["coreService"] = "，".join([mapping[x[0]] for x in dark[:3]])
        if cur_graph["coreService"] == "":
            cur_graph["coreService"] = "未知"
        
        abstract_data[graph_id] = cur_graph

    return abstract_data


if __name__ == "__main__":
    a = get_abstract_data("./all_data.json")