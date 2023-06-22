var data = [];                  // JSON data

// ManiVault invokes this function to set the plot data,
// we set up the plot immedeatly afterwards
function setChartData(d) {
    log("ExampleViewJS: radar_chart.tools.js: setting data");
    data = d;

    drawChart()
}

// removes previous charts, plots a new one and sets of selection notifications
function drawChart() {
    if(data.length == 0)
        return

    log("drawChart")

    // remove possible old chart 
    d3.select("div#container").select("svg").remove();
    
    // config chart
    var chart = RadarChart.chart();
    chart.config({
        containerClass: 'radar-chart',
        w: window.innerWidth,
        h: window.innerHeight,
    });

    // plot chart in auto-resizable box
    d3.select("div#container")
      .append("svg")
      .attr("preserveAspectRatio", "xMinYMin meet")
      .attr("viewBox", "0 0 " + window.innerWidth + " " + window.innerHeight)
      .classed("svg-content", true)
      .datum(data)
      .call(chart);

    // Pass selected item ID to ManiVault
    // Depending on the JS library, the event handling will differ
    // In this example, we may only select one item at a time and
    // pass a string of the form "point 2" to ManiVault
    // Just as with the data transfer, it is also possible to send
    // an array like [0, 2, 4, 765] containing only IDs
    d3.select("div#container").select("svg").selectAll("polygon").on('click', function (dd) {
        d3.event.stopPropagation();
        passSelectionToQt([chart.config().tooltipFormatClass(dd.className)])
    });

    // In this example, we de-select by clicking anywhere outside of the
    // data point polygons and pass an empty selection array to ManiVault
    d3.selectAll("div#container").select("svg").on('click', function (event) { passSelectionToQt([]) });
}

