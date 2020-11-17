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

  constructor(public viewCtrl: ModalController) { }

  connect(dev){
    this.parentPage.connectToDevice(dev);
    this.dismiss();
  }
  ngOnInit() {
  }

  dismiss() {
    this.viewCtrl.dismiss();
  }

}
