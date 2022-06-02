<template>
    <div class="visits-page">
        <h1 class="page-title">黑灰产元网络 &nbsp;
            <small>
                <small>The Meta Network</small>
            </small>
        </h1>
        <b-row>
            <b-col lg="8">
                <Widget class="bg-transparent">
                    <main-graph :data="mainGraphData" @choose_graph_id='changeGraph($event)' />
                </Widget>
            </b-col>
            <!-- <b-col lg="4" offset-lg="1"> -->
            <b-col lg="4">
                <Widget class="bg-transparent" title="<h4>黑产子图<span class='fw-semi-bold'>&nbsp;概要</span></h4>"
                    customHeader>
                    <abstract :data="abstractData" />
                </Widget>
            </b-col>
        </b-row>
        <b-row>
            <b-col lg="8" xs="12">
                <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;结构图</span></h5" customHeader>
                    <div>
                        <b-form-checkbox v-model="checked" name="check-button" switch>
                            配色方案：
                            <b v-if="checked">节点类型</b>
                            <b v-else="checked">产业类型</b>
                        </b-form-checkbox>
                    </div>
                    <sub-graph v-for="(item, index) in subgraphs" :key="index" :data="subGraphData"
                        :checked="checked" />
                </Widget>
            </b-col>
            <b-col lg="4" xs="12">
                <b-row>
                    <b-col lg="12" xs="12">
                        <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;关键路径</span></h5" bodyClass="p-0"
                            customHeader>
                            <path-table :data="tableData" :raw-data="subGraphData" />
                        </Widget>
                    </b-col>
                </b-row>

                <right-down-chart :data="rightDownChartData" />
                <!-- <b-col lg="12" xs="12">
                        <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;统计数据</span></h5" bodyClass="p-0"
                            customHeader>
                            <right-down-chart :data="rightDownChartData" />
                        </Widget>
                    </b-col> -->


            </b-col>
        </b-row>
    </div>
</template>

<script>
import { reactive, ref } from "vue";
import Widget from '@/components/Widget/Widget';

import MainGraph from "./components/MainGraph/MainGraph.vue"
import Abstract from "./components/Abstract/Abstract.vue"
import RightDownChart from "./components/RightDownChart/RightDownChart.vue"
import SubGraph from "./components/SubGraph/SubGraph.vue"
import PathTable from "./components/PathTable/PathTable.vue"

const axios = require('axios');

let subgraph_init = true;

