var data = [];                  // JSON data
var chart = RadarChart.chart(); // char object
var cfg = chart.config();       // retrieve default config
chart.config({
    containerClass: 'radar-chart',
    w: window.innerWidth,
    h: window.innerHeight,
    factor: 0.95,
});

var svg = d3.select("div#container")
  .append("svg")
  .attr("preserveAspectRatio", "xMinYMin meet")
  .attr("viewBox", "0 0 " + window.innerWidth + " " + window.innerHeight)
  .classed("svg-content", true);

// parses JSON string data and radar plot
function setChartData(d) {
    log("ExampleViewJS: radar_chart.tools.js: setting data");
    data = d;

    drawChart()
}

function drawChart() {
    if(data.length == 0)
        return

    log("drawChart")

    // svg.selectAll('*').remove();
    svg.append('g').classed('single', 1).datum(data).call(chart);  
}

// highlights selection from qt
function setSelectionIDsFromQt(IDs) {

}
