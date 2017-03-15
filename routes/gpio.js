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
 * description: edit unit.
 * args: room.
 * return: bool success 
 */
router.post('/editUnit', function (req, res) {
	/* TODO 
	 *  SaveUnitOnDB(req.body);
	 * */	
	res.send(true);
});







module.exports = router;