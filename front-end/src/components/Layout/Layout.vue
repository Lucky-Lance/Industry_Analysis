<template>
<div>
  <Header />
  <div :class="{root: true, sidebarClose}">
    <!-- <Helper /> -->
    <!-- <Sidebar /> -->
    <div ref="content" class="content animated fadeInUp">
      <transition name="router-animation">
        <router-view />
      </transition>
    </div>
    <footer class="contentFooter">
      ChinaVis2022 数据可视化竞赛参赛作品，由陆徐东，陈文迪，方天宬和陈文浩共同制作。
    </footer>
  </div>
</div>
</template>

<script>
import { mapState, mapActions } from 'vuex';

import Sidebar from '@/components/Sidebar/Sidebar';
import Header from '@/components/Header/Header';
import Helper from '@/components/Helper/Helper';

import './Layout.scss';

export default {
  name: 'Layout',
  components: { Sidebar, Header, Helper },
  methods: {
    ...mapActions(
      'layout', ['switchSidebar', 'changeSidebarActive'],
    ),
  },
  computed: {
    ...mapState('layout', {
      sidebarClose: state => state.sidebarClose,
    }),
  },
  created() {
  },
  mounted() {
    this.$refs.content.addEventListener('animationend', () => {
      this.$refs.content.classList.remove('animated');
      this.$refs.content.classList.remove('fadeInUp');
    });
  }
};
</script>

<style src="./Layout.scss" lang="scss" />
