<template>
    <br/>
    <ve-table :columns="data.columns" 
            :table-data="data.tableData" 
            :max-height="300"
            :border-around="false"
            :event-custom-option="eventCustomOption"/>
</template>

<script>

import { VeTable} from "vue-easytable";

export default {
    name: 'PathTable',
    components: {VeTable},
    props: ["data", "rawData"],
    data() {
        return {
             eventCustomOption: {
                bodyRowEvents: ({ row, rowIndex }) => {
                    return {
                        mouseenter: (event) => {
                            let nodes = new Set(this.rawData.pathNodes[rowIndex]);
                            self.$bus.$emit('select', function(x, name){
                                if ( nodes.has(name))          
                                    return false;
                                else 
                                    return true;
                            });
                        },
                        mouseleave: (event) => {
                            let nodes = new Set(this.rawData.pathNodes[rowIndex]);
                            self.$bus.$emit('restore', function(x, name){
                                if ( nodes.has(name)) 
                                    return false;
                                else 
                                    return true;
                            });
                        },
                    };
                },
            },
        }
    }
}
</script>

<style>
    @import "./style.css";
</style>