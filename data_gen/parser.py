import json
from typing import Dict, Iterable
from random import randint


class DataGenerator():
    def __init__(self, file_name) -> None:
        self.file_name = file_name
        self.names: Dict[str, str] = dict()
        self.nodes: Dict = dict()
        self.edges: Dict = dict()

    def toFile(self):
        with open(self.file_name, "w") as f:
            f.write(
                "import { Nodes, Edges, Layouts } "
                "from \"v-network-graph\"\n\n")

            f.write("const nodes: Nodes = {\n")
            for node in self.nodes:
                f.write("    {}: {{ name: \"{}\", icon: \"{}\" }},\n".format(
                    node, self.nodes[node]["name"], self.nodes[node]["icon"]))
            f.write("}\n\n")

            f.write("const edges: Edges = {\n")
            for edge in self.edges:
                f.write("    {}: {{ source: \"{}\", target: \"{}\" }},\n".format(
                    edge, self.edges[edge]["source"], self.edges[edge]["target"]))
            f.write("}\n\n")

            f.write("const layouts: Layouts = {\n"
                    "    nodes: {\n"
                    "    },\n"
                    "}\n\n")

            f.write("export default {\n"
                    "    nodes,\n"
                    "    edges,\n"
                    "    layouts,\n"
                    "}\n\n")

    def initNodes(self, nodes: Iterable):
        for node in nodes:
            self.names[node] = "node" + str(len(self.names))

    def addNode(self, name, type):
        icons = ["&#xe320", "&#xe328", "&#xe331", "&#xe2bd", "&#xf0e2"]
        node_name = self.names[name]
        self.nodes[node_name] = {
            "name": name,
            "icon": icons[type]
        }

    def addEdge(self, src, dst, label=None):
        edge_name = "edge" + str(len(self.edges))
        self.edges[edge_name] = {
            "source": self.names[src],
            "target": self.names[dst],
        }
        if not label is None:
            self.edges[edge_name]["label"] = label


with open("sample.json", "r") as f:
    data = json.load(f)
generator = DataGenerator("data.ts")
generator.initNodes([node["name"] for node in data["nodes"]])
for edge in data["links"]:
    generator.addEdge(str(edge["source"]), str(edge["target"]))
for node in data["nodes"]:
    generator.addNode(node["name"], randint(0, 4))
generator.toFile()
print("Done")
