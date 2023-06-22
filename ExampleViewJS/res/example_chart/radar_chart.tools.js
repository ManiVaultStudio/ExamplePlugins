var data = [];                  // JSON data

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

    // remove possible old chart
    d3.select("div#container").select("svg").remove();
    
    // config new chart
    var chart = RadarChart.chart(); // char object
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

    d3.select("div#container").select("svg").selectAll("polygon").on('click', function (dd) {
        d3.event.stopPropagation();
        passSelectionToQt([chart.config().tooltipFormatClass(dd.className)])
    });
}

function handleClick() {
    passSelectionToQt([])
}

d3.selectAll("div#container").on('click', handleClick);
