<template>
  <div class="charts-overview">
    <ol class="breadcrumb">
      <li class="breadcrumb-item">YOU ARE HERE</li>
      <li class="breadcrumb-item active">Charts</li>
    </ol>
    <h1 class="page-title">
      Visual -
      <span class="fw-semi-bold">Charts</span>
    </h1>
    <div>
      <b-row>
        <b-col xs="12" lg="12">
          <Widget
              title="<h5>Example <span class='fw-semi-bold'>Table</span></h5>"
              close collapse customHeader
          > 

          <ve-table :columns="columns" 
            :table-data="tableData" 
            :max-height="300"
            :virtual-scroll-option="virtualScrollOption"
            :border-around="false"/>

          </Widget>
        </b-col>
        <b-col xs="12" lg="7">
          <Widget
              title="<h5>Apex <span class='fw-semi-bold'>Column Chart</span></h5>"
              close collapse customHeader
          >
            <apexchart type="bar" height="350" :series="cd.apex.column.series" :options="cd.apex.column.options"/>
          </Widget>

        </b-col>
        <b-col xs="12" lg="5">
          <Widget
              title="<h5>Echarts <span class='fw-semi-bold'>Line Chart</span></h5>"
              close collapse customHeader
          >
            <echart :options="cd.echarts.line" :init-options="initEchartsOptions" style="height: 370px"></echart>
          </Widget>
        </b-col>
        <b-col xs='12' lg='5'>
          <Widget
              title="<h5>Highcharts <span class='fw-semi-bold'>Line Chart</span></h5>"
              close collapse customHeader
          >
            <highcharts :options="cd.highcharts.mixed"></highcharts>
            <h5 class="mt">Interactive <span class="fw-semi-bold">Sparklines</span></h5>
            <b-row class="mt">
              <b-col md='6' xs='12'>
                <div class="stats-row">
                  <div class="stat-item">
                    <p class="value5 fw-thin">34 567</p>
                    <h6 class="name text-muted m-0 fs-mini">Overall Values</h6>
                  </div>
                  <div class="stat-item stat-item-mini-chart">
                    <Sparklines :data="sparklineData.series" :options="sparklineData.options1" :width="80" :height="25"></Sparklines>
                  </div>
                </div>
              </b-col>
              <b-col md='6' xs='12'>
                <div class="stats-row">
                  <div class="stat-item">
                    <p class="value5 fw-thin">34 567</p>
                    <h6 class="name text-muted m-0 fs-mini">Overall Values</h6>
                  </div>
                  <div class="stat-item stat-item-mini-chart">
                    <Sparklines :data="sparklineData.series" :options="sparklineData.options2" :width="80" :height="25"></Sparklines>
                  </div>
                </div>
              </b-col>
            </b-row>
          </Widget>
        </b-col>
        <b-col xs='12' lg='7'>
          <b-row>
            <b-col xs="12" lg="6">
              <Widget
                  title="<h5>Apex <span class='fw-semi-bold'>Monochrome Pie</span></h5>"
                  close collapse customHeader
              >
                <apexchart type="pie" height="200" :series="cd.apex.pie.series" :options="cd.apex.pie.options"/>
              </Widget>
            </b-col>
            <b-col xs="12" lg="6">
              <Widget
                  title="<h5>Echart <span class='fw-semi-bold'>Donut Chart</span></h5>"
                  close collapse customHeader
              >
                <echart :options="cd.echarts.donut" :init-options="initEchartsOptions" style="height: 175px"></echart>
              </Widget>
            </b-col>
            <b-col xs="12" lg="12">
              <Widget
                  title="<h5>Highcharts <span class='fw-semi-bold'>Live Chart</span></h5>"
                  close collapse customHeader
              >
                <highcharts :options="ld" ref="highchart"></highcharts>
              </Widget>
            </b-col>
          </b-row>
        </b-col>
        <b-col xs="12" lg="12">
          <Widget
              title="<h5>Echart <span class='fw-semi-bold'>River Chart</span></h5>"
              close collapse customHeader
          >
            <echart :options="cd.echarts.river" :init-options="initEchartsOptions" style="height: 350px;"></echart>
          </Widget>
        </b-col>
      </b-row>
    </div>
  </div>
</template>

<script>
import Widget from "@/components/Widget/Widget";
import {chartData, liveChart, liveChartInterval} from './mock';

import ECharts from 'vue-echarts/components/ECharts';
import 'echarts/lib/chart/line';
import 'echarts/lib/chart/themeRiver';
import 'echarts/lib/chart/pie';
import 'echarts/lib/component/tooltip';
import 'echarts/lib/component/legend';

import Highcharts from 'highcharts';
import exporting from 'highcharts/modules/exporting';
import exportData from 'highcharts/modules/export-data';

exporting(Highcharts);
exportData(Highcharts);

import { Chart } from 'highcharts-vue';
import Sparklines from '../../components/Sparklines/Sparklines'

import "vue-easytable/libs/theme-dark/index.css";
import { VeTable, VePagination } from "vue-easytable";

export default {
  name: "Charts",
  components: { Widget, echart: ECharts, highcharts: Chart, Sparklines, VeTable, VePagination},
  data() {
    return {
      cd: chartData,
      ld: liveChart,
      initEchartsOptions: {
        renderer: 'canvas'
      },
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
                ],
    }
  },
  computed: {
    sparklineData() {
      return {
        series: [{data: [1, 7, 3, 5, 7, 8]}],
        options1: {
          colors: [this.appConfig.colors.green],
          plotOptions: {
            bar: {
              columnWidth: '50%'
            }
          }
        },
        options2: {
          colors: [this.appConfig.colors.red],
          plotOptions: {
            bar: {
              columnWidth: '50%'
            }
          }
        }
      }
    }
  },
  beforeDestroy() {
    clearInterval(liveChartInterval);
  }
};
</script>

<style src="./Charts.scss" lang="scss" />
