#ifndef CONSTANTS_H
#define CONSTANTS_H

const char html[] = "<!doctype html><html lang='en'><head>  <meta charset='utf-8'>  <meta name='viewport' content='width=device-width, initial-scale=1'>  <title>CO2 Device Configuration</title></head><body><style>body{display:flex;margin:0;padding:0;font-family:Arial,Helvetica,sans-serif;background-color:#f5f5f5}h2{text-align:center;margin:0}p{font-size:.9em}.category{font-size:.8em;font-weight:700;margin:0}hr{color:#ddd;border:1px solid;margin-top:5px;margin-bottom:15px}#container{margin-left:auto;margin-right:auto;width:330px;padding:20px}label{font-weight:700;font-size:.8em;display:block;margin-bottom:5px}input{box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;margin:0;padding:10px;font-size:16px;border:1px solid #ddd;width:100%;display:block}button{background-color:#0d6efd;border-color:#0d6efd;border:1px solid;color:#fff;display:inline-block;text-align:center;text-decoration:none;vertical-align:middle;width:100%;padding:10px;margin-top:20px;font-size:1.5em}</style>  <div id='container'>    <h2>CO2 Sensor Configuration</h2><br><br>    <form method='GET' action='/saveconfig' id='form1'>      <span class='category'>Wi-Fi credentials</span>      <hr>      <input type='text' id='ssid' name='ssid' placeholder='SSID'><br>      <input type='password' id='password' name='password' placeholder='Password'><br>      <span class='category'>API configuration</span>      <hr>      <input type='text' id='deviceid' name='deviceid' placeholder='Device ID'><br>      <input type='text' id='username' name='username' placeholder='Username'>      <button type='submit' form='form1' value='Save'>Save</button>    </form>  </div></body></html>";
const char htmlConfigurationSaved[] = "<!doctype html><html lang='en'><head>  <meta charset='utf-8'>  <meta name='viewport' content='width=device-width, initial-scale=1'>  <title>CO2 Device Configuration</title></head><body><style>body{display:flex;margin:0;padding:0;font-family:Arial,Helvetica,sans-serif;background-color:#f5f5f5}h2{text-align:center;margin:0}p{font-size:.9em}#container{margin-left:auto;margin-right:auto;width:330px;padding:20px}</style>  <div id='container'>    <h2>Configuration saved</h2><br><br>    <p>This device will connect to your Wi-Fi access point and send data in a few seconds.</p>    <p>If you have an active internet connection, you can check out the dashboard <a href='https://co2.nikku.nl'>here</a></p>    <hr>    <p><strong>Having problems?</strong><br>You can turn the device off and on again while holding the factory settings button, this will allow you to enter your information again, please make sure all information is entered correctly.</p>  </div></body></html>";

//ESP32 as access-point credentials
const char *ap_ssid = "CO2Sense";
const char *ap_password = "password";

//Keep track of SSID and password to autofill this in the config page
char ssid[32];
char password[32];

char deviceid[32];
char username[32];
#endif
