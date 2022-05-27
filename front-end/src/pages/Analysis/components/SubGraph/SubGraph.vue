<template>
    <v-network-graph :selected-nodes="data.selectedNodes" :nodes="data.nodes" :edges="data.edges"
        v-model:zoom-level="data.zoomLevel" :layouts="data.layouts" :paths="data.paths" :configs="configs"
        :event-handlers="eventHandlers">
        <template #edge-label="{ edge, ...slotProps }">
            <v-edge-label :text="edge.label" align="center" vertical-align="above" v-bind="slotProps" />
        </template>

        <!-- Use CSS to define references to external fonts.
         To use CSS within SVG, use <defs>. -->
        <defs>
            <component is="style">
                @font-face { font-family: 'Material Icons'; font-style: normal;
                font-weight: 400; src:
                url(https://fonts.gstatic.com/s/materialicons/v97/flUhRq6tzZclQEJ-Vdg-IuiaDsNcIhQ8tQ.woff2)
                format('woff2'); }
            </component>
        </defs>

        <!-- Replace the node component -->
        <template #override-node="{ nodeId, scale, config, ...slotProps }">
            <circle :r="config.radius * scale" :fill="config.color" v-bind="slotProps" />
            <text font-family="Material Icons" :font-size="22 * scale" fill="#ffffff" text-anchor="middle"
                dominant-baseline="central" style="pointer-events: none" v-html="data.nodes[nodeId].icon" />
        </template>
    </v-network-graph>
</template>


<script>
import { ref, reactive, toRaw } from "vue";
import * as vNG from "v-network-graph";
import { ForceLayout } from "v-network-graph/lib/force-layout";

export default {
    name: "SubGraph",
    data() {
        return {
            eventHandlers: {
                "node:click": ({ node }) => {
                    // toggle
                    console.log(node);
                    // console.log(this.data.layouts);
                    // this.data.nodes[node].color = "gray";
                    for (let i = 0; i < this.data.pathNodes.length; ++i)
                        if (this.data.pathNodes[i].has(node)) {
                            let pathName = "path" + (i + 1).toString();
                            console.log(pathName, this.data.paths[pathName]);
                            this.data.paths[pathName].active ^= 1;
                        }
                    // data.nodes[node].active = !data.nodes[node].active
                },
            },
            configs: reactive(
                vNG.defineConfigs({
                    view: {
                        scalingObjects: true,
                        // layoutHandler: new ForceLayout({
                        //     positionFixedByDrag: false,
                        //     positionFixedByClickWithAltKey: true,
                        //     // * The following are the default parameters for the simulation.
                        //     // * You can customize it by uncommenting below.
                        //     // createSimulation: (d3, nodes, edges) => {
                        //     //   const forceLink = d3.forceLink(edges).id(d => d.id)
                        //     //   return d3
                        //     //     .forceSimulation(nodes)
                        //     //     .force("edge", forceLink.distance(100))
                        //     //     .force("charge", d3.forceManyBody())
                        //     //     .force("collide", d3.forceCollide(50).strength(0.2))
                        //     //     .force("center", d3.forceCenter().strength(0.05))
                        //     //     .alphaMin(0.001)
                        //     // }
                        // }),
                    },
                    node: {
                        selectable: true,
                        normal: {
                            type: "rect",
                            width: 32,
                            height: 32,
                            borderRadius: 8,
                            radius: (node) => node.size,
                            color: (node) => node.color,
                            // ["#6aa096", "#1c5fcc", "#2d8515", "#db3934", "#fdb462"]
                        },
                        hover: {
                            radius: (node) => node.size + 2,
                            // color: (node) => node.color,
                        },
                        label: {
                            visible: true,
                            fontFamily: "gillsans-bold",
                            fontSize: 12,
                            lineHeight: 1.1,
                            color: "#ffffff",
                            margin: 4,
                            direction: "south",
                            text: "name",
                        },
                    },
                    edge: {
                        gap: 12,
                        normal: {
                            width: (edge) => edge.width, // Use the value of each edge object
                            color: (edge) => edge.color,
                            dasharray: (edge) => (edge.dashed ? "4" : "0"),
                        },
                        type: "curve",
                        margin: 2,
                        // label: {
                        //     fontFamily: "gillsans-bold",
                        //     fontSize: 16,
                        //     lineHeight: 1.1,
                        //     color: "#ffffff",
                        // },
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
                            width: (path) => path.width * path.active,
                            dasharray: "10 16",
                            animate: true,
                            animationSpeed: 40,
                        },
                    },
                })
            )
        }
    },
    props: ["data"]
}

</script>