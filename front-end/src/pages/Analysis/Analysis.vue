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
          <main-graph :data="mainGraphData"/>
        </Widget>
      </b-col>
      <b-col lg="4" offset-lg="1">
        <Widget
          class="bg-transparent"
          title="<h5>Map<span class='fw-semi-bold'>&nbsp;Statistics</span></h5>"
          customHeader
        >
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
                <span><AnimatedNumber :value="75" v-bind="animateNumberOptions"></AnimatedNumber>%</span>
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
                <span><AnimatedNumber :value="84" v-bind="animateNumberOptions"></AnimatedNumber>%</span>
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
                <span><AnimatedNumber :value="92" v-bind="animateNumberOptions"></AnimatedNumber>%</span>
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
        <Widget
          title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;结构图</span></h5"
          customHeader>
          <sub-graph :data="subGraphData"/>
        </Widget>
      </b-col>
      <b-col lg="4" xs="12">
        <b-row>
          <b-col lg="12" xs="12">
            <Widget
              title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;关键路径</span></h5" bodyClass="p-0"
              customHeader>
              <path-table :data="tableData"/>
            </Widget>
          </b-col>
          <b-col lg="12" xs="12">
            <Widget
              title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;统计数据</span></h5" bodyClass="p-0"
              customHeader>
              <right-down-chart :data="rightDownChartData"/>
            </Widget>
          </b-col>
        </b-row>

      </b-col>
    </b-row>
  </div>
</template>

<script>
import Vue from 'vue';
import {reactive } from "vue";
import Widget from '@/components/Widget/Widget';
import AnimatedNumber from "animated-number-vue";

import MainGraph from "./components/MainGraph/MainGraph.vue"
import RightDownChart from "./components/RightDownChart/RightDownChart.vue"
import SubGraph from "./components/SubGraph/SubGraph.vue"
import PathTable from "./components/PathTable/PathTable.vue"