export default {
    name: 'Analysis',
    components: {
        Widget, MainGraph, Abstract, RightDownChart, SubGraph, PathTable
    },
    data() {
        return {
            currentGraphID: 0,
            checked: true,
            mainGraphData: {
                "allNodes": [],
                "allEdges": []
            },
            abstractData: {
                "nodeNum": 0,
                "edgeNum": 0,
                "size": 0,
                "darkPercent": 0,
                "corePercent": 0,
                "coreService": "0",
                "type": ""
            },
            subGraphData: {
                nodes: {},
                edges: {},
                paths: reactive({}),
                pathNodes: [],
                layouts: {},
                selectedNodes: ref([]),
                nodesProp: {},
                zoomLevel: 0.5
            },

            tableData: {
                "columns": [],
                "tableData": []
            },

            rightDownChartData: {
                "freqData": [],

                "type2field": {},

                "field2type": {}
            },
            subgraphs: []
        };
    },
    mounted() {
        this.getMainData();
        this.getAbstractData(this.currentGraphID);
        this.getTableData(this.currentGraphID);
        this.getSubGraph(this.currentGraphID);
        this.getRightDownChartData(this.currentGraphID);

        this.$bus.$on('select', (filter) => {
            // console.log(this.subGraphData.nodes);
            for (let node in this.subGraphData.nodesProp) {
                if (filter(this.subGraphData.nodesProp[node], node)) {
                    this.subGraphData.nodes[node]["color"] = this.subGraphData.nodes[node]["color"] + "10";
                }
                else {
                    this.subGraphData.nodes[node]["size"] += 6;
                }
            }
            for (let edge in this.subGraphData.edges) {
                let source = this.subGraphData.edges[edge]["source"];
                let target = this.subGraphData.edges[edge]["target"];
                if (filter(this.subGraphData.nodesProp[source], source) || filter(this.subGraphData.nodesProp[target], target)) {
                    this.subGraphData.edges[edge]["color"] = this.subGraphData.edges[edge]["color"] + "10";
                }
                else {
                    this.subGraphData.edges[edge]["width"] += 4;
                }
            }
        });

        this.$bus.$on('restore', (filter) => {
            for (let node in this.subGraphData.nodesProp) {
                if (filter(this.subGraphData.nodesProp[node], node)) {
                    this.subGraphData.nodes[node]["color"] = this.subGraphData.nodes[node]["color"].slice(0, -2);
                }
                else {
                    this.subGraphData.nodes[node]["size"] -= 6;
                }
            }
            for (let edge in this.subGraphData.edges) {
                let source = this.subGraphData.edges[edge]["source"];
                let target = this.subGraphData.edges[edge]["target"];
                if (filter(this.subGraphData.nodesProp[source], source) || filter(this.subGraphData.nodesProp[target], target)) {
                    this.subGraphData.edges[edge]["color"] = this.subGraphData.edges[edge]["color"].slice(0, -2);
                }
                else {
                    this.subGraphData.edges[edge]["width"] -= 4;
                }
            }
        });
    },
    methods: {
        getMainData: function () {
            self = this;
            axios.get(this.$api + "/mainGraphData")
                .then(function (response) {
                    self.mainGraphData = response.data;
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
                    console.log("finish");
                });
        },
        getAbstractData: function (id) {
            self = this;
            axios.get(this.$api + "/abstractData/" + id)
                .then(function (response) {
                    self.abstractData = response.data;
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
                    console.log("finish");
                });
        },
        getTableData: function (id) {
            self = this;
            axios.get(this.$api + "/tableData/" + id)
                .then(function (response) {
                    self.tableData = response.data;
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
                    console.log("finish");
                });
        },
        getSubGraph: function (id) {
            self = this;
            self.subgraphs = []
            axios.get(this.$api + "/subGraphData/" + id)
                .then(function (response) {
                    // console.log("update subgraph");
                    // console.log(self.subGraphData.paths);

                    for (let i in self.subGraphData.paths)
                        self.subGraphData.paths[i].active = false;

                    // console.log(self.subGraphData.paths);
                    // console.log("remove");

                    let res = response.data;
                    console.log(res);
                    self.subGraphData.nodes = res.nodes;

                    // add "black" color
                    // console.log("Black");
                    let color_map = {
                        // "": "#ffffff",
                        "A": "#f9e996",
                        "B": "#5886b7",
                        "C": "#95caa3",
                        "D": "#be4a4c",
                        "E": "#ffa807",
                        "F": "#6e6efd",
                        "G": "#eb7df4",
                        "H": "#ad85e4",
                    }
                    for (let node in self.subGraphData.nodes) {
                        let type = res.nodesProp[node].black;
                        // console.log(type.length);
                        if (type.length > 1)
                            type = type.substring(0, 1);
                        let color = "#ffffff";
                        if (type in color_map)
                            color = color_map[type];
                        self.subGraphData.nodes[node]["black_color"] = color;
                    }
                    // console.log(self.subGraphData.nodes);

                    self.subGraphData.edges = res.edges;
                    self.subGraphData.layouts = res.layouts;
                    self.subGraphData.selectedNodes = ref([]);

                    self.subGraphData.nodesProp = res.nodesProp;
                    self.subGraphData.zoomLevel = res.zoomlevel;

                    // initialize slots
                    if (subgraph_init) {
                        subgraph_init = false;
                        for (let i = 0; i < 100; ++i)
                            self.subGraphData.paths["path" + i] = { edges: [], active: false, width: 10 };

                        console.log("path solt init");
                        console.log(self.subGraphData.paths);
                    }

                    for (let i in res.paths) {
                        if (i in self.subGraphData.paths) {
                            self.subGraphData.paths[i].active = false;
                            self.subGraphData.paths[i].edges = res.paths[i].edges;
                        }
                        else {
                            console.error("failed!!!");
                        }
                    }
                    let pathNodes = []
                    for (let i in res.pathNodes) {
                        pathNodes.push(new Set(res.pathNodes[i]));
                    }
                    self.subGraphData.pathNodes = pathNodes;
                    // self.subGraphData.layouts = {}

                    console.log("subGraph:", self.subGraphData);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
                    self.subgraphs = [0];
                    console.log("subgraph finish");
                });
        },
        getRightDownChartData: function (id) {
            self = this;
            axios.get(this.$api + "/rightDownCharData/" + id)
                .then(function (response) {
                    self.rightDownChartData = response.data;
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
                    console.log("finish");
                });
        },
        changeGraph: function (e) {
            this.currentGraphID = e;
        }
    },
    watch: {
        currentGraphID(newID, oldID) {
            console.log(this.currentGraphID);
            this.checked = true;
            this.getAbstractData(newID);
            this.getTableData(newID);
            this.getSubGraph(newID);
            this.getRightDownChartData(newID);
        }
    }

};
</script>


<style src="./Analysis.scss" lang="scss"/>
