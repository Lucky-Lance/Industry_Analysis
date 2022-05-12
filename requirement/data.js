const nodes = {
    node1: { name: "N1", icon: "&#xe320" /* Laptop Mac */ },
    node2: { name: "N2", icon: "&#xe328" /* Router */ },
    node3: { name: "N3", icon: "&#xe331" /* Tablet Mac */ },
    node4: { name: "N4", icon: "&#xe2bd" /* Cloud */ },
    node5: { name: "N5", icon: "&#xf0e2" /* Support Agent */ },
    node6: { name: "N6", icon: "&#xea75" /* Video Settings */ },
    node7: { name: "N7", icon: "&#xe2bd" /* Cloud */ },
    node8: { name: "N8", icon: "&#xe2bd" /* Cloud */ },
    node9: { name: "N9", icon: "&#xe2bd" /* Cloud */ },
    node10: { name: "N10", icon: "&#xe2bd" /* Cloud */ },
}

const edges = {
    edge1: { source: "node1", target: "node2", label: "1-2" },
    edge2: { source: "node3", target: "node2", label: "3-2" },
    edge3: { source: "node2", target: "node4", label: "2-4" },
    edge4: { source: "node2", target: "node4", label: "2-4" },
    edge5: { source: "node4", target: "node5", label: "4-5" },
    edge6: { source: "node4", target: "node6", label: "4-6" },
    edge7: { source: "node5", target: "node7", label: "5-7" },
    edge8: { source: "node5", target: "node8", label: "5-8" },
    edge9: { source: "node6", target: "node9", label: "6-9" },
    edge10: { source: "node6", target: "node10", label: "6-10" },
}

const paths = {
    path1: { edges: ["edge1", "edge3", "edge5", "edge7"], width: 0 },
    path2: { edges: ["edge2", "edge4", "edge6", "edge10"], width: 0 },
})

const pathNodes = [
    new Set(["node1", "node2", "node4", "node5", "node7"]),
    new Set(["node3", "node2", "node4", "node6", "node10"]),
]
