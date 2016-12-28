// To read input from terminal
process.stdin.resume();
process.stdin.setEncoding('utf8');
var util = require('util');

// funcionalidades da biblioteca 'node-bebop'
var bebop = require('node-bebop');
var drone = bebop.createClient();

drone.connect(
    function() {
        
        console.log(  "1- takeoff()     7- down()       \n"
                    + "2- land()        8- rightFlip()  \n"
                    + "3- stop()        9- leftFlip()      \n"
                    + "4- left()        q- goFront()    \n"
                    + "5- right()       w- goBack()     \n"
                    + "6- up()          e- done()      \n"
                    + " \n"  );
        
        process.stdin.on('data', function (cmd) {
            
            console.log('received data:', util.inspect(cmd));

                
                if(cmd === "1\n")
                    takeoff();
                else if(cmd === "2\n")
                    land();
                else if(cmd === "3\n")
                    stop();
                else if(cmd === "4\n")
                    goLeft();
                else if(cmd === "5\n")
                    goRight();
                else if(cmd === "6\n")
                    goUp();
                else if(cmd === "7\n")
                    goDown();
                else if(cmd === "8\n")
                    rightFlip();
                else if(cmd === "9\n")
                    leftFlip();
                else if(cmd === "q\n")
                    goFront();
                else if(cmd === "w\n")
                    goBack();
                else if(cmd === "e\n")
                    done();
                else
                    land();
        });

});

// basic functions


function takeoff() {
    drone.takeoff();
    console.log('takeoff()');
}

function land() {
    drone.land();
    console.log('land()');
}

function stop() {
    drone.stop();
    console.log('stop()');
}

// Left Side

function leftFlip() {
    drone.leftFlip();
    console.log('going left(10)');
}

function rightFlip() {
    drone.rightFlip();
    console.log('going right(10)');
}

function goUp() {
    drone.up(10);
    console.log('going up(10)');
}

function goDown() {
    drone.down(10);
    console.log('going down(10)');
}

// Right Side

function goLeft() {
    drone.left(10);
    console.log('left(10)');
}

function goRight() {
    drone.right(10);
    console.log('right(10)');
}

function goFront() {
    drone.forward(10)
    console.log('forward(10)');
}

function goBack() {
    drone.backward(10);
    console.log('backward(10)');
}


function done() {
    console.log('Now that process.stdin is paused, there is nothing more to do.');
    process.exit();
}
