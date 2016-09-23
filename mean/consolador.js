/*
* Override console.log() to pre-pend (<pid>) to message...
*
* Many thanks to Arun P Johny at
*   http://stackoverflow.com/questions/16259711/how-can-i-override-console-log-and-append-a-word-at-the-beginning-of-the-outpu
*/
(function(){
    //if(window.console && console.log){
    if(console.log){
        var old = console.log;
        console.log = function(){
            Array.prototype.unshift.call(arguments, '(' + process.pid + ')');
            old.apply(this, arguments)
        }
    }  
})();

/*
console.log("process.pid = ", process.pid );
console.log("Let off some steam, Bennett!");
console.log("Let off some ", "steam", ", Bennett!");
*/
