/**
 * create on: 15.3.2017. 
 * create by: David G.
 * description: this module route all gpio requests.-
 */


var express = require('express');
var router = express.Router();
var fs = require("fs");
var path = require('path');
var bodyParser = require('body-parser');
var gpio = require('rpi-gpio');
var rpio = require('rpio');

//var data_base = require(path.join(__dirname, '../data_models/data_base.js'));

router.use(bodyParser.json()); // for parsing application/json


var pin = 3;
var state = false;

function write() {
    gpio.write(pin, true, function(err) {
    	
        if (err) {throw err;}
        
        console.log('Written to pin');
    });
}


function toggle(pin) {
		
    gpio.write(pin, state, function(err) {
    	
        if (err) {throw err;}
        
        console.log('Written  '+ state + ' to pin');
    	if(!state){state = true;}else{stste = false;}

    });   
}



function writeOn(pin) {
	
    gpio.write(pin, true, function(err) {
    	
        if (err) {throw err;}
        
        console.log('Written  '+ true + ' to pin');
    });   
}

function writeOff(pin) {
	
    gpio.write(pin, false, function(err) {
    	
        if (err) {throw err;}
        
        console.log('Written  '+ false + ' to pin');
    });   
}


function startPWM(){

var rpio = require('rpio');

var pin = 12;           /* P12/GPIO18 */
var range = 1024;       /* LEDs can quickly hit max brightness, so only use */
var max = 128;          /*   the bottom 8th of a larger scale */
var clockdiv = 8;       /* Clock divider (PWM refresh rate), 8 == 2.4MHz */
var interval = 5;       /* setInterval timer, speed of pulses */
var times = 5;          /* How many times to pulse before exiting */

/*
 * Enable PWM on the chosen pin and set the clock and range.
 */
rpio.open(pin, rpio.PWM);
rpio.pwmSetClockDivider(clockdiv);
rpio.pwmSetRange(pin, range);

/*
 * Repeatedly pulse from low to high and back again until times runs out.
 */
var direction = 1;
var data = 0;
var pulse = setInterval(function() {
        rpio.pwmSetData(pin, data);
        if (data === 0) {
                direction = 1;
                if (times-- === 0) {
                        clearInterval(pulse);
                        rpio.open(pin, rpio.INPUT);
                        return;
                }
        } else if (data === max) {
                direction = -1;
        }
        data += direction;
}, interval, data, direction, times);
	
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
 * create on: 18.10.2016. 
 * create by: David G.
 * description: get all units.
 * args: -
 * return: scenario array 
 */
router.get('/set/on', function (req, res) {
	
	console.log('state = ' + state);
	
	//init
	gpio.setup(pin, gpio.DIR_OUT, function(){
		writeOn(pin);
	});
	res.send(true);
});


/**
 * create on: 18.10.2016. 
 * create by: David G.
 * description: get all units.
 * args: -
 * return: scenario array 
 */
router.get('/set/off', function (req, res) {
	
	console.log('state = ' + state);
	
	//init
	gpio.setup(pin, gpio.DIR_OUT, function(){
		writeOff(pin);
	});
	res.send(true);
});



/**
 * create on: 18.10.2016. 
 * create by: David G.
 * description: start PWM in pin 12.
 * args: -
 * return: scenario array 
 */
router.get('/start/pwm', function (req, res) {
	
	startPWM();
	res.send(true);
	
});








module.exports = router;