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
    view: { scalingObjects: true, },
    node: {
        selectable: true,
        normal: {
            type: "rect",
            width: 32,
            height: 32,
            borderRadius: 8,
            color: "#ff6f00",
        },
        hover: {
            color: "#ff5500",
            width: 36,
            height: 36,
            borderRadius: 8,
        },
        label: {
            visible: true,
            fontFamily: undefined,
            fontSize: 12,
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
        margin: 2,
        marker: {
            source: {
                type: "none",
                width: 4,
                height: 4,
                margin: -1,
                units: "strokeWidth",
                color: null,
            },
            target: {
                type: "arrow",
                width: 4,
                height: 4,
                margin: -1,
                units: "strokeWidth",
                color: null,
            },
        },
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
            <v-edge-label :text="edge.label" :font-size="12" fill="#2d6df3" align="center" vertical-align="above"
                v-bind="slotProps" />
        </template>

        <!-- Use CSS to define references to external fonts.
         To use CSS within SVG, use <defs>. -->
        <defs>
            <!-- Cannot use <style> directly due to restrictions of Vue. -->
            <component is="style">
                @font-face { font-family: 'Material Icons'; font-style: normal; font-weight: 400; src:
                url(https://fonts.gstatic.com/s/materialicons/v97/flUhRq6tzZclQEJ-Vdg-IuiaDsNcIhQ8tQ.woff2)
                format('woff2'); }
            </component>
        </defs>

        <!-- Replace the node component -->
        <template #override-node="{ nodeId, scale, config, ...slotProps }">
            <circle :r="config.radius * scale" :fill="config.color" v-bind="slotProps" />
            <!-- Use v-html to interpret escape sequences for icon characters. -->
            <text font-family="Material Icons" :font-size="22 * scale" fill="#ffffff" text-anchor="middle"
                dominant-baseline="central" style="pointer-events: none" v-html="data.nodes[nodeId].icon" />
        </template>

    </v-network-graph>
</template>
