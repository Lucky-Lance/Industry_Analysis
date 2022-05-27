<template>
    <div>
        <b-row>
        <b-col lg="12" xs="12">
            <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;节点度数分布</span></h5" bodyClass="p-0"
                customHeader>
                    <div id='dashboardBar'></div>
                    
            </Widget>
        </b-col>
        </b-row>
        <b-row>
            <b-col lg="6" xs="12">
                <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;节点类型分布</span></h5" bodyClass="p-0"
                    customHeader>
                        <br/>
                        <div id='dashboardPie1'></div>
                </Widget>
            </b-col>
            <b-col lg="6" xs="12">
                <Widget title="<h5>黑产子网<span class='fw-semi-bold'>&nbsp;节点上下游分布</span></h5" bodyClass="p-0"
                    customHeader>
                        <br/>
                        <div id='dashboardPie2'></div>
                </Widget>
            </b-col>
        </b-row>
    </div>
</template>

<script>
import * as d3 from "d3";
import Widget from '@/components/Widget/Widget';

export default {
    name: 'RightDownChart',
    components: {
        Widget,
    },
    props: ["data"],
    mounted() {
        this.dashboard(['#dashboardBar', '#dashboardPie1', '#dashboardPie2'], this.data.freqData, this.data.type2field, this.data.field2type);
    },
    methods: {
        dashboard(id, fData, type2field, field2type) {

            console.log("id", id);

            for (let x in id){
                var mySvgs = document.querySelectorAll(id[x] + ' svg')
                for(let i = 0; i < mySvgs.length; i++){
                    mySvgs[i].remove();
                }
            }

            for (let x in id){
                var mySvgs = document.querySelectorAll(id[x] + ' table')
                for(let i = 0; i < mySvgs.length; i++){
                    mySvgs[i].remove();
                }
            }
           
            var barColor = 'steelblue';
            function segColor(c) { return { Domain: "#807dba", IP: "#e08214", Cert: "#41ab5d", ASN: "#E80000", Top: "#807dba", Mid: "#e08214", Bot: "#41ab5d" }[c]; }

            // compute total for each state.
            fData.forEach(function (d) { d.total = d.freq.Domain + d.freq.IP + d.freq.Cert + d.freq.ASN; });

            // function to handle histogram.
            function histoGram(fD, self) {

                var hG = {}, hGDim = { t: 60, r: 0, b: 30, l: 0 };
                hGDim.w = 500 - hGDim.l - hGDim.r,
                    hGDim.h = 300 - hGDim.t - hGDim.b;

                //create svg for histogram.

                var hGsvg = d3.select(id[0]).append("svg")
                    .attr("preserveAspectRatio", "xMidYMid meet")
                    .attr("viewBox", "0 0 "+(hGDim.w + hGDim.l + hGDim.r)+" "+(hGDim.h + hGDim.t + hGDim.b))
                    // .attr("width", hGDim.w + hGDim.l + hGDim.r)
                    // .attr("height", hGDim.h + hGDim.t + hGDim.b)
                    .append("g")
                    .attr("transform", "translate(" + hGDim.l + "," + hGDim.t + ")");

                // create function for x-axis mapping.
                var x = d3.scale.ordinal().rangeRoundBands([0, hGDim.w], 0.1)
                    .domain(fD.map(function (d) { return d[0]; }));

                // Add x-axis to the histogram svg.
                hGsvg.append("g").attr("class", "x axis")
                    .attr("transform", "translate(0," + hGDim.h + ")")
                    .style('fill', 'white')
                    .call(d3.svg.axis().scale(x).orient("bottom"));


                // Create function for y-axis map.
                var y = d3.scale.linear().range([hGDim.h, 0])
                    .domain([0, d3.max(fD, function (d) { return d[1]; })]);

                // Create bars for histogram to contain rectangles and freq labels.
                var bars = hGsvg.selectAll(".bar").data(fD).enter()
                    .append("g").attr("class", "bar");

                //create the rectangles.
                bars.append("rect")
                    .attr("x", function (d) { return x(d[0]); })
                    .attr("y", function (d) { return y(d[1]); })
                    .attr("width", x.rangeBand())
                    .attr("height", function (d) { return hGDim.h - y(d[1]); })
                    .attr('fill', barColor)
                    .attr('rx', 3)
                    .attr('ry', 3)
                    .attr('stroke', 'black') // 矩形边框颜色
                    .on("mouseover", mouseover)// mouseover is defined below.
                    .on("mouseout", mouseout);// mouseout is defined below.

                //Create the frequency labels above the rectangles.
                bars.append("text").text(function (d) { return d3.format(",")(d[1]) })
                    .style('fill', 'white')
                    .attr("x", function (d) { return x(d[0]) + x.rangeBand() / 2; })
                    .attr("y", function (d) { return y(d[1]) - 5; })
                    .attr("text-anchor", "middle");

                function mouseover(d) {  // utility function to be called on mouseover.
                    self.$bus.$emit('select', function(x, name){
                        let start = +d[0].split(" - ")[0];
                        let end = +d[0].split(" - ")[1];
                        if (x["degree"] >= start && x["degree"] < end) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("stroke", "white");

                    // filter for selected state.
                    let st = fData.filter(function (s) { return s.State == d[0]; })[0],
                        nD = d3.keys(st.freq).map(function (s) { return { type: s, freq: st.freq[s] }; }),
                        aD = d3.keys(st.area).map(function (s) { return { type: s, area: st.area[s] }; });

                    // call update functions of pie-chart and legend.    
                    pC.update(nD);
                    leg.update(nD);
                    pC1.update(aD);
                    leg1.update(aD);
                }

                function mouseout(d) {    // utility function to be called on mouseout.
                    // reset the pie-chart and legend. 
                    self.$bus.$emit('restore', function(x, name){
                        let start = +d[0].split(" - ")[0];
                        let end = +d[0].split(" - ")[1];
                        if (x["degree"] >= start && x["degree"] < end) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("stroke", "none");

                    pC.update(tF);
                    leg.update(tF);
                    pC1.update(aF);
                    leg1.update(aF);
                }

                // create function to update the bars. This will be used by pie-chart.
                hG.update = function (nD, color) {
                    // update the domain of the y-axis map to reflect change in frequencies.
                    y.domain([0, d3.max(nD, function (d) { return d[1]; })]);

                    // Attach the new data to the bars.
                    var bars = hGsvg.selectAll(".bar").data(nD);

                    // transition the height and color of rectangles.
                    bars.select("rect").transition().duration(500)
                        .attr("y", function (d) { return y(d[1]); })
                        .attr("height", function (d) { return hGDim.h - y(d[1]); })
                        .attr("fill", color);

                    // transition the frequency labels location and change value.
                    bars.select("text").transition().duration(500)
                        .text(function (d) { return d3.format(",")(d[1]) })
                        .attr("y", function (d) { return y(d[1]) - 5; });
                }
                return hG;
            }

            // function to handle pieChart.
            function pieChart_tF(pD) {
                var pC = {}, pieDim = { w: 250, h: 250 };
                pieDim.r = Math.min(pieDim.w, pieDim.h) / 2;

                // create svg for pie chart.
                var piesvg = d3.select(id[1]).append("svg")
                    .attr("preserveAspectRatio", "xMidYMid meet")
                    .attr("viewBox", "0 0 "+pieDim.w+" "+pieDim.h)
                    // .attr("width", pieDim.w).attr("height", pieDim.h)
                    .append("g")
                    .attr("transform", "translate(" + pieDim.w / 2 + "," + pieDim.h / 2 + ")");

                // create function to draw the arcs of the pie slices.
                var arc = d3.svg.arc().outerRadius(pieDim.r - 10).innerRadius(pieDim.r - 70).cornerRadius(5);

                // create a function to compute the pie slice angles.
                var pie = d3.layout.pie().sort(null).value(function (d) { return d.freq; });

                // Draw the pie slices.
                piesvg.selectAll("path").data(pie(pD)).enter().append("path").attr("d", arc)
                    .each(function (d) { this._current = d; })
                    .style("fill", function (d) { return segColor(d.data.type); })
                    .on("mouseover", mouseover).on("mouseout", mouseout);

                // create function to update pie-chart. This will be used by histogram.
                pC.update = function (nD) {
                    piesvg.selectAll("path").data(pie(nD)).transition().duration(500)
                        .attrTween("d", arcTween);
                }


                // Utility function to be called on mouseover a pie slice.
                function mouseover(d) {
                    self.$bus.$emit('select', function(x, name){
                        let gType = d.data.type;
                        if (x["type"]  == gType) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("transform", "scale(1.05)")
                        .attr("stroke", "white");

                    var filterField = type2field[d.data.type];
                    var field = []
                    for(var key in filterField){
                        field.push({type: key, area: filterField[key]})
                    }
                    pC1.update(field);
                    leg1.update(field);
                    // call the update function of histogram with new data.
                    hG.update(fData.map(function (v) {
                        return [v.State, v.freq[d.data.type]];
                    }), segColor(d.data.type));
                }
                //Utility function to be called on mouseout a pie slice.
                function mouseout(d) {
                    self.$bus.$emit('restore', function(x, name){
                        let gType = d.data.type;
                        if (x["type"]  == gType) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("transform", "scale(1)")
                        .attr("stroke", "none");

                    // call the update function of histogram with all data.
                    hG.update(fData.map(function (v) {
                        return [v.State, v.Count];
                    }), barColor);
                    pC1.update(aF);
                    leg1.update(aF);
                }
                // Animating the pie-slice requiring a custom function which specifies
                // how the intermediate paths should be drawn.
                function arcTween(a) {
                    var i = d3.interpolate(this._current, a);
                    this._current = i(0);
                    return function (t) { return arc(i(t)); };
                }
                return pC;
            }

            // function to handle pieChart.
            function pieChart_aF(pD) {

                var pC1 = {}, pieDim = { w: 250, h: 250 };
                pieDim.r = Math.min(pieDim.w, pieDim.h) / 2;

                // create svg for pie chart.
                var piesvg = d3.select(id[2]).append("svg")
                    .attr("preserveAspectRatio", "xMidYMid meet")
                    .attr("viewBox", "0 0 "+pieDim.w+" "+pieDim.h)
                    // .attr("width", pieDim.w).attr("height", pieDim.h)
                    .append("g")
                    .attr("transform", "translate(" + pieDim.w / 2 + "," + pieDim.h / 2 + ")");

                // create function to draw the arcs of the pie slices.
                var arc = d3.svg.arc().outerRadius(pieDim.r - 10).innerRadius(pieDim.r - 70).cornerRadius(5);

                // create a function to compute the pie slice angles.
                var pie = d3.layout.pie().sort(null).value(function (d) { return d.area; });

                // Draw the pie slices.
                piesvg.selectAll("path").data(pie(pD)).enter().append("path").attr("d", arc)
                    .each(function (d) { this._current = d; })
                    .style("fill", function (d) { return segColor(d.data.type); })
                    .on("mouseover", mouseover).on("mouseout", mouseout);

                // create function to update pie-chart. This will be used by histogram.
                pC1.update = function (nD) {
                    piesvg.selectAll("path").data(pie(nD)).transition().duration(500)
                        .attrTween("d", arcTween);
                }
                // Utility function to be called on mouseover a pie slice.
                function mouseover(d) {

                    self.$bus.$emit('select', function(x, name){
                        let gType = d.data.type;
                        if (x["pos"]  == gType) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("transform", "scale(1.05)")
                        .attr("stroke", "white");

                    var filterType = field2type[d.data.type];

                    var field = [];
                    for(var key in filterType){
                        field.push({type: key, freq: filterType[key]})
                    }
                    pC.update(field);
                    leg.update(field);
                    // call the update function of histogram with new data.
                    hG.update(fData.map(function (v) {
                        return [v.State, v.area[d.data.type]];
                    }), segColor(d.data.type));
                }
                //Utility function to be called on mouseout a pie slice.
                function mouseout(d) {

                    self.$bus.$emit('restore', function(x, name){
                        let gType = d.data.type;
                        if (x["pos"]  == gType) 
                            return false;
                        else 
                            return true;
                    });

                    d3.select(this)
                        .transition()
                        .duration(300)
                        .attr("transform", "scale(1)")
                        .attr("stroke", "none");

                    // call the update function of histogram with all data.
                    hG.update(fData.map(function (v) {
                        return [v.State, v.Count];
                    }), barColor);
                    pC.update(tF);
                    leg.update(tF);
                }
                // Animating the pie-slice requiring a custom function which specifies
                // how the intermediate paths should be drawn.
                function arcTween(a) {
                    var i = d3.interpolate(this._current, a);
                    this._current = i(0);
                    return function (t) { return arc(i(t)); };
                }
                return pC1;
            }

            // function to handle legend.
            function legend_tF(lD) {
                var leg = {};

                // create table for legend.
                var legend = d3.select(id[1])
                    .append("table").attr('class', 'legend');

                // create one row per segment.
                var tr = legend.append("tbody").selectAll("tr").data(lD).enter().append("tr");

                // create the first column for each segment.
                tr.append("td").append("svg").attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3').append("rect")
                    .attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3')
                    .attr("fill", function (d) { return segColor(d.type); });

                // create the second column for each segment.
                tr.append("td").text(function (d) { return d.type; });

                // create the third column for each segment.
                tr.append("td").attr("class", 'legendFreq')
                    .text(function (d) { return d3.format(",")(d.freq); });

                // create the fourth column for each segment.
                tr.append("td").attr("class", 'legendPerc')
                    .text(function (d) { return getLegend(d, lD); });

                // Utility function to be used to update the legend.
                leg.update = function (nD) {
                    // update the data attached to the row elements.
                    var l = legend.select("tbody").selectAll("tr").data(nD);

                    // update the frequencies.
                    l.select(".legendFreq").text(function (d) { return d3.format(",")(d.freq); });

                    // update the percentage column.
                    l.select(".legendPerc").text(function (d) { return getLegend(d, nD); });
                }

                function getLegend(d, aD) { // Utility function to compute percentage.
                    return d3.format("%")(d.freq / d3.sum(aD.map(function (v) { return v.freq; })));
                }

                return leg;
            }

            // function to handle legend.
            function legend_aF(lD) {
                var leg = {};

                var areaMap = {'Top': "上游", 'Mid': "中游", 'Bot': "下游"};

                // create table for legend.
                var legend = d3.select(id[2]).append("table").attr('class', 'legend');

                // create one row per segment.
                var tr = legend.append("tbody").selectAll("tr").data(lD).enter().append("tr");

                // create the first column for each segment.
                tr.append("td").append("svg").attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3').append("rect")
                    .attr("width", '16').attr("height", '16').attr('rx', '3').attr('ry', '3')
                    .attr("fill", function (d) { return segColor(d.type); });

                // create the second column for each segment.
                tr.append("td").text(function (d) { return areaMap[d.type]; });

                // create the third column for each segment.
                tr.append("td").attr("class", 'legendFreq')
                    .text(function (d) { return d3.format(",")(d.area); });

                // create the fourth column for each segment.
                tr.append("td").attr("class", 'legendPerc')
                    .text(function (d) { return getLegend(d, lD); });

                

                // Utility function to be used to update the legend.
                leg.update = function (nD) {
                    // update the data attached to the row elements.
                    var l = legend.select("tbody").selectAll("tr").data(nD);

                    // update the frequencies.
                    l.select(".legendFreq").text(function (d) { return d3.format(",")(d.area); });

                    // update the percentage column.
                    l.select(".legendPerc").text(function (d) { return getLegend(d, nD); });
                }

                function getLegend(d, aD) { // Utility function to compute percentage.
                    return d3.format("%")(d.area / d3.sum(aD.map(function (v) { return v.area; })));
                }

                return leg;
            }

            // calculate total frequency by segment for all state.
            var tF = ['Domain', 'IP', 'Cert', 'ASN'].map(function (d) {
                return { type: d, freq: d3.sum(fData.map(function (t) { return t.freq[d]; })) };
            });
            var aF = ['Top', 'Mid', 'Bot'].map(function (d) {
                return { type: d, area: d3.sum(fData.map(function (t) { return t.area[d]; })) };
            });

            // calculate total frequency by state for all segment.
            var sF = fData.map(function (d) { return [d.State, d.Count]; });

            var hG = histoGram(sF, this), // create the histogram.
                pC = pieChart_tF(tF), // create the pie-chart.
                leg = legend_tF(tF),  // create the legend.
                pC1 = pieChart_aF(aF), // create the pie-chart.
                leg1 = legend_aF(aF);  // create the legend.
        }
    },
    watch: {
        data(newData, oldData) {
            console.log("update:", newData)
            this.dashboard(['#dashboardBar', '#dashboardPie1', '#dashboardPie2'], newData.freqData, newData.type2field, newData.field2type);
        }
    }
}

</script>

<style>
    @import "./style.css";
</style>