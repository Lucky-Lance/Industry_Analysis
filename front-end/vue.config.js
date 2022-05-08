let publicPath = process.env.NODE_ENV === 'production' ? 'Industry_Analysis/' : '/';

module.exports = {
  publicPath,
  productionSourceMap: false,
};
