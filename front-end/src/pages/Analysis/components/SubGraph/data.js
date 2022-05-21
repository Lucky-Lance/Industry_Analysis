import { reactive } from '@vue/composition-api';

const nodes = {
    node1: {
        name: "N1", icon: "&#xe320", /* Laptop Mac */
        size: 16, color: "#ff6f00"
    },
    node2: {
        name: "N2", icon: "&#xe328", /* Router */
        size: 16, color: "#ff6f00"
    },
    node3: {
        name: "N3", icon: "&#xe331", /* Tablet Mac */
        size: 16, color: "#ff6f00"
    },
    node4: {
        name: "N4", icon: "&#xe2bd", /* Cloud */
        size: 20, color: "lightskyblue"
    },
    node5: {
        name: "N5", icon: "&#xf0e2", /* Support Agent */
        size: 16, color: "#ff6f00"
    },
    node6: {
        name: "N6", icon: "&#xea75", /* Video Settings */
        size: 16, color: "#ff6f00"
    },
    node7: {
        name: "N7", icon: "&#xe2bd", /* Cloud */
        size: 20, color: "lightskyblue"
    },
    node8: {
        name: "N8", icon: "&#xe2bd", /* Cloud */
        size: 20, color: "lightskyblue"
    },
    node9: {
        name: "N9", icon: "&#xe2bd", /* Cloud */
        size: 20, color: "lightskyblue"
    },
    node10: {
        name: "N10", icon: "&#xe2bd", /* Cloud */
        size: 20, color: "lightskyblue"
    },
}

const edges = {
    edge1: {
        source: "node1", target: "node2", label: "1-2",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge2: {
        source: "node3", target: "node2", label: "3-2",
        width: 3, color: "skyblue"
    },
    edge3: {
        source: "node2", target: "node4", label: "2-4",
        width: 3, color: "skyblue"
    },
    edge4: {
        source: "node2", target: "node4", label: "2-4",
        width: 3, color: "skyblue"
    },
    edge5: {
        source: "node4", target: "node5", label: "4-5",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge6: {
        source: "node4", target: "node6", label: "4-6",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge7: {
        source: "node5", target: "node7", label: "5-7",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge8: {
        source: "node5", target: "node8", label: "5-8",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge9: {
        source: "node6", target: "node9", label: "6-9",
        width: 5, color: "#ff6f00", dashed: true
    },
    edge10: {
        source: "node6", target: "node10", label: "6-10",
        width: 3, color: "skyblue"
    },
}

const paths = reactive({
    path1: {
        edges: ["edge1", "edge3", "edge5", "edge7"],
        active: false, width: 10
    },
    path2: {
        edges: ["edge2", "edge4", "edge6", "edge10"],
        active: false, width: 10
    },
})

const pathNodes = [
    new Set(["node1", "node2", "node4", "node5", "node7"]),
    new Set(["node3", "node2", "node4", "node6", "node10"]),
]

const layouts = {
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
    paths,
    pathNodes,
    layouts,
}