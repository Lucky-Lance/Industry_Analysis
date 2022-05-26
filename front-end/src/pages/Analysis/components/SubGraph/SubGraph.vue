<template>
    <v-network-graph :selected-nodes="data.selectedNodes" :nodes="data.nodes" :edges="data.edges"
        :layouts="data.layouts" :paths="data.paths" :configs="configs" :event-handlers="eventHandlers">
        <template #edge-label="{ edge, ...slotProps }">
            <v-edge-label :text="edge.label" align="center" vertical-align="above" v-bind="slotProps" />
        </template>

        <!-- Use CSS to define references to external fonts.
         To use CSS within SVG, use <defs>. -->
        <defs>
            <!-- Cannot use <style> directly due to restrictions of Vue. -->
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
            <!-- Use v-html to interpret escape sequences for icon characters. -->
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
                            // radius: (node) => node.size,
                            raduis: 180,
                            // color: (node) => node.color,
                            // color: "#db3934"
                            // color: "#2d8515"
                            // ["#6aa096", "#1c5fcc"]
                            color: "#2d8515",
                        },
                        hover: {
                            radius: (node) => node.size + 2,
                            color: (node) => node.color,
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
                            // width: (edge) => edge.width, // Use the value of each edge object
                            width: 2,
                            // color: (edge) => edge.color,
                            color: "#d8d8d8",
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
    props: ["data"],
    mounted: function () {
        this.$bus.$on('select', (filter) => {
            for (let node in this.data.nodesProp) {
                if (filter(this.data.nodesProp[node], node)) {
                    this.data.nodes[node]["color"] = this.data.nodes[node]["color"] + "20";
                }
            }
            for (let edge in this.data.edges) {
                let source = this.data.edges[edge]["source"];
                let target = this.data.edges[edge]["target"];
                if (filter(this.data.nodesProp[source], source) || filter(this.data.nodesProp[target], target)) {
                    this.data.edges[edge]["color"] = this.data.edges[edge]["color"] + "20";
                }
            }

        });

        this.$bus.$on('restore', (filter) => {
            for (let node in this.data.nodesProp) {
                if (filter(this.data.nodesProp[node], node)) {
                    this.data.nodes[node]["color"] = this.data.nodes[node]["color"].slice(0, -2);
                }
            }
            for (let edge in this.data.edges) {
                let source = this.data.edges[edge]["source"];
                let target = this.data.edges[edge]["target"];
                if (filter(this.data.nodesProp[source], source) || filter(this.data.nodesProp[target], target)) {
                    this.data.edges[edge]["color"] = this.data.edges[edge]["color"].slice(0, -2);
                }
            }
        });
    }
}

</script>
