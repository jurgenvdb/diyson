console.log("graphs_script.js loaded");

function vector(x, y) {
  this.x = x;
  this.y = y;
}

var startPoint = new vector(0, 0);
var endPoint = new vector(1, 1);
var steps = 100;
var step = 1 / steps;
var lineWidth = 1;
var lineColor = "black";
var absolutePointColor = "black";
var pointColor = "red";
var pointRadius = 5;
var canvas = document.getElementById("brightness_graph");
var ctx = canvas.getContext("2d");
var width = canvas.width;
var height = canvas.height;

var controlPoints = [
    new vector(.5, .2), 
    new vector(.8, .5), 
];

var graphValues = [["Time", "Brightness"]];
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
    graphValues.push([bez.x, bez.y]);
  }
  console.log(graphValues);
}

function drawGraph() {
  ctx.clearRect(0, 0, width, height);
  ctx.beginPath();
  ctx.moveTo(startPoint.x * width, startPoint.y * height);
  for (var i = 1; i < graphValues.length; i++) {
    ctx.lineTo(graphValues[i][0] * width, graphValues[i][1] * height);
  }
  ctx.lineWidth = lineWidth;
  ctx.strokeStyle = lineColor;
  ctx.stroke();
  ctx.closePath();
  for (var i = 0; i < controlPoints.length; i++) {
    ctx.beginPath();
    ctx.arc(controlPoints[i].x * width, controlPoints[i].y * height, pointRadius, 0, 2 * Math.PI);
    ctx.fillStyle = pointColor;
    ctx.fill();
    ctx.closePath();
  }
  ctx.beginPath();
  ctx.arc(startPoint.x * width, startPoint.y * height, pointRadius, 0, 2 * Math.PI);
  ctx.fillStyle = absolutePointColor;
  ctx.fill();
  ctx.closePath();
  ctx.beginPath();
  ctx.arc(endPoint.x * width, endPoint.y * height, pointRadius, 0, 2 * Math.PI);
  ctx.fillStyle = absolutePointColor;
  ctx.fill();
  ctx.closePath();
}

drawGraph();