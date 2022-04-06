// FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS 

  function dateHeure(){   // fonction pour renvoyer la date et l'heure actuelle
    var today = new Date();
    var date = today.getDate()+'-'+(today.getMonth()+1)+'-'+today.getFullYear();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var dateTime = time+' '+date;
    return dateTime;
  }
  
  
  function toggleCheckbox1(element) {   // button pour activer/desactiver la liaison LORA
  if(element.checked){ localStorage.setItem("loraStatus", 1);}
    else {  localStorage.setItem("loraStatus", 0); }
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
      var obj = this.responseText;
      document.getElementById("lora_").innerHTML = (obj).fontcolor("green");
      }}
    if(element.checked){ xhr.open("GET", "/LoraON", true); }
    else { xhr.open("GET", "/LoraOFF", true); }
    xhr.send();
  }
  
  function toggleCheckbox2(element) {  // button pour activer/desactiver la camera
    if(element.checked){ localStorage.setItem("camStatus", 1);}
    else {  localStorage.setItem("camStatus", 0); }
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
      var obj = this.responseText;
      document.getElementById("camera_").innerHTML = (obj).fontcolor("green");
      }}
    if(element.checked){ xhr.open("GET", "/CamON", true); }
    else { xhr.open("GET", "/CamOFF", true);  }
    console.log(localStorage.getItem("camStatus"));
    xhr.send();
  }
  
/*
  function toggleCheckbox3(element) {  // button pour activer/desactiver le capteur 9-axes (BNO)  --- fonctionne pas actuellement
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/Cap9ON", true); }
    else { xhr.open("GET", "/Cap9OFF", true) }
    xhr.send();
  }
*/
    window.onload = function() { // same as window.addEventListener('load', (event) => {
 
    document.getElementById("lora_").innerHTML = (".").fontcolor("white");
    document.getElementById("camera_").innerHTML = (".").fontcolor("white");


  };
