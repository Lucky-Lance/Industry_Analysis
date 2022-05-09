import { Nodes, Edges, Layouts } from "v-network-graph"

const nodes: Nodes = {
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

const edges: Edges = {
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

const layouts: Layouts = {
    nodes: {
        // node1: { x: 0, y: 0 },
        // node2: { x: 100, y: 60 },
        // node3: { x: 0, y: 110 },
        // node4: { x: 250, y: 60 },
        // node5: { x: 350, y: 10 },
        // node6: { x: 350, y: 110 },
        // node7: { x: 450, y: 10 },
        // node8: { x: 450, y: 60 },
        // node9: { x: 450, y: 110 },
        // node10: { x: 450, y: 160 },
    },
}

export default {
    nodes,
    edges,
    layouts,
}
