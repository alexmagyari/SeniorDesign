import { Component } from '@angular/core';
import { ViewChild, ElementRef } from '@angular/core';
import { AlertController } from '@ionic/angular';
import { Geolocation} from '@capacitor/core';

declare var google: any;

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  //Default Parameters
  readonly DefaultHoverTime = 90; // In seconds
  readonly DroneTravelSpeed = 20; // In MPH
  readonly DroneBatteryLife = 30; // In Minutes
  
  // Default Battery Parameters are based on the Tattu R-Line Version 3.0 1550mAh 4s 120C Lipo Battery
  // Calculations via https://www.omnicalculator.com/other/drone-flight-time#drone-flight-time-formula
  readonly LiftPowerPerKg = 150; // Power required to lift one kg, semi efficient system at 150 W/kg
  batteryVoltage = 59.2; // In Voltage
  batteryCapacity = 1.55; // In Amp Hours
  maxBatteryDischarge = .9; // % of max discharge. Typical value is 80-90% to preserve battery life before it needs to be replaced
  allUpWeight = 2; // Total weight of drone and sensor, in kg

  map: any;

  @ViewChild('map', {read: ElementRef, static: false}) mapRef: ElementRef;

  // Markers 0 and 1 may never be deleted. Marker 0 represents the fire center, and marker 1 represents the start point.
  // Start point is determined by user location.
  markers: any = [];
  gmarkers: any = [];

  myLat: number = 29.7604;
  myLng: number = 95.3698;

  // Info variables
  totalHoverTime: number = 0;
  totalTravelTime: number = 0;
  totalFlightTime: number = 0;
  totalBatteryUsage: number = 0;
  totalMaxFlightTime: number = 0;
  droneName: string = "Not Connected";

  constructor(public alertController: AlertController) {}

  ionViewDidEnter(){
    this.showMap();
  }

  async getLocation(centerMap = false){
    const position = await Geolocation.getCurrentPosition();
    this.myLat = position.coords.latitude;
    this.myLng = position.coords.longitude;
    if(centerMap){
      var latLng = new google.maps.LatLng(this.myLat, this.myLng);
      this.map.setCenter(latLng);
      this.map.setZoom(17);
    }
  }

  async resetPoints(){
    for (let i = 0; i < this.gmarkers.length; i++)
      this.gmarkers[i].setMap(null);
    this.gmarkers.length = 0;
    this.markers.length = 0;
    await this.getLocation(true);
    // Add smoke marker
    this.addMarker(this.myLat + 0.0005, this.myLng, true, true, false);
    this.addMarker(this.myLat, this.myLng, false, false, false);
  }

  updateInfoVariables(){
    let ht = 0;
    let td = 0; // total distance in meters
    // ignore 0th index, as it is the fire location
    for (let i = 1; i < this.markers.length; i++){
      ht += this.markers[i].hoverTime;

      if (i != this.markers.length - 1){
        td += google.maps.geometry.spherical.computeDistanceBetween(this.gmarkers[i].getPosition(), this.gmarkers[i + 1].getPosition());
      }
      else
      {
        td += google.maps.geometry.spherical.computeDistanceBetween(this.gmarkers[i].getPosition(), this.gmarkers[1].getPosition());
      }
    }
    let averageAmpDraw = this.allUpWeight * this.LiftPowerPerKg / this.batteryVoltage;
    let t = this.batteryCapacity * this.maxBatteryDischarge / averageAmpDraw;
    td /= 1609.32 // Convert to miles
    td = (td / this.DroneTravelSpeed) * 60
    this.totalTravelTime = (Math.round(td * 100) / 100);
    this.totalHoverTime = (Math.round(ht / 60 * 100) / 100);
    this.totalFlightTime = (Math.round((this.totalTravelTime + this.totalHoverTime) * 100) / 100);
    
    this.totalMaxFlightTime = (Math.round(t * 60 * 100) / 100);

    this.totalBatteryUsage = (Math.round((this.totalFlightTime / this.totalMaxFlightTime) * 100 * 100) / 100);
  }

  addMarkersToMap(markers){
    for (let marker of markers){
      this.addMarker(marker.latitude, marker.longitude);
    }
  }

  addListeners(marker){
    marker.addListener('click', () =>{
      this.viewMarker(marker);
    });

    marker.addListener('dragend', () =>{
      const index = this.markers.findIndex(i => (i.latitude === marker.latitude && i.longitude === marker.longitude))
      if (index > -1) {
        this.markers[index].latitude = marker.position.lat();
        this.markers[index].longitude = marker.position.lng();
        marker.latitude = marker.position.lat();
        marker.longitude = marker.position.lng();
        this.updateInfoVariables();
      }
      else
      {
        console.log("Marker does not exist!");
      }
    });
  }

  async viewMarker(marker) {
    const index = this.markers.findIndex(i => (i.latitude === marker.latitude && i.longitude === marker.longitude))
    let m = this.markers[index];
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Drone Waypoint",
      message: '<p>Latitude: ' + m.latitude + '</p>' + 
               '<p>Longitude: ' + m.longitude + '</p>' + 
               '<p>Hover Time: ' + m.hoverTime + ' [s]</p>',

      buttons: [
        {
          text: 'Edit',
          handler: () =>{
            alert.dismiss();
            this.editHoverTime(m);
          }
        },
        {
          text: 'Delete',
          handler: (d) => {
            alert.dismiss();
            this.deletePoint(m);
          }
        },
        {
          text: 'Close', 
          role: 'cancel',
          cssClass: 'secondary',
          handler: (d) => {
            alert.dismiss();
          }
        },
        ]
    });

    await alert.present();
  }

  async deletePoint(marker){
    const index = this.markers.findIndex(i => (i.latitude === marker.latitude && i.longitude === marker.longitude))
    if (index == 1 || index == 0){
      this.presentBasicAlert("Cannot delete starting waypoint");
      return;
    }
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Delete Waypoint",
      subHeader: "Are you sure you wish to delete this waypoint?",
      buttons: [
        {
          text: 'Cancel', 
          role: 'cancel',
          cssClass: 'secondary',
          handler: (d) => {
            alert.dismiss();
          }
        },
        {
          text: 'Delete',
          cssClass: 'danger',
          handler: (d) =>{
            const index = this.markers.findIndex(i => (i.latitude === marker.latitude && i.longitude === marker.longitude))
            if (index > -1) {
              this.gmarkers[index].setMap(null);
              this.gmarkers.splice(index, 1);
              this.markers.splice(index, 1);
              this.refreshMarkerLabels(this.gmarkers);
              this.updateInfoVariables();
            }
            alert.dismiss();
          }
        }]
    });

    await alert.present();
  }

  async editHoverTime(marker){
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Edit Hover Time",
      subHeader: "Enter new hover time in seconds.",
      inputs: [
        {
          name: 'hoverTime',
          type: 'text',
          placeholder: 'New Hover Time [s]',
          value: marker.hoverTime
        }
      ],
      buttons: [
        {
          text: 'Cancel', 
          role: 'cancel',
          cssClass: 'secondary',
          handler: (d) => {
            alert.dismiss();
          }
        },
        {
          text: 'Save',
          handler: (alertData) =>{
            const index = this.markers.findIndex(i => (i.latitude === marker.latitude && i.longitude === marker.longitude))
            this.markers[index].hoverTime = parseFloat(alertData.hoverTime);
            this.updateInfoVariables();
            alert.dismiss();
          }
        }]
    });

    await alert.present();
  }

  showMap(){ 
    fetch('./assets/mapStyle.json').then(res => res.json())
    .then(json => {
      const location = new google.maps.LatLng(-17.824858, 31.053028);
      const options = {
        center: location,
        zoom: 15,
        disableDefaultUI: true,
        styles: json
      }
      this.map = new google.maps.Map(this.mapRef.nativeElement, options);
      this.addMarkersToMap(this.markers);
      this.addMapListeners(this.map);
      this.resetPoints();
    });
  }

  refreshMarkerLabels(markers){
    let markerCounter : number = 0;

    for (let marker of markers){
      if (markerCounter != 0)
        marker.set('label', {text: markerCounter, color: "white"});
      markerCounter++;
    }
  }

  addMarker(lat : number, lng : number, movable : boolean = true, smokeMarker : boolean = false, requestHoverTime = true){
    const index = this.markers.findIndex(i => (i.latitude === lat && i.longitude === lng))
    if (index != -1) {
      console.log("Attempting to add marker that already exists!")
      return;
    }
    let marker = {
      latitude: lat,
      longitude: lng,
      hoverTime: this.DefaultHoverTime
    }
    let position = new google.maps.LatLng(lat, lng);
    let mapMarker = new google.maps.Marker({
      position: position,
      label: {text: (this.gmarkers.length).toString(), color: "white"},
      latitude: lat,
      longitude: lng,
      hoverTime: this.DefaultHoverTime,
      draggable: movable
    });
    if (smokeMarker)
      mapMarker.setIcon("assets/icon/fire.png");
    this.markers.push(marker);
    this.gmarkers.push(mapMarker);
    mapMarker.setMap(this.map);
    if (!smokeMarker)
      this.addListeners(mapMarker);
    if (requestHoverTime)
      this.editHoverTime(marker);
    this.refreshMarkerLabels(this.gmarkers);
    this.updateInfoVariables();
  }

  addMapListeners(map){
    // Implementing long press
    var mousedUp = false;
    map.addListener('mousedown', (event) => 
      { 
        mousedUp = false;
        setTimeout(() => 
          {
            if(mousedUp === false){
              this.addMarker(event.latLng.lat(), event.latLng.lng());   
            }
          }, 500);
      });
    map.addListener('mouseup', function(event){ 
        mousedUp = true;
    });
    map.addListener('dragstart', function(event){ 
      mousedUp = true;
    });
  }

  uploadWaypoints(){
    if (this.markers.length == 0){
      this.presentBasicAlert("There are no markers to upload!</br>Upload not completed.");
      return;
    }
    if (this.totalBatteryUsage > 100){
      this.presentBasicAlert("Battery usage exceeds 100%! Waypoints not uploaded to prevent drone crash.");
      return;
    }
    for(var marker in this.markers){
      console.log("Uploaded new marker to drone");
    }
  }

  async modifyParameters(){
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Drone Parameters",
      message: "Battery Voltage [V * cellCount]</br>Battery Capacity [A Hours]</br>Max Discharge [%]</br>Drone Weight [kg]",
      inputs: [
        {
          name: 'voltage',
          type: 'text',
          label: 'Battery Voltage',
          placeholder: 'Battery Voltage [V * #cells]',
          value: this.batteryVoltage
        },
        {
          name: 'capacity',
          type: 'text',
          placeholder: 'Battery Capacity [Amp Hours]',
          value: this.batteryCapacity
        },
        {
          name: 'discharge',
          type: 'text',
          placeholder: 'Maximum allowed discharge [%]',
          value: this.maxBatteryDischarge
        },
        {
          name: 'weight',
          type: 'text',
          placeholder: 'Total Drone Weight [kg]',
          value: this.allUpWeight
        }
      ],
      buttons: [
        {
          text: 'Cancel', 
          role: 'cancel',
          cssClass: 'secondary',
          handler: (d) => {
            alert.dismiss();
          }
        },
        {
          text: 'Save',
          handler: (alertData) =>{
            if (alertData.dischage > 1 || alertData.discharge < 0 || isNaN(alertData.discharge))
              this.presentBasicAlert("Battery discharge amount must be a value between 0 and 1. Values not saved.");
            else if (alertData.voltage < 0 || isNaN(alertData.voltage))
              this.presentBasicAlert("Battery voltage must be a value greater than 0. Values not saved.");
            else if (alertData.capacity < 0 || isNaN(alertData.capacity))
              this.presentBasicAlert("Battery capacity must be a value greater than 0. Values not saved.");
            else if (alertData.weight < 0 || isNaN(alertData.weight)) 
              this.presentBasicAlert("Drone weight must be a value greater than 0. Values not saved.");
            else
            {
              this.batteryVoltage = parseFloat(alertData.voltage);
              this.batteryCapacity = parseFloat(alertData.capacity);
              this.maxBatteryDischarge = parseFloat(alertData.discharge);
              this.allUpWeight = parseFloat(alertData.weight);
              this.updateInfoVariables();
              alert.dismiss();
            }
          }
        }]
    });

    await alert.present();
  }

  async confirmReset(){
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Confirm",
      message: "Are you certain you want to refresh location and waypoints?",
      buttons: [
        {
          text: 'Cancel', 
          role: 'cancel',
          handler: (d) => {
            alert.dismiss();
          }
        },
        {
          text: 'Confirm', 
          handler: (d) => {
            this.resetPoints();
            alert.dismiss();
          }
        }]
    });
    await alert.present();
  }

  droneIsConnected(): boolean{
    return false;
  }

  async presentBasicAlert(t: string){
    const alert = await this.alertController.create({
      cssClass: 'alertClass',
      header: "Notice",
      message: t,
      buttons: [
        {
          text: 'Dismiss', 
          role: 'cancel',
          handler: (d) => {
            alert.dismiss();
          }
        }]
    });
    await alert.present();
  }

}
