"use strict";

var bebop = require('node-bebop');

var drone = bebop.createClient();

drone.connect(function () {
    
    console.log("drone connected");


    var obj = { filepath: "flightPlan.mavlink", type: flightPlan };
    
    drone.Mavlink.start(obj);
    //https://raw.githubusercontent.com/Parrot-Developers/libARCommands/master/Xml/common_commands.xml
    //<arg name="type" type="enum">
    //            type of the played mavlink file
    //            <enum name="flightPlan">
    //                Mavlink file for FlightPlan
    //            </enum>
    //            <enum name="mapMyHouse">
    //                Mavlink file for MapMyHouse
    //            </enum>
    //         </arg>
    drone.on("PositionChanged", function (data) {

        console.log("position changed:" + data);

    });

});

//var obj = { filepath: "/internal_000/flightplans/flightPlan.mavlink", type: "flightPlan" };