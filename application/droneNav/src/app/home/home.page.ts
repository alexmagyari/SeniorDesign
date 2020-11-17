import { Component, NgZone} from '@angular/core';
import { ViewChild, ElementRef } from '@angular/core';
import { AlertController } from '@ionic/angular';
import { Geolocation} from '@capacitor/core';
import { Platform } from '@ionic/angular';
import { BLE } from '@ionic-native/ble/ngx';
import { ModalController } from '@ionic/angular';
import { ViewDevicesPage } from '../view-devices/view-devices.page';

declare var google: any;

const GPS_SERVICE = 'ff10';
const GPS_CHARACTERISTIC = 'ff00';


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
  drone: any = {};

  // Ble Vars
  devices: any[] = [];
  statusMessage: string;

  constructor(public alertController: AlertController, public platform: Platform, 
    private ble: BLE, 
    private ngZone: NgZone, public modalController: ModalController) {}

  ionViewDidEnter(){
    this.showMap();
  }

  async getBLEDevices(){
    if (this.platform.is("ios") || this.platform.is("android")){
      this.setStatus('Scanning for Bluetooth LE Devices');
      this.devices = [];  // clear list

      // For testing on PC
      // this.devices = 
      // [
      //   {
      //     'name': "Drone",
      //     'id': "IDIDIDIDID-10101010",
      //     'advertising': [2,1,6,3,3,15,24,8,9,66,97,116,116,101,114,121],
      //     'rssi': -37
      //   },
      //   {
      //     'name': "SomeSpeaker",
      //     'id': "NOTTHEDRONE-2020202020",
      //     'advertising': [2,1,6,3,3,15,24,8,9,66,97,116,116,101,114,122],
      //     'rssi': -60
      //   },
      //   {
      //     'name': "AHeadPhone",
      //     'id': "NOTTHEDRONE-EITHER2020202",
      //     'advertising': [2,1,6,3,3,15,24,8,9,66,97,116,116,101,114,123],
      //     'rssi': -80
      //   }
      // ];
      this.ble.scan([], 5).subscribe(
        device => this.onDeviceDiscovered(device), 
        error => this.scanError(error)
      );

      const alert = await this.alertController.create({
        cssClass: 'alertClass',
        header: "",
        message: '<h1><ion-spinner name="crescent"></ion-spinner><br />Searching for Devices...</h1>',
      });
      await alert.present();

      const modal = await this.modalController.create({
        component: ViewDevicesPage,
        componentProps: { 
          parentPage: this,
          deviceList: this.devices
         }
      });

      setTimeout(this.setStatus.bind(this), 5000, 'Scan complete');
      setTimeout(function(){

        alert.dismiss();
        modal.present();
      }, 5000);

      
      
    }
    else{
      this.presentBasicAlert("BLE only works on Android or iOS.");
    }
  }

  connectToDevice(device){

    this.setStatus('Connecting to ' + device.name || device.id);

    this.ble.connect(device.id).subscribe(
      drone => this.onConnected(drone),
      drone => this.onDeviceDisconnected(drone)
    );
    
  }

  onConnected(peripheral) {
    this.ngZone.run(() => {
      this.setStatus('');
      this.drone = peripheral;
    });
    this.writeGPSData();
  }

  writeGPSData() {
    console.log('Writing GPS Data');
    let buffer = this.convertMarkersToByteArray().buffer;
    this.ble.write(this.drone.id, GPS_SERVICE, GPS_CHARACTERISTIC, buffer).then(
      () => this.presentBasicAlert("Markers uploaded to drone. Please step away and prepare for launch."),
      e => this.presentBasicAlert('Unexpected Error. Please reset drone and try again.')
    );
  }

  convertMarkersToByteArray(){
    let arr = new Uint8Array(this.markers.length * 10);
    let c = 0;
    this.markers.forEach(e => {
      let b = this.toFloat32(e.latitude);
      arr[c++] = (b & 0xFF000000) >>> 24;
      arr[c++] = (b & 0x00FF0000) >>> 16;
      arr[c++] = (b & 0x0000FF00) >> 8;
      arr[c++] = (b & 0x000000FF);
      console.log("Converted " + e.latitude + " to " + b.toString(16));
      b = this.toFloat32(e.longitude);
      arr[c++] = (b & 0xFF000000) >>> 24;
      arr[c++] = (b & 0x00FF0000) >>> 16;
      arr[c++] = (b & 0x0000FF00) >> 8;
      arr[c++] = (b & 0x000000FF);
      console.log("Converted " + e.longitude + " to " + b.toString(16));
      b = e.hoverTime;
      arr[c++] = (b & 0xFF00) >> 8;
      arr[c++] = (b & 0x00FF);
      console.log("Converted " + e.hoverTime + " to " + b.toString(16));
    });
    arr.forEach(e => {
      console.log(e.toString(16));
    })
    return arr;
  }

  onDeviceDisconnected(peripheral) {
    this.presentBasicAlert('The drone has been disconnected.');
  }
  
  onDeviceDiscovered(device) {
    console.log('Discovered ' + JSON.stringify(device, null, 2));
    this.ngZone.run(() => {
      this.devices.push(device);
    });
  }

  setStatus(message) {
    console.log(message);
    this.ngZone.run(() => {
      this.statusMessage = message;
    });
  }

  scanError(error) {
    this.setStatus('Error ' + error);
    this.presentBasicAlert('Error scanning for Bluetooth low energy devices');
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
    this.getBLEDevices();
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

  toFloat32(value) {
    var bytes = 0;
    switch (value) {
        case Number.POSITIVE_INFINITY: bytes = 0x7F800000; break;
        case Number.NEGATIVE_INFINITY: bytes = 0xFF800000; break;
        case +0.0: bytes = 0x40000000; break;
        case -0.0: bytes = 0xC0000000; break;
        default:
            if (Number.isNaN(value)) { bytes = 0x7FC00000; break; }

            if (value <= -0.0) {
                bytes = 0x80000000;
                value = -value;
            }

            var exponent = Math.floor(Math.log(value) / Math.log(2));
            var significand = ((value / Math.pow(2, exponent)) * 0x00800000) | 0;

            exponent += 127;
            if (exponent >= 0xFF) {
                exponent = 0xFF;
                significand = 0;
            } else if (exponent < 0) exponent = 0;

            bytes = bytes | (exponent << 23);
            bytes = bytes | (significand & ~(-1 << 23));
        break;
    }
    return bytes;
  }
}
