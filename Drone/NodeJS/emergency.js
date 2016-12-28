//Nunca usar, so em casos extremos e se o drone se encontrar a cm do chao 

var bebop = require('node-bebop');

var drone = bebop.createClient();

drone.connect(
    function() {
        drone.emergency();
});
