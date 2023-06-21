var data = [];                  // JSON data
var chart = RadarChart.chart(); // char object

// parses JSON string data and radar plot
function setChartData(d) {
    log("ExampleViewJS: radar_chart.tools.js: setting data");
    data = d;

    redrawChart()
}

function redrawChart() {
    // resize
    chart.config({
        containerClass: 'radar-chart',
        w: window.innerWidth,
        h: window.innerHeight,
        factor: 0.95,
    });

    RadarChart.draw("#chartBody", data);
}

// highlights selection from qt
function setSelectionIDsFromQt(IDs) {

}

window.onresize = function () {
    redrawChart()
};