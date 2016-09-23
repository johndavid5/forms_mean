/**
*
* Example: A Counting Stream
*
* This is a basic example of a Readable stream.
* It emits the numerals from 1 to 100 in ascending order, and then ends.
*
* https://nodejs.org/api/stream.html#stream_class_stream_writable
*/
var Readable = require('stream').Readable;
var util = require('util');

util.inherits(ReadableCounter, Readable);

function ReadableCounter(opt) {

	var sWho = "ReadableCounter";

	Readable.call(this, opt);

 	console.log( sWho + "(): BEFORE: this = ", this, ", opt = ", opt, "...");

	this._max = 100;

	if( opt.max ){
		this._max = 1 * opt.max; /* coerce to int */
	}

	this._index = 1;

 	console.log( sWho + "(): AFTER: this = ", this, ", opt = ", opt, "...");

}/* ReadableCounter(opt) */

ReadableCounter.prototype._read = function() {

	var i = this._index++;

	if (i > this._max){
		this.push(null); // That's all she wrote...
	}
	else {
		var str = '' + i + "\n";
		var buf = new Buffer(str, 'ascii');
		this.push(buf);
	}
};

module.exports = ReadableCounter;
