/*
  Rui Santos
  Complete project details at:
   - ESP32: https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
   - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Example adapted from: https://github.com/mobizt/ESP-Mail-Client
*/

// To send Emails using Gmail on port 465 (SSL), you need to create an app password: https://support.google.com/accounts/answer/185833
#include <ESP8266WiFi.h>
#include <Arduino.h>

#include <ESP_Mail_Client.h>

#define WIFI_SSID "<ENTER YOUR WIFI NAME(SSID)>"
#define WIFI_PASSWORD "<ENTER YOUR WIFI PASSWORD>"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "<ENTER SENDER'S E-MAIL>"
#define AUTHOR_PASSWORD "<ENTER SENDER'S E-MAIL'S PASSWORD>"

/* Recipient's email*/
#define RECIPIENT_EMAIL "<ENTER RECIPIENT'S EMAIL>"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup(){
  Serial.begin(115200);
  Serial.println();
  String start = Serial.readString();
  Serial.print(start);
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
 
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);
  Serial.println("smtp debug enabled.");
  

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);
  Serial.println("smtp callback set.");

  /* Declare the session config data */
  ESP_Mail_Session session;
  Serial.println("esp mail session declared.");

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  Serial.println("session config set.");

  /* Declare the message class */
  SMTP_Message message;
  Serial.println("smtp message declared.");

  /* Set the message headers */
  message.sender.name = "ESP8266";         //You can edit here however you want
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Hirsiz Algilandi !";  //You can edit here however you want
  message.addRecipient("Bulut", RECIPIENT_EMAIL);
  Serial.println("message headers set.");

  /*Send HTML message*/                   //You can edit here however you want
  String htmlMsg = "<div><h1 style=\"color:#FF0000;\">Hırsız Algılandı !</h1>Lazer Güvenlik Sistemi devreye girdi.<br>Lütfen istenmeyen misafirlere karşı sisteminizi kontrol edin.<br>Bu mesaj güvenlik sistemi tarafindan iletildi.</div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  Serial.println("html message sent.");

  /*
  //Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");
  
  
  
  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;
  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
   
}

void loop(){

}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
