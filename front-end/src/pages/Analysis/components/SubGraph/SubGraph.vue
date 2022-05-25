<template>
    <v-network-graph v-model:selected-nodes="selectedNodes" :nodes="data.nodes" :edges="data.edges"
        :layouts="data.layouts" :paths="data.paths" :configs="configs" :event-handlers="eventHandlers">

        <!-- Draw a rectangle with a red background -->
        <defs>
            <component :is="'style'">
                <!-- prettier-ignore -->
                .point { fill: #ff000080; }
                .coords { font-size: 11px; fill: #444; }
                .lt { text-anchor: start; dominant-baseline: hanging; transform: translate(5px, 5px); }
                .rt { text-anchor: end; dominant-baseline: hanging; transform: translate(-5px, 5px); }
                .lb { text-anchor: start; dominant-baseline: ideographic; transform: translate(5px, -5px); }
                .rb { text-anchor: end; dominant-baseline: ideographic; transform: translate(-5px, -5px); }
            </component>
        </defs>
        <template #redZone>
            <rect x="50" y="50" width="400" height="200" fill="#ff000030" />
            <circle cx="50" cy="50" r="5" class="point" />
            <circle cx="450" cy="50" r="5" class="point" />
            <circle cx="50" cy="250" r="5" class="point" />
            <circle cx="450" cy="250" r="5" class="point" />
            <text x="50" y="50" class="coords lt">(50, 50)</text>
            <text x="450" y="50" class="coords rt">(450, 50)</text>
            <text x="50" y="250" class="coords lb">(50, 450)</text>
            <text x="450" y="250" class="coords rb">(450, 450)</text>
        </template>

        <template #edge-label="{ edge, ...slotProps }">
            <v-edge-label :text="edge.label" :font-size="18" fill="#ffffff" align="center" vertical-align="above"
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

<script setup>
import { ref } from "vue";
import * as vNG from "v-network-graph"
import data from "./data"
import {
    ForceLayout
} from "v-network-graph/lib/force-layout"
 
const selectedNodes = ref([])

const eventHandlers = {
    "node:click": ({ node }) => {
        // toggle
        data.nodes[node].color = "gray";
        for (let i = 0; i < 2; ++i)
            if (data.pathNodes[i].has(node)) {
                let pathName = "path" + (i + 1).toString();
                console.log(pathName, data.paths[pathName]);
                data.paths[pathName].active ^= 1;
            }
        // data.nodes[node].active = !data.nodes[node].active
    },
}

const configs = vNG.defineConfigs({
    view: {
        scalingObjects: true,
        layoutHandler: new ForceLayout({
            positionFixedByDrag: false,
            positionFixedByClickWithAltKey: true,
            // * The following are the default parameters for the simulation.
            // * You can customize it by uncommenting below.
            // createSimulation: (d3, nodes, edges) => {
            //   const forceLink = d3.forceLink<ForceNodeDatum, ForceEdgeDatum>(edges).id(d => d.id)
            //   return d3
            //     .forceSimulation(nodes)
            //     .force("edge", forceLink.distance(100))
            //     .force("charge", d3.forceManyBody())
            //     .force("collide", d3.forceCollide(50).strength(0.2))
            //     .force("center", d3.forceCenter().strength(0.05))
            //     .alphaMin(0.001)
            // }
        }),
    },
    node: {
        selectable: true,
        normal: {
            type: "rect",
            width: 32,
            height: 32,
            borderRadius: 8,
            radius: node => node.size,
            color: node => node.color,
        },
        hover: {
            radius: node => node.size + 2,
            color: node => node.color,
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
    // view: {
    //     grid: {
    //         visible: true,
    //         interval: 10,
    //         thickIncrements: 5,
    //         line: {
    //             color: "#e0e0e0",
    //             width: 1,
    //             dasharray: 1,
    //         },
    //         thick: {
    //             color: "#cccccc",
    //             width: 1,
    //             dasharray: 0,
    //         },
    //     }
    // },
    edge: {
        gap: 12,
        normal: {
            width: edge => edge.width, // Use the value of each edge object
            color: edge => edge.color,
            dasharray: edge => (edge.dashed ? "4" : "0"),
        },
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
            width: path => path.width * path.active,
            dasharray: "10 16",
            animate: true,
            animationSpeed: 40,
        },
    },
})
</script>