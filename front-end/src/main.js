// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue';
import BootstrapVue from 'bootstrap-vue';
import * as VueGoogleMaps from 'vue2-google-maps';
import Toasted from 'vue-toasted';
import VCalendar from 'v-calendar';
import VueApexCharts from 'vue-apexcharts';

import store from './store';
import router from './Routes';
import App from './App';
import layoutMixin from './mixins/layout';

import api from "./interface/index"
Vue.prototype.$api = api.commonUrl

import VNetworkGraph from "v-network-graph"
import "v-network-graph/lib/style.css"

Vue.use(VNetworkGraph)

// import VueCompositionAPI from '@vue/composition-api'
// Vue.use(VueCompositionAPI)

Vue.use(BootstrapVue);
Vue.use(VCalendar, {
    firstDayOfWeek: 2
});
Vue.use(VueGoogleMaps, {
    load: {
        key: 'AIzaSyB7OXmzfQYua_1LEhRdqsoYzyJOPh9hGLg',
    },
});

Vue.component('apexchart', VueApexCharts);
Vue.mixin(layoutMixin);
Vue.use(Toasted, { duration: 10000 });

Vue.config.productionTip = false;

Vue.prototype.$bus = new Vue()

/* eslint-disable no-new */
new Vue({
    el: '#app',
    store,
    router,
    render: h => h(App),
});