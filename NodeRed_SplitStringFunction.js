var output = msg.payload.split(",")

//var arr = msg.payload.replace( /[\(\)]/g, "" ).split( /\s*,\s*/ );

var temp = parseFloat(output[0]);
var humidity = parseFloat(output[1]);


var Temprature = {payload:temp};
var Humidity = {payload:humidity};

return [Temprature,Humidity];
