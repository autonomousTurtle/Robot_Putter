# Robot_Putter
May we help electrtonic putter

https://maywehelp.org/ 

robot_putter_code_rev1 is the arduino program that controls the robot - it acts as the rf receiver
putter_controller_rf_code is the arduino program that controls the remote control. It is the rf 

### Assembly

### Wiring Diagram

## BOM

### 1. Electrical
| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Adafruit Servo Driver Breakout<br>PCA9685|1|[Adafruit](https://www.adafruit.com/product/815)|Adafruit: [Drawings and Tutorials](https://learn.adafruit.com/16-channel-pwm-servo-driver/downloads)|
|Raspberry Pi 4<br>Model B 4GB|1|[CanaKit](https://www.canakit.com/raspberry-pi-4-extreme-kit.html)<br>[Adafruit # 4292](https://www.adafruit.com/product/4292)|CanaKit includes all components necessary to use the pi.<br>Minimum 32GB SD Card|
|Dual Motor Driver<br>Pololu Qik 2s9v1|1|[Pololu #1110](https://www.pololu.com/product/1110)<br>[DigiKey # 2183-1110-ND](https://www.digikey.com/en/products/detail/pololu-corporation/1110/10450402)|Inludes headers. Drives two DC brushed motors, 1A constant, 3A peak
|12V Battery Pack<br>Tenergy 2000mAh NiMH|2|[Amazon](https://www.amazon.com/Tenergy-Capacity-Rechargeable-Replacement-Equipments/dp/B077Y9HNTF?th=1)<br>[Tenergy Power #11606](https://power.tenergy.com/tenergy-nimh-12v-2000mah-battery-pack-w-bare-leads/)|Requires [T-Plug headers](https://www.progressiverc.com/products/t-plug-connectors) <br>For use with [6V-12V Tenergy Smart Charger](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-6v-12v-ul/)
|Battery Charger<br>Tenergy NiMH Charger 6-12V|1|[Tenergy Power #01025](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-6v-12v-ul/)|Can also use [Tenergy NiHM 7.2-12V Charger](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-7-2v-12v-ul/)
|AI 3D Camera<br>Luxonis OAK-D|1|[Luxonis OAK-D](https://shop.luxonis.com/products/oak-d)<br>[DigiKey #SEN-17770](https://www.digikey.in/en/products/detail/sparkfun-electronics/SEN-17770/13901888?s=N4IgjCBcoLQBxVAYygMwIYBsDOBTANCAPZQDa4ArAGwIC6AvvYQExkgDKAogHIxgDsggAwgGQA)|Requires high throughput USBC cable, inlcuded with camera
|12V - 6V DC Converter<br>|1|[Amazon](https://www.amazon.com/SMAKN%C2%AE-DC-DC-Converter-Supply-Waterproof/dp/B014ZQ6TFY)|There are many options, most will work. Does not need to be waterproof
|UBEC<br>5V @ 3A DC/DC Buck Converter|1|[Adafruit #1385](https://www.adafruit.com/product/1385)<br>[Amazon](https://www.amazon.com/DEVMO-3amp-6-0-25-2v-Adjustable-Products/dp/B0971H54FQ/ref=sr_1_11_sspa?keywords=bec&qid=1656535243&sr=8-11-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFLODkyWVZFNDQwUUQmZW5jcnlwdGVkSWQ9QTA3MTExMDdTSzRZT0ZKNDU1NUQmZW5jcnlwdGVkQWRJZD1BMDk3ODYxMjI2SUtSNktVS1NOT00md2lkZ2V0TmFtZT1zcF9tdGYmYWN0aW9uPWNsaWNrUmVkaXJlY3QmZG9Ob3RMb2dDbGljaz10cnVl)|Used to power the Rasbperry Pi from the 12V battery - must have 5V output, 3A recommended
|T-Plug Connectors<br>|1|[ProgressiveRC #AC-TC](https://www.progressiverc.com/products/t-plug-connectors)<br>[Amazon](https://www.amazon.com/Hobbypark-T-Plug-Connectors-Battery-Electric/dp/B01C8NWJ78)|Other battery connectors will work (XT60, JST, EC5) just make sure it is secure. 


### 2. Electro-Mechanical 
| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Putter Servo Motor<br>Tenergy NiMH Charger 6-12V|1|[Tenergy Power #01025](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-6v-12v-ul/)|Can also use [Tenergy NiHM 7.2-12V Charger](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-7-2v-12v-ul/)
|Camera Servo Motor<br>Tenergy NiMH Charger 6-12V|1|[Tenergy Power #01025](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-6v-12v-ul/)|Can also use [Tenergy NiHM 7.2-12V Charger](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-7-2v-12v-ul/)
|Tank Tread Base<br>Tenergy NiMH Charger 6-12V|1|[Tenergy Power #01025](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-6v-12v-ul/)|Can also use [Tenergy NiHM 7.2-12V Charger](https://power.tenergy.com/tenergy-smart-universal-charger-for-nimh-nicd-battery-packs-7-2v-12v-ul/)
|Adaptive Controller<br>Nintendo HORI Flex|1|[HORI #NSW-280U](https://stores.horiusa.com/flex-controller-for-nintendo-switch/?showHidden=true)|[Any switch with a 3.5mm jack](https://www.ablenetinc.com/switches/all-switches/) can be used with this controller. 

### 3. Mechanical
