var mainApp = angular.module("mainApp", ['ngRoute', 'ngCookies']);
mainApp.config(['$routeProvider', function ($routeProvider) {
    $routeProvider.
    when('/parks', {
        templateUrl: 'views/parks.html',
        controller: 'parksController'
    })
    .when('/statistics', {
        templateUrl: 'views/statistics.html',
        controller: 'statisticsController'
    })
    .when('/admin', {
        templateUrl: 'views/admin_drone.html',
        controller: 'adminController'
    })
    .when('/admin_parks', {
        templateUrl: 'views/admin_parks.html',
        controller: 'adminParksController'
    })
    .when('/admin_create_park', {
        templateUrl: 'views/admin_create_park.html',
        controller: 'createParkController'
    })
    .when('/admin_parks_detail/:idPark', {
          templateUrl: 'views/admin_parks_detail.html',
          controller: 'editParkController'
      })
    .when('/about', {
        templateUrl: 'views/about.html'
    })
    .when('/home', {
        templateUrl: 'views/home.html'
    })
    .when('/login', {
        templateUrl: 'views/login.html',
        controller: 'loginController'
    })
    .when('/park/:id', {
        templateUrl: 'views/specificpark.html',
        controller: 'viewPark'
    })
    .when('/park/:id/:idCap', {
        templateUrl: 'views/specificpark.html',
        controller: 'viewPark'
    })
    .otherwise({
        redirectTo: 'views/home.html'
    });
}]);
google.load('visualization', '1', {
    packages: ['corechart']
});
google.setOnLoadCallback(function () {
    angular.bootstrap(document.body, ['mainApp']);
});
mainApp.controller('adminParksController', function ($scope, $http) {
    var url = "/api/park";
    $http.get(url).success(function (response) {
        $scope.parks = response;
        
    });
});
mainApp.controller('parksController', function ($scope, $http) {
    var url = "/api/park";
    $http.get(url).success(function (response) {
        $scope.parks = response;
        angular.forEach($scope.parks, function (parks) {
            $http.get('/api/park/' + parks.idPark + '/last').then(function (responseCap) {
                parks.lastCapture = responseCap.data;
            })
        })
    });
});
mainApp.controller('statisticsController', function ($scope, $http, $filter) {
    var url = "/api/park";
    $http.get(url).success(function (response) {
        $scope.parks = response;
    });
    $scope.selparksChange = function (park) {
        $http.get("/api/capture/park/" + park.idPark).success(function (response) {
            $scope.captures = response;
            var data = new google.visualization.DataTable();
            data.addColumn('string', 'Time');
            data.addColumn('number', $filter('date')(new Date(),
                'yyyy-MM-dd'));
            angular.forEach($scope.captures, function (c) {
                data.addRow([$filter('date')(c.finishTime, 'H'),
                    parseInt(c.noccupiedLots)
                ]);
            });
            var options = {
                title: 'Nº de carros',
                curveType: 'function',
                legend: {
                    position: 'bottom'
                }
            };
            var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
            chart.draw(data, options);
        });
    };
});

mainApp.controller('createParkController', function ($scope, $http, $sce) {


    var url = "/api/park";

    

    $scope.formData = {};

    $scope.formData.active = 1;

    // process the form
    $scope.processForm = function () {
        $http({
            method: 'POST',
            url: url,
            data: $scope.formData, // pass in data as strings
            headers: { 'Content-Type': 'application/json' },
        })
                .error(function () {
                    $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Erro: Nao foi possivel contactar o serviço rest!</div>');
                })
                .success(function (data) {
                    console.log(data);
                    if (data == -1) {
                        // if not successful, bind errors to error variables
                        $scope.status = $sce.trustAsHtml('<div class="alert alert-danger" role="alert">Erro: Nao foi possivel criar o parque</div>');
                    } else {
                        // if successful, bind success message to message
                        $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Parque criado com sucesso!</div>');
                    }
                });
    }


});
mainApp.controller('editParkController', function ($scope, $http, $sce,$routeParams) {


    var url = "/api/park/" + $routeParams.idPark;
    $http.get(url).success(function (response) {
        $scope.formData = response;
    });

    $scope.formData = {};

    // process the form
    $scope.processForm = function () {
        $http({
            method: 'PUT',
            url: url,
            data: $scope.formData, // pass in data as strings
            headers: { 'Content-Type': 'application/json' },
        })
                .error(function () {
                    $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Erro: Nao foi possivel contactar o serviço rest!</div>');
                })
                .success(function (data) {
                    console.log(data);
                    if (data == -1) {
                        // if not successful, bind errors to error variables
                        $scope.status = $sce.trustAsHtml('<div class="alert alert-danger" role="alert">Erro: Nao foi possivel alterar o parque</div>');
                    } else {
                        // if successful, bind success message to message
                        $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Parque alterado com sucesso!</div>');
                    }
                });
    }


});

