<template>
    <ve-table :columns="data.columns" 
            :table-data="data.tableData" 
            :max-height="300"
            :border-around="false"/>
</template>

<script>

import { VeTable} from "vue-easytable";

export default {
    name: 'PathTable',
    components: {VeTable},
    props: ["data"]
}
</script>