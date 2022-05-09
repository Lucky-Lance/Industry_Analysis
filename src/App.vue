<script setup lang="ts">
import { reactive, ref } from "vue";
import * as vNG from "v-network-graph"
import data from "./data"

const paths: vNG.Paths = {
    path1: { edges: ["edge1", "edge3", "edge5", "edge7"] },
    path2: { edges: ["edge2", "edge4", "edge6", "edge10"] },
}
const selectedNodes = ref<string[]>([])

const configs = vNG.defineConfigs({
    node: {
        selectable: true,
        normal: { type: "circle", radius: 20, color: "#99ccff" },
        hover: { color: "#88bbff" },
        label: {
            visible: true,
            fontFamily: undefined,
            fontSize: 11,
            lineHeight: 1.1,
            color: "#000000",
            margin: 4,
            direction: "south",
            text: "name",
        },
    },
    edge: {
        gap: 12,
        normal: { color: "#6699cc" },
        type: "curve",
    },
    path: {
        visible: true,
        normal: {
            width: 10,
            dasharray: "10 16",
            animate: true,
            animationSpeed: 40,
        },
    },
})
</script>

<template>
    <v-network-graph v-model:selected-nodes="selectedNodes" :nodes="data.nodes" :edges="data.edges"
        :layouts="data.layouts" :paths="paths" :configs="configs">
        <template #edge-label="{ edge, ...slotProps }">
            <v-edge-label :text="edge.label" align="center" vertical-align="above" v-bind="slotProps" />
        </template>
    </v-network-graph>
</template>
