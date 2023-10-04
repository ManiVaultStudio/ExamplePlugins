var isQtAvailable = false;

// Here, we establish the connection to Qt
// Any signals that we want to send from ManiVault to
// the JS side have to be connected here
try {
    new QWebChannel(qt.webChannelTransport, function (channel) {
        // Establish connection
        QtBridge = channel.objects.QtBridge;

        // register signals
        QtBridge.qt_js_setDataAndPlotInJS.connect(function () { drawChart(arguments[0]); });   // drawChart is defined in radar_chart.tools.js

        // confirm successful connection
        isQtAvailable = true;
        notifyBridgeAvailable();
    });
} catch (error) {
	log("ExampleViewJSPlugin: qwebchannel: could not connect qt");
}

// The slot js_available is defined by ManiVault's WebWidget and will
// invoke the initWebPage function of our web widget (here, ChartWidget)
function notifyBridgeAvailable() {

    if (isQtAvailable) {
        QtBridge.js_available();
    }
    else {
        log("ExampleViewJSPlugin: qwebchannel: QtBridge is not available - something went wrong");
    }

}

// The QtBridge is connected to our WebCommunicationObject (ChartCommObject)
// and we can call all slots defined therein
function passSelectionToQt(dat) {
    if (isQtAvailable) {
        QtBridge.js_qt_passSelectionToQt(dat);
	}
}

// utility function: pipe errors to log
window.onerror = function (msg, url, num) {
    log("ExampleViewJSPlugin: qwebchannel: Error: " + msg + "\nURL: " + url + "\nLine: " + num);
};

// utility function: auto log for Qt and console
function log(logtext) {

    if (isQtAvailable) {
        QtBridge.js_debug(logtext.toString());
    }
    else {
        console.log(logtext);
    }
}
