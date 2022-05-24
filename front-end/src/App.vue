<template>
  <router-view />
</template>

<script>
export default {
  name: 'App',
  created() {
    const currentPath = this.$router.history.current.path;

    if (window.localStorage.getItem('authenticated') === 'false') {
      this.$router.push('/login');
    }

    if (currentPath === '/app') {
      this.$router.push('/app/dashboard');
    }

    if (currentPath === '/') {
      this.$router.push('/analysis');
    }

  },
};
</script>

<style src="./styles/theme.scss" lang="scss" />