export default {
  name: 'Analysis',
  components: {
    Widget,  AnimatedNumber, MainGraph, RightDownChart, SubGraph, PathTable
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
      checkedArr: [false, false, false],
      dataCollection: null,
      mainGraphData: {
        allNodes: [
                    { id: 1, industry: "industry1", type: "Parent", pid: 0, subids: [2, 3, 4, 5], group: 0, label: "industry1" },
                    { id: 2, industry: "industry1", type: "cert", value: "3", port: "22", pid: 1, group: 0, label: "cert" },
                    { id: 3, industry: "industry1", type: "ip", value: "200", pid: 1, group: 0, label: "ip" },
                    { id: 4, industry: "industry1", type: "domain", value: "10", pid: 1, group: 0, label: "domain" },
                    { id: 5, industry: "industry1", type: "name", value: "20", pid: 1, group: 0, label: "name" },


                    { id: 6, industry: "industry2", type: "Parent", pid: 0, subids: [7, 8, 9, 10], group: 1, label: "industry2" },
                    { id: 7, industry: "industry2", type: "cert", value: "30", pid: 6, group: 1, label: "cert" },
                    { id: 8, industry: "industry2", type: "ip", value: "200", pid: 6, group: 1, label: "ip" },
                    { id: 9, industry: "industry2", type: "domain", value: "10", pid: 6, group: 1, label: "domain" },
                    { id: 10, industry: "industry2", type: "name", value: "40", pid: 6, group: 1, label: "name" }
                ],
        allEdges: [
            { id: "1_2", from: 1, to: 2 },
            { id: "1_3", from: 1, to: 3 },
            { id: "1_4", from: 1, to: 4 },
            { id: "1_5", from: 1, to: 5 },

            { id: "6_7", from: 6, to: 7 },
            { id: "6_8", from: 6, to: 8 },
            { id: "6_9", from: 6, to: 9 },
            { id: "6_10", from: 6, to: 10 }
        ]
      },
      subGraphData:{
        nodes: {
          node1: {
              name: "N1",
              icon: "&#xe320",
              /* Laptop Mac */
              size: 16,
              color: "#ff6f00"
          },
          node2: {
              name: "N2",
              icon: "&#xe328",
              /* Router */
              size: 16,
              color: "#ff6f00"
          },
          node3: {
              name: "N3",
              icon: "&#xe331",
              /* Tablet Mac */
              size: 16,
              color: "#ff6f00"
          },
          node4: {
              name: "N4",
              icon: "&#xe2bd",
              /* Cloud */
              size: 20,
              color: "lightskyblue"
          },
          node5: {
              name: "N5",
              icon: "&#xf0e2",
              /* Support Agent */
              size: 16,
              color: "#ff6f00"
          },
          node6: {
              name: "N6",
              icon: "&#xea75",
              /* Video Settings */
              size: 16,
              color: "#ff6f00"
          },
          node7: {
              name: "N7",
              icon: "&#xe2bd",
              /* Cloud */
              size: 20,
              color: "lightskyblue"
          },
          node8: {
              name: "N8",
              icon: "&#xe2bd",
              /* Cloud */
              size: 20,
              color: "lightskyblue"
          },
          node9: {
              name: "N9",
              icon: "&#xe2bd",
              /* Cloud */
              size: 20,
              color: "lightskyblue"
          },
          node10: {
              name: "N10",
              icon: "&#xe2bd",
              /* Cloud */
              size: 20,
              color: "lightskyblue"
          },

        },
        edges: {
          edge1: {
              source: "node1",
              target: "node2",
              label: "1-2",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge2: {
              source: "node3",
              target: "node2",
              label: "3-2",
              width: 3,
              color: "skyblue"
          },
          edge3: {
              source: "node2",
              target: "node4",
              label: "2-4",
              width: 3,
              color: "skyblue"
          },
          edge4: {
              source: "node2",
              target: "node4",
              label: "2-4",
              width: 3,
              color: "skyblue"
          },
          edge5: {
              source: "node4",
              target: "node5",
              label: "4-5",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge6: {
              source: "node4",
              target: "node6",
              label: "4-6",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge7: {
              source: "node5",
              target: "node7",
              label: "5-7",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge8: {
              source: "node5",
              target: "node8",
              label: "5-8",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge9: {
              source: "node6",
              target: "node9",
              label: "6-9",
              width: 5,
              color: "#ff6f00",
              dashed: true
          },
          edge10: {
              source: "node6",
              target: "node10",
              label: "6-10",
              width: 3,
              color: "skyblue"
          },
    },
    paths: reactive({
        path1: {
            edges: ["edge1", "edge3", "edge5", "edge7"],
            active: false,
            width: 10
        },
        path2: {
            edges: ["edge2", "edge4", "edge6", "edge10"],
            active: false,
            width: 10
        },
    }),
    pathNodes: [
        new Set(["node1", "node2", "node4", "node5", "node7"]),
        new Set(["node3", "node2", "node4", "node6", "node10"]),
    ],
    layouts: {
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
      },
      tableData:{
            columns: [
                    { field: "name", key: "a", title: "Name", align: "center" },
                    { field: "date", key: "b", title: "Date", align: "left" },
                    {
                        field: "hobby",
                        key: "c",
                        title: "Hobby",
                        align: "right",
                    },
                    { field: "address", key: "d", title: "Address" },
                ],
            tableData: [
                {
                    name: "John",
                    date: "1900-05-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Shanghai",
                },
                {
                    name: "Dickerson",
                    date: "1910-06-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Beijing",
                },
                {
                    name: "Larsen",
                    date: "2000-07-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Chongqing",
                },
                {
                    name: "Geneva",
                    date: "2010-08-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Xiamen",
                },
                {
                    name: "Jami",
                    date: "2020-09-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Shenzhen",
                },
                {
                    name: "John",
                    date: "1900-05-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Shanghai",
                },
                {
                    name: "Dickerson",
                    date: "1910-06-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Beijing",
                },
                {
                    name: "Larsen",
                    date: "2000-07-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Chongqing",
                },
                {
                    name: "Geneva",
                    date: "2010-08-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Xiamen",
                },
                {
                    name: "Jami",
                    date: "2020-09-20",
                    hobby: "coding and coding repeat",
                    address: "No.1 Century Avenue, Shenzhen",
                },
            ]
      },
      rightDownChartData: {
        freqData: [
                { State: '0 - 50', Count: 100, freq: { Domain: 4786, IP: 1319, Cert: 249, ASN: 100 }, area: { Top: 1, Mid: 9, Bot: 11 } }
                , { State: '50 - 100', Count: 22, freq: { Domain: 1101, IP: 412, Cert: 674, ASN: 23 }, area: { Top: 2, Mid: 8, Bot: 12 } }
                , { State: '100 - 150', Count: 31, freq: { Domain: 932, IP: 2149, Cert: 418, ASN: 34 }, area: { Top: 3, Mid: 7, Bot: 13 } }
                , { State: '150 - 200', Count: 50, freq: { Domain: 832, IP: 1152, Cert: 1862, ASN: 45 }, area: { Top: 4, Mid: 6, Bot: 14 } }
                , { State: '200 - 250', Count: 300, freq: { Domain: 4481, IP: 3304, Cert: 948, ASN: 56 }, area: { Top: 5, Mid: 5, Bot: 15 } }
                , { State: '250 - 300', Count: 64, freq: { Domain: 324, IP: 3434, Cert: 436, ASN: 67 }, area: { Top: 6, Mid: 4, Bot: 16 } }
                , { State: '300 - 350', Count: 97, freq: { Domain: 4566, IP: 546, Cert: 675, ASN: 78 }, area: { Top: 7, Mid: 3, Bot: 17 } }
                , { State: '350 - 400', Count: 18, freq: { Domain: 8676, IP: 876, Cert: 665, ASN: 89 }, area: { Top: 8, Mid: 2, Bot: 18 } }
                , { State: '> 400', Count: 9, freq: { Domain: 3421, IP: 987, Cert: 354, ASN: 90 }, area: { Top: 9, Mid: 1, Bot: 19 } }
            ],
        type2field: {
                'Domain': { 'Top': 12, 'Mid': 90, 'Bot': 21 }
                , 'IP': { 'Top': 22, 'Mid': 80, 'Bot': 41 }
                , 'Cert': { 'Top': 32, 'Mid': 20, 'Bot': 30 }
                , 'ASN': { 'Top': 2, 'Mid': 60, 'Bot': 50 }
            },
        field2type: {
            'Top': { 'Domain': 12, 'IP': 90, 'Cert': 21, 'ASN': 10}
            , 'Mid': { 'Domain': 22, 'IP': 80, 'Cert': 41, 'ASN': 30}
            , 'Bot': { 'Domain': 32, 'IP': 20, 'Cert': 30, 'ASN': 40}
        }
      }

    };
  },
  methods: {
    checkTable(id) {
      let arr = [];
      if (id === 0) {
        const val = !this.checkedArr[0];
        for (let i = 0; i < this.checkedArr.length; i += 1) {
          arr[i] = val;
        }
      } else {
        arr = this.checkedArr;
        arr[id] = !arr[id];
      }
      if (arr[0]) {
        let count = 1;
        for (let i = 1; i < arr.length; i += 1) {
          if (arr[i]) {
            count += 1;
          }
        }
        if (count !== arr.length) {
          arr[0] = !arr[0];
        }
      }
      Vue.set(this, 'checkedArr', arr);
    },
    fillData () {
      this.dataCollection = {
        labels: [this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt()],
        datasets: [
          {
            label: 'Data One',
            backgroundColor: '#1870DC',
            borderColor: 'transparent',
            data: [this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt()]
          }, {
            label: 'Data Two',
            backgroundColor: '#F45722',
            borderColor: 'transparent',
            data: [this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt(), this.getRandomInt()]
          }
        ]
      }
    },
    getRandomInt () {
      return Math.floor(Math.random() * (50 - 5 + 1)) + 5
    }
  },
  beforeMount() {
    this.fillData();
  },
};
</script>

<style src="./Analysis.scss" lang="scss"/>
