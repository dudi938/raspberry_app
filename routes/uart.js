/**
 * create on: 16.3.2017. 
 * create by: David G.
 * description: this module route all uart requests.-
 */


var express = require('express');
var router = express.Router();
var fs = require("fs");
var path = require('path');
var bodyParser = require('body-parser');
var SerialPort = require("serialport");

var speed = 0;

var RS232_PROTOCOL_CMD_ACK = "AT";
var RS232_PROTOCOL_CMD_ERROR = "ERROR";
var RS232_PROTOCOL_CMD_ROBOT_GO_FORWARD = "AT+GF";
var RS232_PROTOCOL_CMD_ROBOT_GO_BACKWARD = "AT+GB";
var RS232_PROTOCOL_CMD_ROBOT_STOP = "AT+ST";



router.use(bodyParser.json()); // for parsing application/json


//open serial port
var port = new SerialPort("/dev/ttyUSB0", {
  baudRate: 115200
});


/**
 * create on: 20.3.2017. 
 * create by: David G.
 * description: parse cmd op code
 * args: string cmd
 * return: at cmd to send to the ST MCU
 */
function parseRequestCommand(cmd, power){
	switch (cmd){
		
		case "robot_go_forward":
			var pow = 50;
			if(power !== null)
				pow = power
			return RS232_PROTOCOL_CMD_ROBOT_GO_FORWARD + '=' + pow;
		break;
		case "robot_go_backward":
			return RS232_PROTOCOL_CMD_ROBOT_GO_BACKWARD;
		break;
		case "robot_stop":
			return RS232_PROTOCOL_CMD_ROBOT_STOP;
		break;
		default:
			return RS232_PROTOCOL_CMD_ERROR;
		break;				
	}
}



/**
 * create on: 15.3.2017. 
 * create by: David G.
 * description: log every request from this module.
 * args: -
 * return: -
 */
router.all('*', function (req, res, next) {
	console.log(path.basename(module.filename) +', ' + req.url);
	next();
});



/**
 * create on: 16.03.2017. 
 * create by: David G.
 * description: send data over the serial port
 * args: -
 * return: - 
 */
router.get('/robot_cmd', function (req, res) {
	
	 port.write(parseRequestCommand(req.param("cmd"), req.param("power")) + '\r\n' ,function(err){
		 console.log('com_error\r\n\r\n' + err);
		}
	 );	 
	res.send(true);	
});



/**
 * create on: 16.03.2017. 
 * create by: David G.
 * description: start PWM in pin 12.
 * args: -
 * return: scenario array 
 */
router.get('/get/speed', function (req, res) {
 
	res.send('Current speed is :' + speed);
	
});


/**
 * create on: 16.03.2017. 
 * create by: David G.
 * description: listen to serial port
 * args: -
 * return: -
 */
port.on('data', function (data) {
    console.log('Data: ' + data);
  
	speed  = data;
	
});


module.exports = router;