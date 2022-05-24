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
          <main-graph/>
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
          <sub-graph/>
        </Widget>
      </b-col>
      <b-col lg="4" xs="12">
        <b-row>
          <b-col lg="12" xs="12">
            <Widget
              title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;关键路径</span></h5" bodyClass="p-0"
              customHeader>
              <path-table/>
            </Widget>
          </b-col>
          <b-col lg="12" xs="12">
            <Widget
              title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;统计数据</span></h5" bodyClass="p-0"
              customHeader>
              <right-down-chart/>
            </Widget>
          </b-col>
        </b-row>

      </b-col>
    </b-row>
  </div>
</template>

<script>
import Vue from 'vue';
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

<style src="./Analysis.scss" lang="scss" />
