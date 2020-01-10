const HtmlWebpackPlugin = require("html-webpack-plugin");
const path = require("path");

module.exports = {
	mode: "development",
	entry: "./src/index.ts",
	module: {
		rules: [
			{
				test: /\.tsx?$/,
				use: "ts-loader",
				exclude: /node_modules/
			},
			{
				test: /\.css$/i,
				use: ["style-loader", "css-loader"]
			},
			{
				test: /\.(glsl|vert|frag)$/,
				use: "ts-shader-loader"
			}
		]
	},
	resolve: {
		extensions: [".tsx", ".ts", ".js"]
	},
	output: {
		path: path.resolve(__dirname, "./dist"),
		filename: "bundle.js"
	},
	devServer: {
		contentBase: "./dist"
	},
	plugins: [
		new HtmlWebpackPlugin({
			template: "src/index.html"
		})
	]
};
