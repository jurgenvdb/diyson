console.log("graphs_script.js loaded");

const updateInterval = 20;
setInterval(drawGraphs, updateInterval);



class Graph {
  constructor(divId, title, xTitle, yTitle, minX, maxX, minY, maxY, startPoint, endPoint, controlPoints, steps) {
    this.startPoint = startPoint;
    this.endPoint = endPoint;
    this.controlPoints = controlPoints;
    this.steps = steps;
    this.graphValues = [];
    this.generateData();
    this.chart = new CanvasJS.Chart(divId, {
      theme: "light1",
      title: {
        text: title
      },
      axisX: {
        title: xTitle,
        minimum: minX,
        maximum: maxX,
      },
      axisY: {
        title: yTitle,
        minimum: minY,
        maximum: maxY,
      },
      data: [
        {
          type: "line",
          dataPoints: [{ x: this.controlPoints[1].x, y: this.controlPoints[1].y }, { x: this.endPoint.x, y: this.endPoint.y }],
          color: "black",
          lineThickness: 2,
          markerSize: 0,
        },
        {
          type: "line",
          dataPoints: [{ x: this.startPoint.x, y: this.startPoint.y }, { x: this.controlPoints[0].x, y: this.controlPoints[0].y }],
          color: "black",
          lineThickness: 2,
          markerSize: 0,
        },
        {
          type: "scatter",
          dataPoints: [{ x: this.controlPoints[0].x, y: this.controlPoints[0].y }, { x: this.controlPoints[1].x, y: this.controlPoints[1].y }],
          color: "blue",
          markerSize: 10,
        },
        {
          type: "line",
          dataPoints: this.graphValues,
          color: "red",
          lineThickness: 4,
          markerSize: 1,
        }]
    });
  }

  generateData = function () {
    for (var t = 0; t <= steps; t += 1) {
      this.graphValues.push({ x: t / 100, y: bezier(t / steps, this.controlPoints).y });
      if (this.graphValues.length > 100) {
        this.graphValues.shift();
      }
    }
  }

  render = function () {
    this.chart.render();
  }
}


function bezier(t, controlPoints = [{ x: 0, y: 1 }, { x: 0, y: 1 }]) {
  var res = {
    x: (1 - t) * startPoint.x + 3 * t * Math.pow(1 - t, 2) * controlPoints[0].x + 3 * Math.pow(t, 2) * (1 - t) * controlPoints[1].x + Math.pow(t, 3) * endPoint.x,
    y: (1 - t) * startPoint.y + 3 * t * Math.pow(1 - t, 2) * controlPoints[0].y + 3 * Math.pow(t, 2) * (1 - t) * controlPoints[1].y + Math.pow(t, 3) * endPoint.y
  }
  return res;
}

const steps = 100;
const startPoint = { x: 0, y: 0 };
const endPoint = { x: 1, y: 1 };
const minimum = -.5;
const maximum = 1.5;

brightnessGraph = new Graph("brightness_graph", "Brightness curve", "time", "Brightness", minimum, maximum, minimum, maximum, startPoint, endPoint, [{ x: 0, y: 1 }, { x: 0, y: 1 }], steps);
temperatureGraph = new Graph("temperature_graph", "Temperature curve", "time", "Temperature", minimum, maximum, minimum, maximum, startPoint, endPoint, [{ x: 0, y: 1 }, { x: 0, y: 1 }], steps);

// function generateData() {
//   // graphValues = [];
//   for (var t = 0; t <= steps; t += 1) {
//     var bez = bezier(t / steps);
//     graphValues.push({ x: t / 100, y: bez.y });
//     if (graphValues.length > 100) {
//       graphValues.shift();
//     }
//   }


//   console.log(graphValues);
// }

// var brightness_chart = new CanvasJS.Chart("brightness_graph", {
//   theme: "light1",
//   title: {
//     text: "Brightness curve"
//   },
//   axisX: {
//     title: "time",
//     minimum: minimum,
//     maximum: maximum,
//   },
//   axisY: {
//     title: "Brightness",
//     minimum: minimum,
//     maximum: maximum,
//   },
//   data: [
//     {
//       type: "line",
//       dataPoints: [{ x: controlPoints[1].x, y: controlPoints[1].y }, { x: endPoint.x, y: endPoint.y }],
//       color: "black",
//       lineThickness: 2,
//       markerSize: 0,
//     },
//     {
//       type: "line",
//       dataPoints: [{ x: startPoint.x, y: startPoint.y }, { x: controlPoints[0].x, y: controlPoints[0].y }],
//       color: "black",
//       lineThickness: 2,
//       markerSize: 0,
//     },
//     {
//       type: "scatter",
//       dataPoints: [{ x: controlPoints[0].x, y: controlPoints[0].y }, { x: controlPoints[1].x, y: controlPoints[1].y }],
//       color: "blue",
//       markerSize: 10,
//     },
//     {
//       type: "line",
//       dataPoints: graphValues,
//       color: "red",
//       lineThickness: 4,
//       markerSize: 1,
//     }]
// });


function drawGraphs() {
  brightnessGraph.render();
  temperatureGraph.render();
}




// https://canvasjs.com/docs/charts/how-to/creating-dynamic-charts/