let publicPath = process.env.NODE_ENV === 'production' ? 'Industry_Analysis/' : '/';

module.exports = {
    publicPath,
    productionSourceMap: false,
    chainWebpack: config => {
        config.resolve.alias.set('vue', '@vue/compat')

        config.module
            .rule('vue')
            .use('vue-loader')
            .tap(options => {
                return {
                    ...options,
                    compilerOptions: {
                        compatConfig: {
                            MODE: 2
                        }
                    }
                }
            })
    },
    lintOnSave: false
};