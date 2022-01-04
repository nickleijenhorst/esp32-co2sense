#ifndef CONSTANTS_H
#define CONSTANTS_H

const char* rawTemplateHtml = "<!doctype html><html lang='en'><head>  <meta charset='utf-8'>  <meta name='viewport' content='width=device-width, initial-scale=1'>  <title>CO2 Device Configuration</title></head><body><style>body{display:flex;margin:0;padding:0;font-family:Arial,Helvetica,sans-serif;background-color:#f5f5f5}h2{text-align:center;margin:0}p{font-size:.9em}.category{font-size:.8em;font-weight:700;margin:0}hr{color:#ddd;border:1px solid;margin-top:5px;margin-bottom:15px}#container{margin-left:auto;margin-right:auto;width:330px;padding:20px}label{font-weight:700;font-size:.8em;display:block;margin-bottom:5px}input{box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;margin:0;padding:10px;font-size:16px;border:1px solid #ddd;width:100%;display:block}button{background-color:#0d6efd;border-color:#0d6efd;border:1px solid;color:#fff;display:inline-block;text-align:center;text-decoration:none;vertical-align:middle;width:100%;padding:10px;margin-top:20px;font-size:1.5em}</style>  <div id='container'>    <h2>CO2 Sensor Configuration</h2><br><br>    <form method='GET' action='/saveconfig' id='form1'>      <span class='category'>Wi-Fi credentials</span>      <hr>      <input type='text' id='ssid' name='ssid' placeholder='SSID' value='{SSID}'><br>      <input type='password' id='password' name='password' placeholder='Password' value='{PASSWORD}'><br>      <span class='category'>API configuration</span>      <hr>      <input type='text' id='deviceid' name='deviceid' placeholder='Device ID' value='{DEVICEID}'><br>      <input type='text' id='username' name='username' placeholder='Username' value='{USERNAME}'>      <button type='submit' form='form1' value='Save'>Save</button>    </form>  </div></body></html>";
String html = String("");
const char htmlConfigurationSaved[] = "<!doctype html><html lang='en'><head>  <meta charset='utf-8'>  <meta name='viewport' content='width=device-width, initial-scale=1'>  <title>CO2 Device Configuration</title></head><body><style>body{display:flex;margin:0;padding:0;font-family:Arial,Helvetica,sans-serif;background-color:#f5f5f5}h2{text-align:center;margin:0}p{font-size:.9em}#container{margin-left:auto;margin-right:auto;width:330px;padding:20px}</style>  <div id='container'>    <h2>Configuration saved</h2><br><br>    <p>This device will connect to your Wi-Fi access point and send data in a few seconds.</p>    <p>If you have an active internet connection, you can check out the dashboard <a href='https://co2.nikku.nl'>here</a></p>    <hr>    <p><strong>Having problems?</strong><br>You can turn the device off and on again while holding the factory settings button, this will allow you to enter your information again, please make sure all information is entered correctly.</p>  </div></body></html>";

//ESP32 as access-point credentials
const char *ap_ssid = "CO2Sense";
const char *ap_password = "password";

const char* endpoint = "https://co2.nikku.nl/api";

const char* root_ca= \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \
  "WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
  "RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
  "AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
  "R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
  "sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
  "NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
  "Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
  "/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
  "AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
  "Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
  "FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
  "AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
  "Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
  "gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
  "PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \
  "ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
  "CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
  "lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
  "avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
  "yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
  "yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
  "hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
  "HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
  "MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
  "nLRbwHOoq7hHwg==\n" \
  "-----END CERTIFICATE-----\n";

#endif
