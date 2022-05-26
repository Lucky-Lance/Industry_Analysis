<template>
    <div>
        <div id="mynetwork"></div>
        <div class="menu" id="divHoverNode" style="display: none;"></div>
    </div>
</template>

<script>
    const vis = require('vis-network/dist/vis-network.min.js')
    require('vis-network/dist/dist/vis-network.min.css')
    import $ from 'jquery'

    export default {
        name: 'MainGraph',
        props: ["data"],
        mounted() {
            this.create('mynetwork', this.data.allNodes, this.data.allEdges);
        },
        methods: {
            create(id, allNodes, allEdges) {
                // 获取容器
                var container = document.getElementById(id);
                // 将数据赋值给vis 数据格式化器
                this.nodes = new vis.DataSet(allNodes);
                this.edges = new vis.DataSet(allEdges);
                var data = {
                    nodes: this.nodes,
                    edges: this.edges
                };
                var options = {
                    nodes: {
                        shape: "dot",
                        borderWidth: 2,
                        scaling: {
                            customScalingFunction: function (min, max, total, value) {
                                return value / total;
                            },
                            min: 5,
                            max: 40,
                        },
                        shadow: true,
                        font: {
                            color: "#f4f4f5"
                        }
                    },
                    edges: {
                        width: 2,
                        shadow: true,
                    },
                    interaction: {
                        hover: true,
                        hoverConnectedEdges: true
                    },
                };
                this.network = new vis.Network(container, data, options);
                var initial_delele = []
                for (let i = 0; i < allNodes.length; i++) {
                    let node_id = allNodes[i].id;
                    let flag = false;
                    var sub_nodes = this.get_directly_sub_nodes(node_id, allNodes);
                    if (sub_nodes.length == 0) {
                        continue;
                    }
                    for (let j = 0; j < initial_delele.length; j++) {
                        if (node_id == initial_delele[j]) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag == true) {
                        continue;
                    }
                    this.remove_all_sub_nodes(node_id, allNodes);
                    initial_delele.push(node_id);
                    for (let j = 0; j < sub_nodes.length; j++) {
                        initial_delele.push(sub_nodes[j]);
                    }
                }

                let hoverNode = (properties) => {
                    var hoveNodeList = this.getNode(properties.node, allNodes);
                    // console.log('hoveNodeList',hoveNodeList);
                    var deviceType = hoveNodeList.type;

                    if (deviceType == "Parent") {
                        let $ul = "<ul>"
                            + "<li>" + hoveNodeList.industry + "</li>"
                            + "</ul>";
                        $("#divHoverNode").append($ul);
                    } else {
                        let $ul = "<ul>"
                            // +"<li><img src=' "+imgPathSrc+" ' width='30px' height='25px'><span> 设备类型:"+hoveNodeList.name+" </span> </li>"
                            + "<li>" + hoveNodeList.industry + "</li>"
                            + "<li>Type:" + hoveNodeList.type + "</li>"
                            + "<li>Count:" + hoveNodeList.value + "</li>"
                            + "</ul>";
                        $("#divHoverNode").append($ul);
                    }
                    $('#divHoverNode').css({
                        'display': 'block',
                        'left': properties.event.offsetX + 15,
                        'top': properties.event.offsetY + 15
                    });
                    $('#menuOperation').hide();
                }

                let hoverEdge = (properties) => {
                    // console.log('悬停边',properties);
                    // var hoveEdgeList = getEdge(properties.edge);
                    // console.log('hoveEdgeList',hoveEdgeList);
                    let $ul = "<ul>"
                        + "</ul>";
                    $("#divHoverNode").append($ul);
                    $('#divHoverNode').css({
                        'display': 'block',
                        'left': properties.event.offsetX + 15,
                        'top': properties.event.offsetY + 15
                    });
                    $('#menuOperation').hide();
                }

                let click = (properties) => {
                    var clickNodeList = this.getNode(properties.nodes[0], allNodes);
                    // console.log('clickNodeList',clickNodeList);
                    if (typeof (clickNodeList) == "undefined") {
                        $('#menuOperation').hide();
                    } else {
                        $('#menuOperation').css({
                            'display': 'block',
                            'left': properties.event.center.x + 15,
                            'top': properties.event.center.y + 15
                        });
                        $("#divHoverNode").hide();
                    }
                }

                let doubleClick = (params) => {
                    if (params.nodes.length != 0) {//确定为节点双击事件
                        var click_node_id = params.nodes[0];
                        if (+click_node_id === 1)
                            this.$emit("choose_graph_id", 0);
                        else
                            this.$emit("choose_graph_id", 1);
                        console.log("click:", click_node_id);
                        this.remove_all_sub_nodes(click_node_id, allNodes);
                        
                    }
                }

                //todo  悬停在节点上--显示弹框
                this.network.on('hoverNode', hoverNode);

                //todo  从节点移开---隐藏弹框
                this.network.on('blurNode', function () {
                    $("#divHoverNode").hide();
                    $("#divHoverNode").empty();//移除之后清空div
                });

                //todo  悬停在边上--显示弹框
                this.network.on('hoverEdge', hoverEdge);
                
                //todo  从边上移开---隐藏弹框
                this.network.on('blurEdge', function () {
                    $("#divHoverNode").hide();
                    $("#divHoverNode").empty();//移除之后清空div
                });

                //todo  点击的判断是否选中节点时候显示隐藏
                this.network.on('click', click);
                //todo 双击时折叠和展开
                this.network.on("doubleClick", doubleClick);
            },
            getNode(option, allNodes) {
                for (var i = 0; i < allNodes.length; i++) {
                    if (option == allNodes[i].id) {
                        return allNodes[i];
                    }
                }
            },
            // todo 删除下级所有节点
            remove_all_sub_nodes(node_id, allNodes) {
                var sub_nodes = this.get_directly_sub_nodes(node_id, allNodes);
                if (sub_nodes.length == 0) {//当前点击的为叶子节点
                    //判断是否有下级节点
                    if (typeof (allNodes[node_id - 1]['subids']) != 'undefined') {
                        let worker = (index, item) => {
                            this.nodes.add(allNodes[item - 1]);
                            this.edges.add({ id: node_id + '_' + item, from: node_id, to: item });
                        }
                        $.each(allNodes[node_id - 1]['subids'], worker);
                    } else {
                        // alert('当前为叶子节点');
                    }
                } else {
                    let worker = (index, item) => {
                        var sub_sub_nodes = this.get_directly_sub_nodes(item, allNodes);
                        if (sub_sub_nodes.length == 0) {
                            this.nodes.remove({ id: item });
                            this.edges.remove({ id: node_id + '_' + item });
                        } else {
                            this.remove_all_sub_nodes(item, allNodes);
                        }
                        this.nodes.remove({ id: item });
                        this.edges.remove({ id: node_id + '_' + item });
                    }
                    $.each(sub_nodes, worker);
                }
            },
            //todo 获取某节点直属下级节点
            get_directly_sub_nodes(node_id, allNodes) {
                var return_nodes = [];
                var connectedNodes =this.network.getConnectedNodes(node_id);//获取所有连接节点
                let worker = (index, item) => {
                    if (item != allNodes[node_id - 1]['pid']) {//当前节点的父节点 ，不操作
                        return_nodes.push(item);
                    }
                }
                $.each(connectedNodes, worker);
                return return_nodes;
            }
        },
        watch: {
            data(newData, oldData) {
                this.create('mynetwork', newData.allNodes, newData.allEdges);
            }
        }
    }


</script>

<style>
    @import "./style.css";
</style>