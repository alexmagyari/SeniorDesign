import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { ViewDevicesPage } from './view-devices.page';

describe('ViewDevicesPage', () => {
  let component: ViewDevicesPage;
  let fixture: ComponentFixture<ViewDevicesPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ViewDevicesPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(ViewDevicesPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
