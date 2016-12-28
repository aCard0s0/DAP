//Usar para receber as coordenadas dependendo da posicao do drone
//Ira ser usado para estabelecer os pontos de interece para planear rotas de voo aos parques

var bebop = require('node-bebop');

var drone = bebop.createClient();

drone.connect(function() {
  drone.GPSSettings.resetHome();
  drone.WifiSettings.outdoorSetting(1);

  drone.on("PositionChanged", function(data) {
    console.log(data);
  });
});
