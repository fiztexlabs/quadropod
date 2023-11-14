#include <web_interface.h>

using namespace robo;

void robo::WebInterface::updatePage(WiFiClient &client)
{
    // make a String to hold incoming data from the client
    String currentLine = "";
    
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header_ += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

            // Web Page
            client.println("</head><body><h1>Controls interface</h1>");

            for(size_t i = 0; i < controllers_; ++i)
            {
                client.println("<p>Position: <span id=\"servoPos"+String(i)+"\"></span></p>");
                client.println("<input type=\"range\" min=\"-180\" max=\"180\" class=\"slider\" id=\"servoSlider"+String(i)+"\" onchange=\"servo(this.value"+String(i)+")\" value"+String(i)+"=\"" + stringVariables_.at(i) + "\"/>");

                client.println("<script>var slider = document.getElementById(\"servoSlider"+String(i)+"\");");
                client.println("var servoP"+String(i)+" = document.getElementById(\"servoPos"+String(i)+"\"); servoP"+String(i)+".innerHTML = slider.value"+String(i)+";");
                client.println("slider.oninput = function() { slider.value"+String(i)+" = this.value"+String(i)+"; servoP"+String(i)+".innerHTML = this.value"+String(i)+"; }");
                client.println("$.ajaxSetup({timeout:1000}); function servo(pos"+String(i)+") { ");
                client.println("$.get(\"/?value"+String(i)+"=\" + pos"+String(i)+" + \"&\"); {Connection: close};}</script>");

            }

            client.println("</body></html>");

            // GET /?value=180& HTTP/1.1
            if(serial_!=nullptr)
            {
                serial_->println("Read input values:");
            }
            for(size_t i = 0; i < controllers_; ++i)
            {
                if (header_.indexOf("GET /?value=") >= 0)
                {
                    pos1_ = header_.indexOf('=');
                    pos2_ = header_.indexOf('&');
                    stringVariables_.at(i) = header_.substring(pos1_ + 1, pos2_);
                    intVariables_.at(i) = stringVariables_.at(i).toInt();

                    if(serial_!=nullptr)
                    {
                        serial_->println(stringVariables_.at(i));
                    }
                }
            }

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }

    // clear header
    header_ = "";
}

bool robo::WebInterface::begin()
{
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
            if (WiFi.status() != WL_CONNECTED)
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

        server_->begin();

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

bool robo::WebInterface::listen()
{
    WiFiClient client = server_->available();

    if (client)
    {
        if(serial_!=nullptr)
        {
            serial_->println("client connected");
        }

        updatePage(client);
        client.stop();

        
        if(serial_!=nullptr)
        {
            serial_->println("client disconnected");
            serial_->println("");
        }

        return true;
    }
    else
    {
        return false;
    }
}

void robo::WebInterface::getValues(std::vector<int> &values)
{
    values = intVariables_;
}

void robo::WebInterface::setControllersNum(const int n)
{
    controllers_ = n;
}
