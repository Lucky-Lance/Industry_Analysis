import json

def get_right_down_data(file_name):
    with open(file_name) as f:
        all_data = json.load(f)

    right_down_data = {}

    for graph_id in all_data:
        props = all_data[graph_id]["nodesProp"]

        # all_type = set([node["type"] for name, node in props.items()])
        all_type = ["Domain", "IP", "Cert", "ASN"]
        print("all type:", all_type)
        
        degrees = [node["degree"] if node["type"] in all_type else 0
                             for name, node in props.items()]
        degrees = sorted(filter(lambda x: x != 0, degrees))

        max_degree = degrees[int(len(degrees) * 0.95)]
        print("max degree:", max_degree)

        # num_bins = 5 if max_degree < 20 else 10
        num_bins = 10

        gap = max_degree // num_bins + 1

        cur_graph = {}

        # freqData
        cur_graph["freqData"] = []
        for i in range(num_bins):
            cur_graph["freqData"].append({
                "State": f"{gap*i} - {gap*(i+1)}" if i != num_bins - 1 else f"> {gap*i}",
                "Count": 0,
                "freq": {
                    type: 0 for type in all_type
                },
                "area": {
                    "Top": 0,
                    "Mid": 0,
                    "Bot": 0
                }
            })

        # type2field
        cur_graph["type2field"] = {
            type : {
                "Top": 0,
                "Mid": 0,
                "Bot": 0
            } for type in all_type
        }

        # field2type
        cur_graph["field2type"] = {
            pos : {
                type: 0 for type in all_type
            } for pos in ["Top", "Mid", "Bot"]
        }
        
        for node in props.values():
            bucket = min(node["degree"] // gap, num_bins - 1)
            type = node["type"]
            pos = node["pos"]

            # filter
            cur_graph["freqData"][bucket]["Count"] += 1
            if type in all_type:
                cur_graph["freqData"][bucket]["freq"][type] += 1
            cur_graph["freqData"][bucket]["area"][pos] += 1

            if type in all_type:
                cur_graph["type2field"][type][pos] += 1
                cur_graph["field2type"][pos][type] += 1

        right_down_data[graph_id] = cur_graph
    
    return right_down_data


if __name__ == "__main__":
    a = get_right_down_data("./all_data.json")