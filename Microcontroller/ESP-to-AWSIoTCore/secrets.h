#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "ESP32_TestMQTT"     //change this                         
 
const char WIFI_SSID[] = "xxxxx";               //change this
const char WIFI_PASSWORD[] = "xxxxx";          //change this
const char AWS_IOT_ENDPOINT[] = "xxxx";       //change this
 
// Amazon Root CA 1                                     //change this
static const uint8_t AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const uint8_t AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAMK/A9JxaeZc/KZxFEWnGcbi0NJhMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA3MDgwMTUz
MjFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCgW3l/ucN34+7hqJn5
YiEtFnpsjO9qH7fO5Wz8FW/Zn6E4Dt0iobf2veSDWdgSUPirBmN3i0GYfdjfHGgO
ps6A/N/ZBC6gr2zKDFHYtyTupEfIggtss6n6Aq9Gk0+k+wDnX6yS1/x3fdS5uXGW
ua8XD6WI4o5z3HYZv/Z6dEsCPn7lf7x8TXn6xRKMn9udHz8zoU4kVrOxuoikYDor
NQkzrXr2uzi1P2NzEUFpVZbuFlq3nrJiHU2y6K357kMIEYNI5zI2Gljt6t0Jcj+p
QgrqAwsqCNCt0wlyZ2xQ98eb6/a5Rzi9o5sDK4E/paEFgrACjx72KC3WCPP80CK5
bNkhAgMBAAGjYDBeMB8GA1UdIwQYMBaAFHaeoh9ysL4AiAZYDkmGq34kt8HdMB0G
A1UdDgQWBBRuuulLYgM9DV+Xho0wgJ/Ebams8DAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAtxYExzyIAQF6PyNwvaeeWTPF
mBlZ+35BlrucRf/Twv3Dyep60nH21JNqn01YyHaSOVs4L3QSRFEiquqawQPWmclG
4TmzqWatiouOHpsiPo887i6my7oi854Trutm/DoPPXhG1iy+TtjU6jqzKrPrkWgN
+tq3oY6DICW2f7fuZJCCn3iPODf9MWQGK4SlTxUvqZcz3r7JdsfsmsemkFG4yEbS
4WDMneu/uxx3veWSi9+PkOvit9Gfe1MKeXIIBJ3GMSUJUFd3lHWEkuhRqRM8Kkij
W3DM6CeUUEq1lh1Pkl+V1LTfVgISHmSoyNm2spclhEN18TdhyH4by8wgJJXZzQ==
-----END CERTIFICATE-----

 
 
)KEY";
 
// Device Private Key                                               //change this
static const uint8_t AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAoFt5f7nDd+Pu4aiZ+WIhLRZ6bIzvah+3zuVs/BVv2Z+hOA7d
IqG39r3kg1nYElD4qwZjd4tBmH3Y3xxoDqbOgPzf2QQuoK9sygxR2Lck7qRHyIIL
bLOp+gKvRpNPpPsA51+sktf8d33UublxlrmvFw+liOKOc9x2Gb/2enRLAj5+5X+8
fE15+sUSjJ/bnR8/M6FOJFazsbqIpGA6KzUJM6169rs4tT9jcxFBaVWW7hZat56y
Yh1Nsuit+e5DCBGDSOcyNhpY7erdCXI/qUIK6gMLKgjQrdMJcmdsUPfHm+v2uUc4
vaObAyuBP6WhBYKwAo8e9igt1gjz/NAiuWzZIQIDAQABAoIBAH1r7pTa+6q7CW1Q
TNYETYSRb0GV3fA4TLCzO1HThn9FXniVD5r1uAhyL9gYnmp9EHHb9lJbBfw6ejTP
aAgFqcCziHWZ0sp1SvahOibx4B4TOd328O6de5YDIYpmz8VvWR6e80CLVHk1XvvA
sro8E1lLMKLm6JpwVaMARM00RjLC59OC/lY+xdjIdAbhj8gxtqQnFAmc83AKz6Cf
ByAcLvKvSMo2jtwpwXVqyMnmHQQJRFMtNI1fcFul357a9z9bAxR1p/txbm2ILk3v
uZV6/CclVHT9HV8u6qvcPN1V5zzYEhSSoA73+GoRmPE7bPMox3ePdcdAp50CRgdR
7fgsOR0CgYEAzBE8opnVq0YTMAWAOXdKFLmiciDONtbi/ad59uXzJWaor9idvEQr
sjsBEXK398KoD1TMF1PdCT5Nw1t1++mMA9SS5+etViZ0xvODIHhVuMyqGKroRtWa
nsz7q4Z3ctte5wDodoma+CmN7LYhfiW6+uR0DHyMYfogOd0QXke+WPMCgYEAySqU
G8gAwRGbNkJ5iA8fElM2LgLHceXDXOJEUhZ4QcSqzL0AFvCoJeieWj2/zYP3p4XL
c0YL+qAIsErnOC4JjGwPYy/rPbTYPS4XUWMh5p4r7RYTRuUwYa7AKD5y51bpRUFa
kLZaeZPSJ5LWrNoq/W11rRLFGXJAXGJSlOxeipsCgYBOJKubeF0h4Qun7/NSDiQI
G63hFj7hMRhrGZ7StOuO/ujJraF5XoF9FqsqC0v3TEse09HzfOG/kGQm0vczKBO9
w8EdnNH7k25UYRtta6cnxPe9ynR364PyG3ykCjHjga2tt+NMwRTewsreQkBBFiGr
wZ/OyELZofaFMen6sG5rkQKBgQCfDXxHLu5xuKL41rxSeGo/ye0XmfjptBmwUfJk
2QFCNuY4tOLiQGsidfbmMNZ6/vGL0byXqvWDto7IHKEOiU/hZ/sOmG+tvVQPo2aq
YfAG8F6YuRCKyGFlio7CAwZEpTIRIOgjbRrB77+ErJPx3RmKgMY/Do0qXmxG3zKe
LvldKwKBgQCyOuTEWJx/28f1YPp0f2xCphcIC1NimNSFw3G0rudxvpYJPNdBvO1z
X1Zp3NKnSUCLDuUCWnUywygl/2qy+6LBIh98g3JIWpKxktiLS0RRSt/2FxVT/6kB
ujnZfFwlH3t65T5m9uRiwaWYoFYtbNBMmw/ZdPc9ShkfZ92YODiZ0Q==
-----END RSA PRIVATE KEY-----


 
 
)KEY";
