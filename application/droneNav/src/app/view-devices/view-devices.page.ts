import { Component, OnInit } from '@angular/core';
import { ModalController } from '@ionic/angular';

@Component({
  selector: 'app-view-devices',
  templateUrl: './view-devices.page.html',
  styleUrls: ['./view-devices.page.scss'],
})
export class ViewDevicesPage implements OnInit {

  parentPage = null;
  deviceList = [];
  selectedDevice = null;

  constructor(public viewCtrl: ModalController) { }

  connect(){
    this.parentPage.connectToDevice(this.selectedDevice);
    this.dismiss();
  }
  ngOnInit() {
  }

  changeSelectedDevice(dev){
    this.selectedDevice = dev;
    console.log("Set ID to " + dev.id);
  }

  dismiss() {
    this.viewCtrl.dismiss();
  }

}
