/**
 * Copyright (c) 2023 Shenzhen Dingtian Technologies Co.,Ltd All rights reserved.
 * file: ethernet.ino
 * author: lzp<lzp@dingtian-tech.com>
 * date: 2023/12/27
 * function: relay board ethernet demo for arduino
 */

#include <ETH.h>

/////////////////////gpio define///////////////////////////////////////
/* UART0 */
#define GPIO_TXD0   1
#define GPIO_RXD0   3

/* RMII */
#define GPIO_RMII_RXD0  25
#define GPIO_RMII_RXD1  26
#define GPIO_RMII_CRS   27
#define GPIO_RMII_TXD1  22
#define GPIO_RMII_TXD0  19
#define GPIO_RMII_TXEN  21
#define GPIO_RMII_CLK   17
#define GPIO_RMII_MDC   23
#define GPIO_RMII_MDIO  18

/* CAN */
#define GPIO_CAN_RX   4
#define GPIO_CAN_TX   5

/////////////////////relay channel select///////////////////////////////////////

//#define DT_2CH  1
#define DT_4CH  1
//#define DT_8CH  1    /* for hw version < v3.6.11 */
//#define DT_16CH 1    /* for hw version < v3.6.0 */
//#define DT_32CH 1    /* for hw version < v3.4.0 */
//#define DT_8CH_V2  1    /* for hw version >= v3.6.11 */
//#define DT_16CH_V2 1    /* for hw version >= v3.6.0 */
//#define DT_32CH_V2 1    /* for hw version >= v3.4.0 */

#if (1 == DT_2CH) || (1 == DT_4CH) || (1 == DT_8CH) || (1 == DT_8CH_V2)
#define GPIO_RMII_PWR 0
#elif (1 == DT_16CH) || (1 == DT_32CH) || (1 == DT_16CH_V2) || (1 == DT_32CH_V2)
#define GPIO_RMII_PWR 12
#else
#define GPIO_RMII_PWR 0
#endif

// ethernet pin define
#define USE_ETHERNET                             // Add support for ethernet (+20k code)
  // Dingtian Ethernet
  #define ETH_PHY_TYPE        ETH_PHY_JL1101
  #define ETH_PHY_ADDR        1
  #define ETH_PHY_MDC         GPIO_RMII_MDC
  #define ETH_PHY_MDIO        GPIO_RMII_MDIO
  #define ETH_PHY_POWER       GPIO_RMII_PWR
  #define ETH_CLK_MODE        ETH_CLOCK_GPIO17_OUT

//// eth event callback
int eth_connected = false;
// WARNING: onEvent is called from a separate FreeRTOS task (thread)!
void onEvent(arduino_event_id_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      // The hostname must be set after the interface is started, but needs
      // to be set before DHCP, so set it from the event handler thread.
      ETH.setHostname("esp32-ethernet-dt");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED: Serial.println("ETH Connected"); break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.println("ETH Got IP");
      Serial.println(ETH);
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_LOST_IP:
      Serial.println("ETH Lost IP");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setup() {
  // init serial log
  Serial.begin(115200);
  delay(1000); /* wait serial ok */

  // ethernet init
  Serial.println("Ethernet Init >>>>");
  Network.onEvent(onEvent);
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);
  ETH.config(IPAddress(192,168,1,100),IPAddress(192,168,1,1),IPAddress(255,255,255,0),IPAddress(192,168,1,1), IPAddress(192,168,1,1));
  Serial.println("Ethernet Init <<<<");

  // put your setup code here, to run once:
}

void loop() {
  delay(1000); /* must give time to watchdog, otherwise crash */

  // put your main code here, to run repeatedly:
  //Serial.println("loop");
  
}
