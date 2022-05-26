<template>
    <div class="visits-page">
        <h1 class="page-title">黑灰产元网络 &nbsp;
            <small>
                <small>The meta network</small>
            </small>
        </h1>
        <b-row>
            <b-col lg="7">
                <Widget class="bg-transparent">
                    <!-- <Map /> -->
                    <main-graph :data="mainGraphData" @choose_graph_id='changeGraph($event)' />
                </Widget>
            </b-col>
            <b-col lg="4" offset-lg="1">
                <Widget class="bg-transparent" title="<h5>Map<span class='fw-semi-bold'>&nbsp;Statistics</span></h5>"
                    customHeader>
                    <p>Status: <strong>Live</strong></p>
                    <p>
                        <span class="circle bg-primary text-white"><i class="la la-map-marker" /></span> &nbsp;
                        146 Countries, 2759 Cities
                    </p>
                    <div class="row progress-stats">
                        <div class="col-9">
                            <h6 class="name">Foreign Visits</h6>
                            <p class="description deemphasize mb-xs">Some Cool Text</p>
                            <b-progress variant="primary" :value="60" :max="100" class="progress-xs" />
                        </div>
                        <div class="col-3 text-center">
                            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                                <span>
                                    <AnimatedNumber :value="75" v-bind="animateNumberOptions"></AnimatedNumber>%
                                </span>
                            </span>
                        </div>
                    </div>
                    <div class="row progress-stats">
                        <div class="col-9">
                            <h6 class="name">Local Visits</h6>
                            <p class="description deemphasize mb-xs">P. to C. Conversion</p>
                            <b-progress variant="danger" :value="39" :max="100" class="progress-xs" />
                        </div>
                        <div class="col-3 text-center">
                            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                                <span>
                                    <AnimatedNumber :value="84" v-bind="animateNumberOptions"></AnimatedNumber>%
                                </span>
                            </span>
                        </div>
                    </div>
                    <div class="row progress-stats">
                        <div class="col-9">
                            <h6 class="name">Sound Frequencies</h6>
                            <p class="description deemphasize mb-xs">Average Bitrate</p>
                            <b-progress variant="success" :value="80" :max="100" class="progress-xs" />
                        </div>
                        <div class="col-3 text-center">
                            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                                <span>
                                    <AnimatedNumber :value="92" v-bind="animateNumberOptions"></AnimatedNumber>%
                                </span>
                            </span>
                        </div>
                    </div>
                    <h6 class="fw-semi-bold mt">Map Distributions</h6>
                    <p>Tracking: <strong>Active</strong></p>
                    <p>
                        <span class="circle bg-primary text-white"><i class="la la-cog" /></span>
                        &nbsp; 391 elements installed, 84 sets
                    </p>
                    <b-input-group class="mt">
                        <b-form-input placeholder="Search..." />
                        <b-input-group-append>
                            <b-btn variant="default">
                                <i class="la la-search" />
                            </b-btn>
                        </b-input-group-append>
                    </b-input-group>
                </Widget>
            </b-col>
        </b-row>
        <b-row>
            <b-col lg="8" xs="12">
                <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;结构图</span></h5" customHeader>
                    <sub-graph :data="subGraphData" />
                </Widget>
            </b-col>
            <b-col lg="4" xs="12">
                <b-row>
                    <b-col lg="12" xs="12">
                        <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;关键路径</span></h5" bodyClass="p-0"
                            customHeader>
                            <path-table :data="tableData" :raw-data="subGraphData"/>
                        </Widget>
                    </b-col>
                    <b-col lg="12" xs="12">
                        <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;统计数据</span></h5" bodyClass="p-0"
                            customHeader>
                            <right-down-chart :data="rightDownChartData" />
                        </Widget>
                    </b-col>
                </b-row>

            </b-col>
        </b-row>
    </div>
</template>

<script>
import { reactive, ref } from "vue";
import Widget from '@/components/Widget/Widget';
import AnimatedNumber from "animated-number-vue";

import MainGraph from "./components/MainGraph/MainGraph.vue"
import RightDownChart from "./components/RightDownChart/RightDownChart.vue"
import SubGraph from "./components/SubGraph/SubGraph.vue"
import PathTable from "./components/PathTable/PathTable.vue"

const axios = require('axios');

let subgraph_init = true;

export default {
    name: 'Analysis',
    components: {
        Widget, AnimatedNumber, MainGraph, RightDownChart, SubGraph, PathTable
    },
    data() {
        return {
            animateNumberOptions: {
                duration: 2000,
                easing: 'easeInQuad',
                formatValue(value) {
                    return value.toFixed(0);
                }
            },
            currentGraphID: 0,
            mainGraphData: {
                "allNodes": [],
                "allEdges": []
            },

            subGraphData: reactive({
                nodes: {},
                edges: {},
                paths: reactive({}),
                pathNodes: [],
                layouts: {},
                selectedNodes: ref([]),
                nodesProp: {}
            }),


            tableData: {
                "columns": [],
                "tableData": []
            },

            rightDownChartData: {
                "freqData": [],

                "type2field": {},

                "field2type": {}
            }
        };
    },
    mounted() {
        this.getMainData();
        this.getTableData(this.currentGraphID);
        this.getSubGraph(this.currentGraphID);
        this.getRightDownChartData(this.currentGraphID);
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
            axios.get(this.$api + "/subGraphData/" + id)
                .then(function (response) {
                    console.log("update subgraph");
                    console.log(self.subGraphData.paths);

                    for (let i in self.subGraphData.paths)
                        self.subGraphData.paths[i].active = false;

                    console.log(self.subGraphData.paths);
                    console.log("remove");

                    let res = response.data;
                    console.log(res);
                    self.subGraphData.nodes = res.nodes;
                    self.subGraphData.edges = res.edges;
                    self.subGraphData.layouts = res.layouts;
                    self.subGraphData.selectedNodes = ref([]);

                    self.subGraphData.nodesProp = res.nodesProp;

                    // initialize solts
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
                    self.subGraphData.layouts = {}

                    console.log(self.subGraphData);
                })
                .catch(function (error) {
                    console.log(error);
                })
                .then(function () {
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
            this.getTableData(newID);
            this.getSubGraph(newID);
            this.getRightDownChartData(newID);
        }
    }

};
</script>















































<style src="./Analysis.scss" lang="scss"/>
