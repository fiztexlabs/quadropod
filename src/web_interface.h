#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <vector>

namespace robo
{
    class WebInterface
    {
        private:
            /// @brief WiFi SSID
            String ssid_;
            
            /// @brief WiFi SSID
            String password_;

            /// @brief Variable to store the HTTP request
            String header_;

            /// @brief WiFi server object
            WiFiServer* server_ = nullptr;

            /// @brief Serial object (used for write debug info to the serial port)
            HardwareSerial* serial_ = nullptr;

            /// @brief Vector of string variables, read from controllers
            std::vector<String> stringVariables_;

            /// @brief Vector of integer variables, read from controllers
            std::vector<int> intVariables_;

            /// @brief Number of controllers (feilds for input values)
            int controllers_ = 0;

            int pos1_ = 0;

            int pos2_ = 0;
            
            /// @brief Indicate, that WiFi interface is running
            bool run_ = false;

            void updatePage(WiFiClient& client);
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
                int controllers = 1,
                int port = 80,
                HardwareSerial* serial = nullptr
            ) :
            ssid_(ssid),
            password_(password),
            controllers_(controllers),
            server_(new WiFiServer(port)),
            serial_(serial)
            {
                stringVariables_ = std::vector<String>(controllers_, String(0));
                intVariables_ = std::vector<int>(controllers_, 0);
            };

            ~WebInterface()
            {
                delete server_;
            }

            /// @brief Begin WiFi and WiFi server
            /// @return true if success
            bool begin();

            /// @brief Listen for incoming clients
            /// @return true if success
            bool listen();

            /// @brief Get values, read from web
            /// @param[out] values: Values, get from web interface 
            void getValues(std::vector<int>& values);

            /// @brief Set number of controllers
            /// @param n: Controllers number
            void setControllersNum(const int n);
    };
}