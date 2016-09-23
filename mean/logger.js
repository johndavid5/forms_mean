var log4js = require('./lib/log4js-node/lib/log4js');

var util = require('util');

//util.inherits(Log4jsPlus, log4js);

//function Log4jsPlus(){
//	function info(msg){	
//		log4js.call(info, "[" + process.pid	+ "]" + msg );
//	}
//}

// Turn off colors...looks goofy in file...
//log4js.configure({ appenders: [ { type: "console", layout: { type: "basic" } } ], replaceConsole: true })

log4js.configure({ appenders: [ { type: "console" } ], replaceConsole: true })

var logger = log4js.getLogger();

logger.setLevel('INFO');

//var logger = Log4jsPlus.getLogger();

module.exports = logger;
