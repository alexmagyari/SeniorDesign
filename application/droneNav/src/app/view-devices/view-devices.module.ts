import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { ViewDevicesPageRoutingModule } from './view-devices-routing.module';

import { ViewDevicesPage } from './view-devices.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    ViewDevicesPageRoutingModule
  ],
  declarations: [ViewDevicesPage]
})
export class ViewDevicesPageModule {}
