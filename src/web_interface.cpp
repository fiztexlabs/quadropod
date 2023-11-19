#include <web_interface.h>

using namespace robo;

bool robo::WebInterface::handleFileRead(String path)
{
    if (path.endsWith("/")) 
      path += "index.html";

    if (SPIFFS.exists(path))
    {
      File file = SPIFFS.open(path, "r");
      size_t sent = server_->streamFile(file, "text/html");
      file.close();
      return true;
    }
    return false;
}

void robo::WebInterface::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server_->uri();
  message += "\nMethod: ";
  message += (server_->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server_->args();
  message += "\n";

  for (uint8_t i = 0; i < server_->args(); i++) {
    message += " " + server_->argName(i) + ": " + server_->arg(i) + "\n";
  }

  server_->send(404, "text/plain", message);
}

String robo::WebInterface::setAngle(const String &angle)
{
    real val = -999.0;

    if(angle == "OX")
    {
        angles_.at(0) = static_cast<real>(server_->arg("a_ox").toDouble());
        val = angles_.at(0);
    }
    if(angle == "OY")
    {
        angles_.at(1) = static_cast<real>(server_->arg("a_oy").toDouble());
        val = angles_.at(1);
    }
    if(angle == "OZ")
    {
        angles_.at(2) = static_cast<real>(server_->arg("a_oz").toDouble());
        val = angles_.at(2);
    }

    return String(angle+" angle set to "+String(val)+" deg");
}

String robo::WebInterface::getAngle(const String &angle)
{
    real val = -999.0;

    if(angle == "OX")
    {
        val = angles_.at(0);
    }
    if(angle == "OY")
    {
        val = angles_.at(1);
    }
    if(angle == "OZ")
    {
        val = angles_.at(2);
    }

    return String(String(val));
}

void robo::WebInterface::setupServer()
{
  server_->onNotFound([this]() {
    if (!handleFileRead(server_->uri()))
      handleNotFound();
  });

  server_->on("/set_a_ox", [this]() {
    server_->send(200, "text/plain", setAngle("OX"));
  });

  server_->on("/get_a_ox", [this]() {
    server_->send(200, "text/plain", getAngle("OX"));
  });

  server_->on("/set_a_oy", [this]() {
    server_->send(200, "text/plain", setAngle("OY"));
  });

  server_->on("/get_a_oy", [this]() {
    server_->send(200, "text/plain", getAngle("OY"));
  });

  server_->on("/set_a_oz", [this]() {
    server_->send(200, "text/plain", setAngle("OZ"));
  });

  server_->on("/get_a_oz", [this]() {
    server_->send(200, "text/plain", getAngle("OZ"));
  });
}

bool robo::WebInterface::begin()
{
    // if interface not running, start web interface (connect to WiFi, begin SPIFFS, FTP server and web server)
    if (!run_)
    {
        if(WiFi.status() != WL_CONNECTED)
        {
            if (serial_!=nullptr)
            {
                serial_->print("Connecting to ");
                serial_->println(ssid_);
            }

            WiFi.begin(ssid_, password_);

            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);

                if (serial_!=nullptr)
                {
                  serial_->print(".");
                }
            }
            if (WiFi.status() == WL_CONNECTED)
            {
                if (serial_!=nullptr)
                {
                  serial_->println("");
                  serial_->println("WiFi connected.");
                  serial_->println("IP address: ");
                  serial_->println(WiFi.localIP());
                }
            }
        }
        else
        {
            if (serial_!=nullptr)
            {
                serial_->println("WiFi already connected");
            }
        }

        if (SPIFFS.begin(true)) 
        {
            if (serial_!=nullptr)
            {
                serial_->println("SPIFFS opened");
            }
            ftpSrv_.begin("admin","admin");
            if (serial_!=nullptr)
            {
                serial_->println("FTP server started");
            }
        }

        setupServer();
        server_->begin();
        if (serial_!=nullptr)
        {
            serial_->println("WEB server started");
        }

        run_ = true;
    }
    else
    {
        if (serial_!=nullptr)
        {
            serial_->println("Interface already run");
        } 
    }

    return true;
}

void robo::WebInterface::handle()
{
    server_->handleClient();
    delay(2);
    ftpSrv_.handleFTP();
}

void robo::WebInterface::getPlatformAngles(std::vector<real> &values)
{
    values = angles_;
}
