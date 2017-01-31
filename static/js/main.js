// listen for request sent over XHR and automatically show/hide spinner
angular.module('ngLoadingSpinner', ['angularSpinners'])
.directive('spinner', ['$http', 'spinnerService', function ($http, spinnerService) {
  return {
    link: function (scope, elm, attrs) {
      scope.isLoading = function () {
        return $http.pendingRequests.length > 0;
      };
      scope.$watch(scope.isLoading, function (loading) {
        if (loading) {
          spinnerService.show('spinner');
        } else {
          spinnerService.hide('spinner');
        }
      });


    }
  };

}]);
//
//
// var categoryKey = angular.module('categoryMap', []);
// // Define the `PhoneListController` controller on the `phonecatApp` module


var catalogApp = angular.module('catalogApp', ['ngLoadingSpinner', 'ngSanitize']);




catalogApp.filter("filterPanel", function () {
  return function (links, filterConfiguration) {
    if (filterConfiguration.enabled) {
      var results = [];
      links.forEach(function (link) {
        var keepLink = false;

        // if no tag checked, show everything
        if (filterConfiguration.includeTags.length == 0 &&
          filterConfiguration.excludeTags.length == 0) {
            keepLink = true;
          }


          // if no include, assume all included and let the exclude tag remove some
          if (filterConfiguration.includeTags.length == 0) {
            keepLink = true;
          } else {
            keepLink = true;
            filterConfiguration.includeTags.forEach(function (tag) {
              if (!link.tags || link.tags.indexOf(tag) < 0) {
                keepLink = false;
              }
            });
          }

          filterConfiguration.excludeTags.forEach(function (tag) {
            if (!link.tags || link.tags.indexOf(tag) >= 0) {
              keepLink = false;
            }

            /*
            filterConfiguration.excludeTags.forEach(function (tag) {
            if (link.tags.State == "live") {
            keepLink = false;
          }
          */


        });

        if (keepLink) {
          results.push(link);
        }
      });
      return results;
    } else {
      return links;
    }
  }
});

catalogApp.filter("emoji", function () {
  return function (input) {
    return input ? emojione.toImage(input) : input;
  }
});

catalogApp.controller('MainController', function ($scope, $http) {
  console.info("Initializing MainController");
  $scope.links = [];

  $scope.categoryMap = ["Beginner/General","Ingest and Store Data",
"Tips","Monitoring","Performance and Consistent Regions","Correlate and Merge Streams",
"Analyze and Classify Data","Best Practices",
"Collections and Data Types","Operators and Functions","Microservices","Troubleshooting"];
 $scope.getCat = function(item){
   var result = [];
    for(var i = 0 ; i < $scope.categoryMap.length ; i++){
        if(parseInt(item) === (i+1)){
          result.push( $scope.categoryMap[i]);
        }
    }
    return result;
 };
 // alert($scope.getCat);

  $scope.filterConfiguration = {
    enabled: true,
    includeTags: [],
    excludeTags: []
  };



  /*
  $http.get('urltoRESTAPICall').
  then(function(response) {
  $scope.greeting = response.data;
  console.log(response.data);
});
*/



$scope.toggleTagConfiguration = function (tag, status) {
  var tagSet = status ? $scope.filterConfiguration.includeTags : $scope.filterConfiguration.excludeTags;
  var position = tagSet.indexOf(tag);
  if (position >= 0) {
    tagSet.splice(position, 1);
  } else {
    tagSet.push(tag);
  }
}

$scope.tagFilter = function (tag) {
  if (categories.indexOf(tag) >= 0) {
    return tag;
  } else {
    return null;
  }
};

[
  {
    url: "./data/main.json"
  }
].forEach(function (source) {
  var i = 0;
  $http.get(source.url).success(function (links) {
    links.forEach(function (link) {
      if (!link.tags) {
        link.tags = [];
      }
      link.tags.push(link.name);
      link.tags.push(link.description);
      if (link.language) {
        link.tags = link.tags.concat(link.language);
      }
      if (link.category) {
        link.tags = link.tags.concat(link.category);
      }

      if (link.services){

        link.tags = link.tags.concat(link.services);
      }

      if (link.toolkits) {
        link.tags = link.tags.concat(link.toolkits);
      }

      if (link.operators) {
        link.tags = link.tags.concat(link.operators);
      }
    })


    $(".hrefLink").each(function( index ) {
      if (!$(this).attr('href')){
        $(this).hide();
      }
    });

    // view the links randomly everytime, to get better track of top clicks of the links
    $scope.links = $scope.links.concat(links);
    $scope.random = function() {
      return 0.5 - Math.random();
    }



    $scope.links.sort( function(link1, link2){
      if (link1.featured && link2.featured){
        return 0;
      } else if (link1.featured && !link2.featured){
        return -1;
      } else if (link2.featured &&  !link1.featured){
        return 1;
      } //link1.name.localeCompare(link2.name);
});
  });
});
});
