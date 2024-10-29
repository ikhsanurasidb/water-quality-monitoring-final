# Water Quality Monitoring using AWS IoT Core

<img width="500" alt="image" src="https://github.com/user-attachments/assets/644936e4-7c05-402d-87c2-bdeebae34866">

## 1. Setup Your AWS IoT Core
You can find comprehensive tutorials on YouTube or other articles, but here’s a summary of the steps:
- Create an AWS account
- Create an AWS IoT policy
-	Create an AWS IoT thing, certificate, and private key
- Find your endpoint: Navigate to the device manager, scroll to the bottom, and find "Settings". Your endpoint will be listed there. 

## 2. Code for Microcontroller
In this setup, I’m using an Arduino Uno to control the sensor and robotic arm, with an ESP8266 as a gateway to send data to the AWS IoT cloud.
- Upload sketch Gateway.ino to your ESP8266.
- Upload sketch Controller.ino to tour Arduino.
   
## 3. Web Based Monitoring
Untuk menggunakan ini anda harus sudah menginstall <a href="https://nodejs.org/en/download/package-manager">Node.js</a>

First, run the development server:

```bash
npm run dev
# or
yarn dev
# or
pnpm dev
# or
bun dev
```
Then, open [http://localhost:3000](http://localhost:3000) with your browser.
