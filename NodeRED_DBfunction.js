
var output = msg.payload.split(",")

var temp = parseFloat(output[0]);
var humidity = parseFloat(output[1]);

var tempN = parseFloat(output[0])+  String.fromCharCode(176) +"C";
var humidityN = parseFloat(output[1])+ "%";

//var sqliteTimeStamp =  CURRENT_TIMESTAMP;
var today = new Date();
var date = today.getFullYear()+'-'+(today.getMonth()+1)+'-'+today.getDate();
var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
var dateTime = date+" "+time;
console.log ("...................DATE....." + dateTime);
//var sqliteTimeStamp = Math.round(Date.now() / 1000);
//var originalDate = Math.round (new Date((data.origDate + 978307200) * 1000))

var theDevice = "DHTSensor";

var RValue = 0;
var Rnew = 0.0;
if(temp >= 15 && temp<50 && humidity>20) 
    {
        RValue = (humidity-20)/2.2;
        Rnew = RValue.toFixed(2);
        
        console.log("....Mold>>>>cond1>>>>" + Rnew);
        
    }
    if(temp>=0 && temp<15 && humidity>40)
    {
        RValue = (humidity-40)*(temp/15)/2.2;
        Rnew = RValue.toFixed(2);
       
        console.log("....Mold>>>>cond2>>>>" + Rnew);
    }
//In if statement conditions actual humidity value should be 78 according to formula,
//but to get some values and to check the difference in result, I took minimum humidity values 20 and 40.
        
console.log("....Mold....." + Rnew);
console.log("....TEMP...." +tempN);
console.log("....HUMIDITY...." +humidityN);
//console.log("...._Date...." +sqliteTimeStamp);

var theSQL = "INSERT INTO DHTR(DateTime,Sensor,Temprature,Humidity,MoldIndex) VALUES"
theSQL = theSQL + "('" + dateTime + "','"+ theDevice + "','" + tempN + "','"+ humidityN + "','"+ Rnew + "');";
msg.topic = theSQL;
return msg;