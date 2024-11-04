#include <M5Unified.h>
#include <SPI.h>
#include <M5_Ethernet.h>
#include <time.h>
#include "NtpClient.hpp"

// For M5CoreS3
#define SCK 36
#define MISO 35
#define MOSI 37
#define CS 9

#define NTP_PACKET_SIZE 48

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

String timeServer = "192.168.25.77"; // time.nist.gov NTP server

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Power.begin();
  Serial.begin(9600);
  SPI.begin(SCK, MISO, MOSI, -1);
  Ethernet.init(CS);

  M5.Display.println("UdpNtpClient");

  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    }
    else if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true)
    {
      delay(1000);
    }
  }

  NtpClient.begin();
}

void loop()
{
  M5.Display.setCursor(0, 12);

  String timeLine = NtpClient.getTime(timeServer, +9);
  M5.Display.println(timeLine);
  Serial.println(timeLine);
  delay(1000);
  Ethernet.maintain();
}
