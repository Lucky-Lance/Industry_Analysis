import json
import pprint

def get_main_data():

    all_data = json.load(open("./data/all_data.json", "r"))

    data_main = {}
    data_main["allNodes"] = []
    data_main["allEdges"] = []

    for key, value in all_data.items():

        nodesProp = value['nodesProp']
        cert, ip, domain, ASN = 0, 0, 0, 0
        key = int(key)
        industry = "Industry" + str(key + 1)
        for nodeIdx in nodesProp:
            type = nodesProp[nodeIdx]['type']
            if type == "Domain":
                domain = domain + 1
            elif type == "IP":
                ip = ip + 1
            elif type == "Cert":
                cert = cert + 1
            elif type == "ASN":
                ASN = ASN + 1
        
        data_main["allNodes"].append({
            "id": 5 * key + 1,
            "industry": industry,
            "type": "Parent",
            "pid": 0,
            "subids": [5 * key + 2, 5 * key + 3, 5 * key + 4, 5 * key + 5],
            "group": key,
            "label": industry
        })

        data_main["allNodes"].append({
            "id": 5 * key + 2,
            "industry": industry,
            "type": "cert",
            "value": str(cert),
            "pid": 5 * key + 1,
            "group": key,
            "label": "cert"
        })

        data_main["allEdges"].append({
            "id": str(5 * key + 1) + "_" + str(5 * key + 2),
            "from" : 5 * key + 1,
            "to": 5 * key + 2,
        })

        data_main["allNodes"].append({
            "id": 5 * key + 3,
            "industry": industry,
            "type": "ip",
            "value": str(ip),
            "pid": 5 * key + 1,
            "group": key,
            "label": "ip"
        })

        data_main["allEdges"].append({
            "id": str(5 * key + 1) + "_" + str(5 * key + 3),
            "from" : 5 * key + 1,
            "to": 5 * key + 3,
        })

        data_main["allNodes"].append({
            "id": 5 * key + 4,
            "industry": industry,
            "type": "domain",
            "value": str(domain),
            "pid": 5 * key + 1,
            "group": key,
            "label": "domain"
        })

        data_main["allEdges"].append({
            "id": str(5 * key + 1) + "_" + str(5 * key + 4),
            "from" : 5 * key + 1,
            "to": 5 * key + 4,
        })

        data_main["allNodes"].append({
            "id": 5 * key + 5,
            "industry": industry,
            "type": "ASN",
            "value": str(ASN),
            "pid": 5 * key + 1,
            "group": key,
            "label": "ASN"
        })


        data_main["allEdges"].append({
            "id": str(5 * key + 1) + "_" + str(5 * key + 5),
            "from" : 5 * key + 1,
            "to": 5 * key + 5,
        })

    return data_main

if __name__ == "__main__":

    pprint.pprint(get_main_data())
