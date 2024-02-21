console.log("graphs_script.js loaded");

function vector(x, y) {
  this.x = x;
  this.y = y;
}

var startPoint = new vector(0, 0);
var endPoint = new vector(1, 1);
var steps = 100;
var step = 1 / steps;

var controlPoints = [
    new vector(.5, .2), 
    new vector(.8, .5), 
];

var graphValues = [];
generateData();

function bezier(t) {
    // console.log(controlPoints);
    // console.log((1-t)*startPoint.x  + 3 * t * Math.pow(1-t, 2) * controlPoints[0].x + 3 * Math.pow(t, 2) * (1-t) * controlPoints[1].x + Math.pow(t, 3) * endPoint.x);
    var res = new vector(
        (1-t)*startPoint.x  + 3 * t * Math.pow(1-t, 2) * controlPoints[0].x + 3 * Math.pow(t, 2) * (1-t) * controlPoints[1].x + Math.pow(t, 3) * endPoint.x,
        (1-t)*startPoint.y  + 3 * t * Math.pow(1-t, 2) * controlPoints[0].y + 3 * Math.pow(t, 2) * (1-t) * controlPoints[1].y + Math.pow(t, 3) * endPoint.y
    );
    // console.log(res);
    return res;
}

function generateData() {
  for (var t = 0; t <= 1; t += step) {
    var bez = bezier(t);
    graphValues.push({x: bez.x, y: bez.y});
  }
  console.log(graphValues);
}

var chart = new CanvasJS.Chart("brightness_graph",{
  title :{
    text: "Live Data"
  },
  axisX: {						
    title: "Axis X Title"
  },
  axisY: {						
    title: "Units"
  },
  data: [{
    type: "line",
    dataPoints : graphValues
  }]
});


// https://canvasjs.com/docs/charts/how-to/creating-dynamic-charts/