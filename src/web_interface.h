#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <string>
#include <SimpleFTPServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <defines.h>

namespace robo
{
    class WebInterface
    {
        private:
            /// @brief WiFi SSID
            String ssid_;
            
            /// @brief WiFi SSID
            String password_;

            /// @brief WiFi server object
            WebServer* server_ = nullptr;

            /// @brief FTP server
            FtpServer ftpSrv_;

            /// @brief Serial object (used for write debug info to the serial port)
            HardwareSerial* serial_ = nullptr;

            /// @brief Platform angles
            std::vector<real> angles_;

            /// @brief Number of controllers (feilds for input values)
            int controllers_ = 0;
            
            /// @brief Indicate, that WiFi interface is running
            bool run_ = false;

            /// @brief Show web page
            /// @param path Path to the html web page in the local FS
            /// @return 
            bool handleFileRead(String path);

            /// @brief Set server behavior, if index.html doesn't exist
            void handleNotFound();

            /// @brief Set platform angle from web
            /// @details Get value from web interface and put it into angles_
            /// @param angle String angle identifer ("OX", "OY" or "OZ")
            /// @return Text message
            String setAngle(const String& angle);

            /// @brief Send platform angle to the web
            /// @details Get value from angles_ and send it to the web
            /// @param angle String angle identifer ("OX", "OY" or "OZ")
            /// @return String angle value
            String getAngle(const String& angle);

            /**
             * @brief Function setup web server behavior
             * @details Set server behavior on different requests 
             */
            void setupServer();
        public:
            /**
            * @brief WiFi web interface for limbs control
            * @param ssid: WiFi SSID
            * @param password: WiFi password
            * @param port: Port for WiFiServer object (80 by default)
            * @param serial: Pointer to the information serial port
            */
            WebInterface(
                const String& ssid,
                const String& password,
                int controllers = 3,
                int port = 80,
                HardwareSerial* serial = &Serial
            ) :
            ssid_(ssid),
            password_(password),
            controllers_(controllers),
            server_(new WebServer(port)),
            serial_(serial)
            {
                // stringVariables_ = std::vector<String>(controllers_, String(0));
                angles_ = std::vector<real>(controllers_, 0.0);
            };

            ~WebInterface()
            {
                delete server_;
            }

            /// @brief Begin WiFi and WiFi server
            /// @return true if success
            bool begin();

            /// @brief Handle web and ftp servers
            void handle();

            /// @brief Get platform angles, read from web
            /// @param[out] values: Values, get from web interface 
            void getPlatformAngles(std::vector<real>& values);
    };
}