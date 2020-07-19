var functions = [];
var fn = function() {return i};
for (var i = 0; i < 10; i++) {
    
    functions.push(fn);
}
functions.forEach(function(fn) {console.log(fn())})