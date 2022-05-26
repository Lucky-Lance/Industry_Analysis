<template>
<div>
    <p></p>
    <p>
        <span class="circle bg-primary text-white"><i class="la la-search" /></span> &nbsp;
        本图中共有 <span class='fw-semi-bold'>&nbsp;{{ data.nodeNum }}</span> 个节点，<span class='fw-semi-bold'>&nbsp;{{ data.edgeNum }}</span>  条连边。
    </p>
    <br/>
    <div class="row progress-stats">
        <div class="col-9">
            <h6 class="name">相对规模</h6>
            <p></p>
            <!-- <p class="description deemphasize mb-xs">P. to C. Conversion</p> -->
            <b-progress variant="primary" :value="data.size" :max="100" class="progress-xs" />
        </div>
        <div class="col-3 text-center">
            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                <span>
                    <AnimatedNumber :value="data.size" v-bind="animateNumberOptions"></AnimatedNumber>%
                </span>
            </span>
        </div>
    </div>
    <br/>
    <div class="row progress-stats">
        <div class="col-9">
            <h6 class="name">黑产节点占比</h6>
            <p></p>
            <!-- <p class="description deemphasize mb-xs">Some Cool Text</p> -->
            <b-progress variant="danger" :value="data.darkPercent" :max="100" class="progress-xs" />
        </div>
        <div class="col-3 text-center">
            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                <span>
                    <AnimatedNumber :value="data.darkPercent" v-bind="animateNumberOptions"></AnimatedNumber>%
                </span>
            </span>
        </div>
    </div>
    <br/>
    <div class="row progress-stats">
        <div class="col-9">
            <h6 class="name">核心资产占比</h6>
            <p></p>
            <!-- <p class="description deemphasize mb-xs">Average Bitrate</p> -->
            <b-progress variant="success" :value="data.corePercent" :max="100" class="progress-xs" />
        </div>
        <div class="col-3 text-center">
            <span class="status rounded rounded-lg bg-widget mt-lg-0 mt-4">
                <span>
                    <AnimatedNumber :value="data.corePercent" v-bind="animateNumberOptions"></AnimatedNumber>%
                </span>
            </span>
        </div>
    </div>
    <br/>
    <h5 class="fw-semi-bold mt">黑产网络概述</h5>
    <p>该黑产网络是一个以 <span class='fw-semi-bold'>&nbsp;{{data.coreService}}</span> 业务为核心的 <span class='fw-semi-bold'>&nbsp;{{data.type}}</span> 黑产网络。</p>
</div>
    
</template>

<script>
import AnimatedNumber from "animated-number-vue";

export default {
    name: "Abstract",
    components: {
         AnimatedNumber
    },
    data () {
        return {
            animateNumberOptions: {
                duration: 2000,
                easing: 'easeInQuad',
                formatValue(value) {
                    return value.toFixed(0);
                }
            }
        }
    },
    props: ["data"]
}


</script>