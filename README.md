# Robot_Putter
### [May We Help](https://maywehelp.org/) Putt-Putt Robot


## Table Of Contents
1. [BOM](https://github.com/autonomousTurtle/Robot_Putter/blob/main/README.md#bom)
2. [Assembly](https://github.com/autonomousTurtle/Robot_Putter/edit/main/README.md#assembly)
3. [Wiring](https://github.com/autonomousTurtle/Robot_Putter/edit/main/README.md#wiring-diagram)
4. [Software](https://github.com/autonomousTurtle/Robot_Putter/tree/main/software)


## BOM

### 1. Electrical
| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Adafruit Servo Driver Breakout<br>PCA9685|1|[Adafruit](https://www.adafruit.com/product/815)|Adafruit: [Drawings and Tutorials](https://learn.adafruit.com/16-channel-pwm-servo-driver/downloads)|
|Raspberry Pi 4<br>Model B 4GB|1|[CanaKit](https://www.canakit.com/raspberry-pi-4-extreme-kit.html)<br>[Adafruit # 4292](https://www.adafruit.com/product/4292)|CanaKit includes all components necessary to use the pi.<br>Minimum 32GB SD Card|
|Arduino UNO<br>Rev3 ATmega328P|1|[Arduino #A000066](https://store-usa.arduino.cc/products/arduino-uno-rev3)<br>[Sparkfun # DEV-11021](https://www.sparkfun.com/products/11021)|[Elegoo](https://www.elegoo.com/products/elegoo-uno-r3-board) and other brands work fine as well|
|Arduino Shield<br>Through-hole protoboard|1|[Amazon Gikfun](https://www.amazon.com/Gikfun-Prototype-Shield-Arduino-Ek1038x3/dp/B00Q9YB7PI/ref=sr_1_4?keywords=Arduino+Shields&qid=1656536658&sr=8-4)|Shields with a 5V and GND rail are the easiest to work with IMO
|Dual Motor Driver<br>L298N|1|[Amazon](https://www.amazon.com/Controller-H-Bridge-Stepper-Control-Mega2560/dp/B07WS89781/ref=asc_df_B07WS89781/?tag=&linkCode=df0&hvadid=385215532707&hvpos=&hvnetw=g&hvrand=15506284023115263426&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1023946&hvtargid=pla-881401609127&ref=&adgrpid=73789134890&th=1)<br>[BC Robotics]https://bc-robotics.com/shop/l298n-motor-driver-board/)|Inludes headers. Drives two DC brushed motors, 1A constant, 3A peak
|12V Battery Pack<br>Dewalt DCB127|[Amazon Dewalt](https://www.amazon.com/DEWALT-DCB127-12V-Lithium-Battery-Pack/dp/B00KYQ1UVS/ref=asc_df_B00KYQ1UVS/?tag=hyprod-20&linkCode=df0&hvadid=309735728871&hvpos=&hvnetw=g&hvrand=13724272350015521383&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1023946&hvtargid=pla-435764226201&psc=1)<br>[Amazon TenHutt](https://www.amazon.com/dp/B07ZQ9W9RP/ref=sspa_dk_detail_4?psc=1&pd_rd_i=B07ZQ9W9RP&pd_rd_w=kYyZc&content-id=amzn1.sym.dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_p=dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_r=MXD3SNQKRF6V4AQFC5QH&pd_rd_wg=Qtt1U&pd_rd_r=8f36ccc3-3ac3-4683-98bb-6ef9187ebfa4&s=hi&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWxfdGhlbWF0aWM&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzNllQUkU1TEo1SEw0JmVuY3J5cHRlZElkPUEwMjkxNTIzMllCRUZRUUVYVTdKUyZlbmNyeXB0ZWRBZElkPUEwNjcxNzg3MkJIMjhVNDJXMU4xTyZ3aWRnZXROYW1lPXNwX2RldGFpbF90aGVtYXRpYyZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)|For use with [12V Dewalt  Charger](https://www.amazon.com/dp/B08FX153FN/ref=sspa_dk_detail_6?psc=1&pd_rd_i=B08FX153FN&pd_rd_w=kAwUn&content-id=amzn1.sym.dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_p=dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_r=551ZEE4FTNDRNSP9TZQB&pd_rd_wg=SapaK&pd_rd_r=2919e800-343d-404d-8d35-07c2f257cbcd&s=hi&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWxfdGhlbWF0aWM&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFNT1NTOUowVlBVWTAmZW5jcnlwdGVkSWQ9QTA1ODc4MDAzSTdDVU9RSlpUSkpQJmVuY3J5cHRlZEFkSWQ9QTAxMTUzMTYxN1RBRlNKUUpWR1ozJndpZGdldE5hbWU9c3BfZGV0YWlsX3RoZW1hdGljJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
|Battery Charger<br>Dewalt Charger 12V|1|[Amazon](https://www.amazon.com/dp/B08FX153FN/ref=sspa_dk_detail_6?psc=1&pd_rd_i=B08FX153FN&pd_rd_w=kAwUn&content-id=amzn1.sym.dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_p=dd2c6db7-6626-466d-bf04-9570e69a7df0&pf_rd_r=551ZEE4FTNDRNSP9TZQB&pd_rd_wg=SapaK&pd_rd_r=2919e800-343d-404d-8d35-07c2f257cbcd&s=hi&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWxfdGhlbWF0aWM&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFNT1NTOUowVlBVWTAmZW5jcnlwdGVkSWQ9QTA1ODc4MDAzSTdDVU9RSlpUSkpQJmVuY3J5cHRlZEFkSWQ9QTAxMTUzMTYxN1RBRlNKUUpWR1ozJndpZGdldE5hbWU9c3BfZGV0YWlsX3RoZW1hdGljJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
|12V - 6V DC Converter<br>|1|[Amazon](https://www.amazon.com/SMAKN%C2%AE-DC-DC-Converter-Supply-Waterproof/dp/B014ZQ6TFY)|There are many options, most will work. Does not need to be waterproof. Also want to explore this [Digikey R12P06S](https://www.digikey.com/en/products/detail/recom-power/R12P06S/7603362)
|UBEC<br>5V @ 3A DC/DC Buck Converter|1|[Adafruit #1385](https://www.adafruit.com/product/1385)<br>[Amazon](https://www.amazon.com/DEVMO-3amp-6-0-25-2v-Adjustable-Products/dp/B0971H54FQ/ref=sr_1_11_sspa?keywords=bec&qid=1656535243&sr=8-11-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFLODkyWVZFNDQwUUQmZW5jcnlwdGVkSWQ9QTA3MTExMDdTSzRZT0ZKNDU1NUQmZW5jcnlwdGVkQWRJZD1BMDk3ODYxMjI2SUtSNktVS1NOT00md2lkZ2V0TmFtZT1zcF9tdGYmYWN0aW9uPWNsaWNrUmVkaXJlY3QmZG9Ob3RMb2dDbGljaz10cnVl)|Used to power the Rasbperry Pi from the 12V battery - must have 5V output, 3A recommended
|Wago Connectors<br>|1|[Digikey 2946-887-952-ND](https://www.digikey.com/en/products/detail/wago-corporation/887-952/15553727?utm_adgroup=Connector%20Kits&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Kits&utm_term=&utm_content=Connector%20Kits&gclid=CjwKCAjwhNWZBhB_EiwAPzlhNgYpcfNqezbg7MqUFi-a7nLTFvponPX8-Ibews3kH3dT0rUR17UO0xoCeEwQAvD_BwE)<br>[Amazon](https://www.amazon.com/Lukche-LEVER-NUTS-Splicing-Connector-Assortment/dp/B095F2Z3DD/ref=asc_df_B095F2Z3DD/?tag=hyprod-20&linkCode=df0&hvadid=507844586593&hvpos=&hvnetw=g&hvrand=10366094789718545267&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9014901&hvtargid=pla-1322438535805&psc=1)|Other connectors will work (XT60, JST, EC5), Wago simply make it easy to prototype. 


### 2. Electro-Mechanical 
| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Putter Servo Motor<br>30KG 270 Degree DS3230|1|[Amazon](https://www.amazon.com/DS3230-30KG-Degree-Digital-Servo/dp/B07VN6F179)<br> [Amazon LJWRC](https://www.amazon.com/LJWRC-DS3230MG-Waterproof-Digital-Degree/dp/B088KJGM4C/ref=asc_df_B088KJGM4C/?tag=hyprod-20&linkCode=df0&hvadid=459700934310&hvpos=&hvnetw=g&hvrand=3437522193926334510&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9014901&hvtargid=pla-944719555418&psc=1)|6.8 V, Full metal gear, waterproof, includes 25T metal servo hub
|Tank Tread Base<br>SZDoit T300|1|[Amazon SZDoit](https://www.amazon.com/dp/B08HRTZNHW/ref=sspa_dk_detail_6?psc=1&pf_rd_p=3be1c5b9-5b41-4830-a902-fa8556c19eb5&pf_rd_r=4TSQEHF3RKRMFJRNAWH4&pd_rd_wg=gD69e&pd_rd_w=j3PPJ&content-id=amzn1.sym.3be1c5b9-5b41-4830-a902-fa8556c19eb5&pd_rd_r=181b3fef-27b9-4c31-8479-c1098baccc28&s=toys-and-games&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyVEw4M0FKWUgxMTFCJmVuY3J5cHRlZElkPUEwMzE1Nzc3TldCSU5HOEw4WVFVJmVuY3J5cHRlZEFkSWQ9QTA2MjkyODlCUTFFM0dHQzBFR1omd2lkZ2V0TmFtZT1zcF9kZXRhaWwmYWN0aW9uPWNsaWNrUmVkaXJlY3QmZG9Ob3RMb2dDbGljaz10cnVl)|Includes qty 2 9V DC motors with encoders
|12V High Torque Motors<br>Geartesian 550RPM|2|[Amazon](https://www.amazon.com/Greartisan-Electric-Reduction-Centric-Diameter/dp/B072R5G5GR/ref=sxts_b2b_sx_reorder_acb_business?content-id=amzn1.sym.4e83a56b-1009-4f6b-8fc7-a473c227f8c6%3Aamzn1.sym.4e83a56b-1009-4f6b-8fc7-a473c227f8c6&crid=1NL527K3Z8FTP&cv_ct_cx=12v%2Bdc%2Bmotor&keywords=12v%2Bdc%2Bmotor&pd_rd_i=B072R5G5GR&pd_rd_r=1aaec1fc-afb8-4e6a-b29b-0564b3969ed8&pd_rd_w=GCRNp&pd_rd_wg=B2sRa&pf_rd_p=4e83a56b-1009-4f6b-8fc7-a473c227f8c6&pf_rd_r=AKH4S2GXTAQ2ZEJTJQ3Y&qid=1664474246&qu=eyJxc2MiOiI2LjM1IiwicXNhIjoiNS44NiIsInFzcCI6IjUuMjUifQ%3D%3D&sprefix=12v%2Bdc%2Bmoto%2Caps%2C386&sr=1-1-62d64017-76a9-4f2a-8002-d7ec97456eea&th=1)|
|Adaptive Controller<br>XBox|1|[Best Buy](https://www.bestbuy.com/site/microsoft-xbox-adaptive-controller-white/6455394.p?skuId=6455394&ref=212&loc=1&gclid=CjwKCAjwhNWZBhB_EiwAPzlhNkHY-ifXdZ5iwV64-iOxaZOUqwsdu-zkY2C9Vu1peYfDs0UZBJuvKxoCEgUQAvD_BwE&gclsrc=aw.ds)|[Any switch with a 3.5mm jack](https://www.ablenetinc.com/switches/all-switches/) can be used with this controller. 
|Warfighter Engaged<br>Joystick FPS|1|[Warfighter](https://warfighterengaged.org/shop/joystix-l-aenef)


### 3. Mechanical

| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Printer Filament<br>PLA|1|[MatterHackers M-EED-KTKU](https://www.matterhackers.com/store/l/175mm-pla-filament-white-1-kg/sk/MEEDKTKU)| Color does not matter. Diameter for most filament printers should be 1.75mm |
|Ball Bearing<br>Shielded|1|[Mcmaster ]([https://www.matterhackers.com/store/l/175mm-pla-filament-white-1-kg/sk/MEEDKTKU](https://www.mcmaster.com/5972K293/))| For 30mm shaft diameter |

### 4. 3D Prints
All of the custom parts are 3D Printed on a filamnet printer with PLA. 

| Description | Quantity | Ordering Information | Comments |  
| --- | :---: | --- | --- |
|Putter Servo Mount|1|[MatterHackers M-EED-KTKU](https://www.matterhackers.com/store/l/175mm-pla-filament-white-1-kg/sk/MEEDKTKU)| Holds the 30kg-cm servo |
|Ball Bearing Inner Housing|1|[BallBearingAttachToRobot_Big_1.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/BallBearingAttachToRobot_Big_1.STL)| Mounts to Cover Front part |
|Ball Bearing Outer Housing|1|[BallBearingAttachToRobot_little_1.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/BallBearingAttachToRobot_little_1.STL)| Holds ball bearing in place wtih the inner housing |
|Servo Horn Attachment|1|[GolfClubAttach_rev2_bottom_1.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/GolfClubAttach_rev2_bottom_1.STL)| Attaches the servo horn and fits inside of the ball bearing |
|Putter Shaft Holder|1|[GolfClubAttach_rev2_top_1.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/GolfClubAttach_rev2_top_1.STL)| Attaaches to the putter shaft on one end and to the Servo Horn Attachment on the other end |
|Putter Head|1|[PutterHead.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/PutterHead.STL)| Feel free to use whatever putter head you want, but 3D printing makes the robot lighter |
|Motor Driver Breakout Board Mount|1|[Motor_Driver_Board_Holder_rev1.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/Motor_Driver_Board_Holder_rev1.STL)| Mounts the L298N breakout board to the bottom plate of the robot base |
|Arduino Uno Mount|1|[Mcmaster ]([https://www.matterhackers.com/store/l/175mm-pla-filament-white-1-kg/sk/MEEDKTKU](https://www.mcmaster.com/5972K293/))| Used of attaching the Uno to the top of the raspberry pi cana kit case |
|Cover Front|1|[cover_front_0.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/cover_front_0.STL)| Covers the front of the robot internals |
|Cover Rear|1|[cover_back_0.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/cover_back_0.STL)| Covers the rear of the robot internals |
|Cover Top|1|[cover_top_cover_0.STL](https://github.com/autonomousTurtle/Robot_Putter/blob/main/3D%20Prints/cover_top_cover_0.STL) | I used clear Lexan for this, but you can also 3D print it |

### Assembly

### Wiring Diagram


### Software Architecture:

* Arduino:  software/drive_and_putt/drive_and_putt.ino
* Raspberry Pi: software/deployed/drive_robot.py



