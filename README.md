# FlatSat project ENSIL-ENSCI
## Introduction

This project's main goal is to develop the Space world within engineering school. For this purpose, the Easy Space association has provided an educational kit including a micro-satellite, the FlatSat, which has been studied by explaining the different functions of the electronic cards present. They aim to illustrate the main functions of a satellite, namely energy management, data collection, telecommunications and coordination.

Two telecommincations modes were studied to communicate data to a ground base, WiFi and LoRa. The Arduino IDE was used to program the Heltec card to retrieve data from the sensors and etablish a wireless link with the computer.The Wifi connection allowed data to be sent faster but over a shorter distance than LoRa. All data was retrieved as a text file for processing in a Matlab script.

Finally, a structure to include the FlatSat in a cubi form on a drone was developed. This structure made it possible to carry out measurements on the drone, using the previous telecommunication modes.

Here, you can find a tutorial to configure the Arduino IDE for Heltec and MK1300 WAN card.

-----

## Arduino IDE configuration for the Heltec ESP32 board

* Install the Arduino IDE : [Arduino IDE](https://www.arduino.cc/en/software)

* go to `file -> preferences`

![1_1](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fdff0c6c7-5aef-4217-8ac0-218e762cea7a%2F1_1.png?table=block&id=9e6edf58-ae2b-40b4-be98-a10fd4d37c3d&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=920&userId=&cache=v2)

* in `Additional Boards Manager URLs`, add the link: `https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.5/package_heltec_esp32_index.json`

![1_2](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Faa28926f-481a-4534-8ac7-c4326d02e1ac%2F1_2.png?table=block&id=37e5a054-8458-4960-8b6b-ac66ec9d1da6&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1210&userId=&cache=v2)
![1_3](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Ff3bc9864-bae7-4a6e-81d2-7995dd978b2c%2F1_3.png?table=block&id=a12bd87c-3225-454e-b240-22175e7aba5f&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=980&userId=&cache=v2)

* Add the ESP32 card manager : `tools -> Board -> Boards Manager`

![1_4](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fd9b2a9c4-eee7-4f62-906b-a38242cb507d%2F1_4.png?table=block&id=af67ae91-c4c5-4866-8add-773579f2d61d&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1030&userId=&cache=v2)

* type 'Heltec ESP32' in the search bar and install

![1_5](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F592c250f-7d01-4563-9da1-36139c69ed8e%2F1_5.png?table=block&id=ecda5b82-80af-421a-bd35-f1a4503d776b&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1070&userId=&cache=v2)

* Define the board to be used : `tools - >Boards -> Heltec ESP32 Arduino -> Wireless Stick Lite`

![1_6](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F46577bcb-28b1-4250-bcdf-718ca933f1f5%2F1_6.png?table=block&id=d383a825-51c4-413c-9e06-cdcc341ea5f5&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1170&userId=&cache=v2)

* Install ESP32 libraries : `sketch -> include Library -> Manage Libraries`

![1_7](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F86e9b164-3217-4861-aafc-45384fc69a58%2F1_7.png?table=block&id=bcb7977c-3c7b-483d-adae-1faae6337396&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=960&userId=&cache=v2)

* Type 'Heltec ESP32' in the search bar and install 'Heltec ESP32 DEV-BOARD'

![1_8](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F4d773821-a9fa-4ef9-b3b1-486ccce024eb%2F1_8.png?table=block&id=4ff99053-fb61-417b-b7c9-d6972a930116&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1360&userId=&cache=v2)

### Installation of the remaining libraries: launch the program (give a name)

* Add to the folder "C:\Users\arman\Documents\Arduino\libraries" the files :
  o ESP32Ping
  o AsyncTCP
  o ESPAsyncWebServer
  o Arduino_JSON
  o LoRa
  
![1_9](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F3ae28955-4238-4330-bc6d-388411238919%2F1_9.png?table=block&id=307cbf12-2cdd-44c4-9851-0531d0fc0dee&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=990&userId=&cache=v2)

* Compile and check for errors

---------------------------

## Add ESP32 Sketch Data Upload 

Source : [randomnertutorials](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/)

* Download the file 'ESP32FS-1.0.zip'
* Unzip the file to the file location: C:\Program Files (x86)\Arduino\tools

![2_1](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F36abd60d-308f-4843-8248-8b868d738d51%2F2_1.png?table=block&id=36b316b7-e394-4a32-b3b8-2b789d339f09&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1100&userId=&cache=v2)

* Restart the IDE and look under tools for "ESP32 Sketch Data Upload".

![2_2](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F2c2649fe-5e14-4ad1-825e-87a30d6b5f03%2F2_2.png?table=block&id=b2cd154b-5591-436a-a14a-40b731cc7bbf&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=940&userId=&cache=v2)

* This allows you to put .html and .js files for web display. Everything must be included in a single file named _data_ in the same folder as the :

![2_3](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F6cf205e3-d1a4-4936-9472-ac27134d6899%2F2_3.png?table=block&id=1293ecca-97b6-4da6-9be7-bf05f4e1d31d&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1920&userId=&cache=v2)


## MKR WAN 1300 board

* Add the board to the board menu: `tools -> Boards -> Boards manager`

![3_1](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F9b93006e-c14c-4a4b-8617-c556726a258d%2F3_1.png?table=block&id=125f0b46-2c00-42b0-9398-f390e9b8a1c7&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1160&userId=&cache=v2)

* Search for 'MKR WAN' in the search bar and install

![3_2](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F752bf474-719e-453b-ab26-75759ac9b8f6%2F3_2.png?table=block&id=eb9f8b86-bb2a-4f22-ae69-05f17053a119&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1280&userId=&cache=v2)

* Check the correct functioning of the Board using the example program in : `file -> example -> LoRa -> LoRaReceiver` and by plugging in the MKR WAN 1300 BOARD and removing it from its antistatic foam

![3_3](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F8fcdb06b-2333-48e2-aab6-8926c91359bc%2F3_3.png?table=block&id=f281abb8-8980-4457-bb85-df5e4222c0cc&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1110&userId=&cache=v2)

* Select the board by doing: `tools-> boards -> Arduino SAMD (....) BOARDS -> Arduino MKR WAN 1300` and selecting the right port (`tools -> port`)

![3_4](https://armand-guzzonato.notion.site/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fe8f0e19e-3e96-4468-8bf7-6b05a2b3a498%2F3_4.png?table=block&id=dcb8abf6-01dc-4b5c-b819-cbd1dfa5ff82&spaceId=dc9b6554-9915-4b11-9e21-0a4be375f44c&width=1300&userId=&cache=v2)



### If the board is not detected, perform an update with the MKRWAN file:
* Put the MKRWAN file in the file `C:\Users\Admin\Documents\Arduino\libraries`
* Go to the examples and launch 'MKRWANFWUpdate_standalone.ino'
* Run the program
