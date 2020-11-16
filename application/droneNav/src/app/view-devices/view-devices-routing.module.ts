import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ViewDevicesPage } from './view-devices.page';

const routes: Routes = [
  {
    path: '',
    component: ViewDevicesPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class ViewDevicesPageRoutingModule {}
