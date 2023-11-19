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

void robo::WebInterface::setupServer()
{
  server_->onNotFound([this]() {
    if (!handleFileRead(server_->uri()))
      handleNotFound();
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

void robo::WebInterface::getValues(std::vector<int> &values)
{
    values = intVariables_;
}

void robo::WebInterface::setControllersNum(const int n)
{
    controllers_ = n;
}
