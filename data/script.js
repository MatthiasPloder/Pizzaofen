var tempereture;
var oben;
var TEMP;

function adjustHeating(temperature) {
  console.log("Heizelement auf " + temperature + " Grad einstellen.");
  document.getElementById("current-temperature").innerHTML = temperature + " Grad Celsius";
  tempereture = temperature;
}

function setPizzaTemperature(temperature) {
  adjustHeating(temperature);
  document.getElementById("heating-range").value = temperature;
}
// GRAD || unten always on  OBEN = 0/1 
function startBacking() {
  var http = new XMLHttpRequest();
  http.open("GET", "/update?output=" + tempereture + "/" + oben, true);
  http.send();
}
window.onload = function () {
  fetch('/temperature')
   .then(response => response.json())
   .then(data => {
      const tempList = document.getElementById('temperature-list');
      data.temperatures.forEach((temperature, index) => {
        const listItem = document.createElement('li');
        listItem.textContent = `Sensor ${index + 1}: ${temperature}°C`;
        tempList.appendChild(listItem);
      });
    })
   .catch(error => console.error('Error fetching temperature data:', error));
};

function toggleHeating(heatingType) {
  let upperHeatingStatus = document.getElementById("upper-heating-status");
  let lowerHeatingStatus = document.getElementById("lower-heating-status");
  let upperHeatingOverlay = document.getElementById("pizzaofen-overlay-upper");
  let lowerHeatingOverlay = document.getElementById("pizzaofen-overlay-lower");
  let bothHeatingOverlay = document.getElementById("pizzaofen-overlay-both");

  if (heatingType === 'upper') {
    if (upperHeatingStatus.innerHTML === "Aus") {
      oben = 1;
      upperHeatingStatus.innerHTML = "An";
      upperHeatingOverlay.style.display = "block"; // Overlay anzeigen
      console.log("Obere Heizung eingeschaltet.");
    } else {
      oben = 0;
      upperHeatingStatus.innerHTML = "Aus";
      upperHeatingOverlay.style.display = "none"; // Overlay ausblenden
      console.log("Obere Heizung ausgeschaltet.");
    }
  } else if (heatingType === 'lower') {
    if (lowerHeatingStatus.innerHTML === "Aus") {
      lowerHeatingStatus.innerHTML = "An";
      lowerHeatingOverlay.style.display = "block"; // Overlay anzeigen
      console.log("Untere Heizung eingeschaltet.");
    } else {
      lowerHeatingStatus.innerHTML = "Aus";
      lowerHeatingOverlay.style.display = "none"; // Overlay ausblenden
      console.log("Untere Heizung ausgeschaltet.");
    }
  }

  // Überprüfen, ob beide Heizelemente eingeschaltet sind und das entsprechende Overlay anzeigen
  if (upperHeatingStatus.innerHTML === "An" && lowerHeatingStatus.innerHTML === "An") {
    bothHeatingOverlay.style.display = "block";
  } else {
    bothHeatingOverlay.style.display = "none";
  }
}

function turnOffAllHeating() {
  tempereture = 0;
  oben = 2;     // 2 = alles aus
  let heatingTypes = ["upper", "lower"];
  heatingTypes.forEach(function (heatingType) {
    document.getElementById(heatingType + "-heating-status").innerHTML = "Aus";
    document.getElementById("pizzaofen-overlay-" + heatingType).style.display = "none";
  });
  document.getElementById("current-temperature").innerHTML = "0 Grad Celsius";
  document.getElementById("heating-range").value = 0;
  document.getElementById("pizzaofen-overlay-both").style.display = "none"; // Overlay für beide Heizungen ausblenden
  console.log("Alle Heizungen ausgeschaltet.");
}

function toggleGrill() {
  turnOffAllHeating();

  let lowerHeatingStatus = document.getElementById("lower-heating-status");
  let lowerHeatingOverlay = document.getElementById("pizzaofen-overlay-lower");

  lowerHeatingStatus.innerHTML = "An";
  lowerHeatingOverlay.style.display = "block"; // Overlay anzeigen
  console.log("Untere Heizung (Grill) eingeschaltet.");
  setPizzaTemperature(170); // Temperatur auf 170 Grad setzen

}

window.onload = function () {
  turnOffAllHeating();
};