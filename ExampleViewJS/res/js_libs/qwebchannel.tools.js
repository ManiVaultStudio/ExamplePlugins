var isQtAvailable = false;

// utility function: auto log for Qt and console
function log(logtext) {

	if (isQtAvailable) {
		QtBridge.js_debug(logtext.toString());
	}
	else {
		console.log(logtext);
	}
}

// find out if the QtBridge is available
try {
    new QWebChannel(qt.webChannelTransport, function (channel) {
        // Establish connection
        QtBridge = channel.objects.QtBridge;

        // register signals 
        QtBridge.qt_js_setDataInJS.connect(function () { setChartData(arguments[0]); });

        // confirm successfull connection
        isQtAvailable = true;
        notifyBridgeAvailable();
    });
} catch (error) {
	log("ExampleViewJSPlugin: qwebchannel: could not connect qt");
}

// 
function notifyBridgeAvailable() {

    if (isQtAvailable) {
        QtBridge.js_available();
    }
    else {
        log("ExampleViewJSPlugin: qwebchannel: QtBridge is not available - something went wrong");
    }

}

// 
function passSelectionToQt(dat) {
    if (isQtAvailable) {
        QtBridge.js_qt_passSelectionToQt(dat);
	}
}

// utility function: pipe errors to log
window.onerror = function (msg, url, num) {
    log("ExampleViewJSPlugin: qwebchannel: Error: " + msg + "\nURL: " + url + "\nLine: " + num);
};