mainApp.controller('loginController', function ($cookies,$scope, $http, $window, $sce) {

	
	
    $scope.processForm = function () {
        $http({
            method: 'GET',
            url: 'api/user/login/' + $scope.formData.user + '/' + $scope.formData.password,
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
            } // set the headers so angular passing info as form data (not request payload)
        })
                .error(function () {
                     $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Erro a efetuar o login!</div>');
                })
                .success(function (data) {
					if (data == true) {
						console.log(data);
						$cookies.put('user-type', 'admin')
						$scope.status = data;
						$window.location.href = 'index_admin.html#admin';
					}
					else {
						$scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Erro a efetuar o login!</div>');
					}
                });
    };
});

mainApp.controller('adminController', function ($scope, $http, $location, $cookies, $sce, $interval) {
    if ($cookies.get("user-type") !== "admin") {
        $location.path('/');
    };
    var url = "/api/park";
    $http.get(url).success(function (response) {
        $scope.parks = response;
    });
    url = "/api/drone";
    $http.get(url).success(function (response) {
        $scope.drones = response;
    });
    
    
    
    $scope.callLogger = function() {
        url = "/api/logger/last/20";
        $http.get(url).success(function (response) {
            $scope.logger = response;
        });
    };

    $interval( function(){ $scope.callLogger(); }, 3000);
    
    $scope.formData = {};
    // process the form
    $scope.processForm = function () {
        
        $http({
            method: 'POST',
            url: "/api/drone/" + $scope.formData.drone.idDrone + "/park/" + $scope.formData.park.idPark + "/send"
        })
            .error(function () {
                $scope.status = $sce.trustAsHtml('<div class="alert alert-danger" role="alert">Erro: Nao foi possivel contatar o serviço rest</div>');
            })
            .success(function (data) {
                if (data == 1) {
                    $scope.status = $sce.trustAsHtml('<div class="alert alert-success" role="alert">Ordem enviada para o drone com sucesso!</div>');
                } else {
                    $scope.status = $sce.trustAsHtml('<div class="alert alert-danger" role="alert">Erro: Nao foi possivel contatar o drone</div>');
                }
            });
    }
});
mainApp.controller('viewPark', function ($scope, $http, $routeParams, $filter, $window, $location) {
    var urlcap;
    if ($routeParams.idCap) {
        $scope.idcap = $routeParams.idCap;
        urlcap = "/api/capture/" + $routeParams.idCap;
    } else {
        urlcap = "/api/park/" + $routeParams.id + "/last";
    }
    var url = "/api/park/" + $routeParams.id;
    $http.get(url).success(function (response) {
        $scope.park = response;
        $http.get(urlcap).then(function (responseCap) {
            $scope.park.lastCapture = responseCap.data;
            $scope.idcap = responseCap.data.idCapture;
        });
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Time');
        data.addColumn('number', $filter('date')(new Date(),
            'yyyy-MM-dd'));
        $http.get('/api/capture/park/' + response.idPark).then(function (cappark) {
            $scope.park.allcaptures = cappark.data;
            angular.forEach($scope.park.allcaptures, function (c) {
                //console.log(c.noccupiedLots);
                data.addRow([$filter('date')(c.finishTime, 'H'),
                    parseInt(c.noccupiedLots)
                ]);
            });
            var options = {
                title: 'Nº de carros',
                curveType: 'function',
                legend: {
                    position: 'bottom'
                }
            };
            var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
            chart.draw(data, options);
        });
    });
})