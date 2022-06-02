<template>
    <div>
        <div id="legend" style="text-align: center;" v-show="!checked">

        </div>
        <v-network-graph :selected-nodes="data.selectedNodes" :nodes="data.nodes" :edges="data.edges"
            v-model:zoom-level="data.zoomLevel" :layouts="data.layouts" :paths="data.paths" :configs="configs"
            :event-handlers="eventHandlers">
            <!-- <template #edge-label="{ edge, ...slotProps }">
            <v-edge-label :text="edge.label" align="center" vertical-align="above" v-bind="slotProps" />
        </template> -->

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
                <text font-family="Material Icons" :font-size="28 * scale" fill="#3b3b3b" text-anchor="middle"
                    dominant-baseline="central" style="pointer-events: none" v-html="data.nodes[nodeId].icon" />
            </template>
        </v-network-graph>
    </div>
</template>


<script>
import { ref, reactive, toRaw } from "vue";
import * as vNG from "v-network-graph";
import { ForceLayout } from "v-network-graph/lib/force-layout";
import * as d3 from "d3";

// let color_map = {
//     "#677adc": "#e4585b",
//     "#ee8fe3": "#f9e996",
//     "#adde79": "#95caa3",
// }

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
                            radius: 22,
                            color: (node) => node.color,
                            // ["#6aa096", "#1c5fcc", "#2d8515", "#db3934", "#fdb462"]
                        },
                        hover: {
                            radius: 24,
                            // radius: (node) => node.size + 2,
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
                            // width: (edge) => edge.width, // Use the value of each edge object
                            width: 2,
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
                                width: 2,
                                height: 2,
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
            ),

        }
    },
    props: ["data", "checked"],
    mounted() {
        this.draw("#legend");
    },
    methods: {
        draw: function (id) {
            let lD = [
                { type: "黄色" },
                { type: "赌博" },
                { type: "诈骗" },
                { type: "贩毒" },
                { type: "贩枪" },
                { type: "黑客" },
                { type: "交易平台" },
                { type: "支付平台" },
                { type: "未知" }
            ];

            let segColor = {
                "黄色": "#f9e996",
                "赌博": "#5886b7",
                "诈骗": "#95caa3",
                "贩毒": "#be4a4c",
                "贩枪": "#ffa807",
                "黑客": "#6e6efd",
                "交易平台": "#eb7df4",
                "支付平台": "#ad85e4",
                "未知": "#FFFFFF",

            };

            // create table for legend.
            let legend = d3.select(id)
                .append("table").attr('class', 'legend');

            // create one row per segment.
            let tr = legend.append("tbody").selectAll("tr").data(lD)
                .enter().append("tr")
                // .style("width", "115px")
                .style("white-space", "nowrap")
                .style("display", "inline-block");

            // create the first column for each segment.
            tr.append("td").append("svg").attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3').append("rect")
                .attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3')
                .attr("fill", function (d) { return segColor[d.type] });

            // create the second column for each segment.
            tr.append("td").text(function (d) { return d.type; });
        }
    },
    watch: {
        checked(newValue, oldValue) {
            console.log("change color");
            // console.log(this.configs);

            if (newValue) {
                this.configs.node.normal.color = (node) => node.color;
            }
            else {
                this.configs.node.normal.color = (node) => node.black_color;
            }
        }
    }
}

</script>