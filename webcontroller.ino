/*
  ESP32 Remote WiFi Servo Control
  esp32-web-servo.ino
  Control servo motor from web page

  Based upon example from Rui Santos
  Random Nerd Tutorials
  https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/


  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/

#include <WiFi.h>
#include <ESP32Servo.h>
#include <list>
#include "motorcontroller.h"

/*
    Web Server Variables
*/
// Network credentials
const char *ssid = "Garrett";
const char *password = "3042872824";

// Web server on port 80 (http)
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(0);
int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

/*
    Robot Arm Variables
*/

// Motor Controller
MotorController motorcontroller;

// Vars to save angles from web server
int baseStepperAngle = 0;
int shoulderServoAngle = 0;
int elbowServoAngle = 0;
int wristPitchServoAngle = 0;
int wristRollServoAngle = 0;
int clawServoAngle = 0;

// Angle list
std::list<int> angles;

// angle, direction pin, step pin, resolution
std::list<int> baseStepper = {0, 12, 14, 200}; 

// angle, max angle, control pin, min, max, hertz
std::list<int> shoulderServo = {0, 270, 18, 500, 2500, 50};
std::list<int> elbowServo = {0, 270, 5, 500, 2500, 100};
std::list<int> wristPitchServo = {0, 180, 21, 500, 2500, 150}; 
std::list<int> clawServo = {0, 50, 19, 500, 2500, 200}; 

void setup()
{
    // Allow allocation of all timers for servo library
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // Start serial
    Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

    /*
        Set up controller
    */
    motorcontroller = MotorController(90, 0.03f);
    motorcontroller.setup_servos({shoulderServo, elbowServo, wristPitchServo, clawServo});
    motorcontroller.setup_stepper(baseStepper);
}

void loop()
{
    listen_for_clients();
}

void listen_for_clients() {
    // Listen for incoming clients
    WiFiClient client = server.available();

    // Client Connected
    if (client)
    {
        // Set timer references
        currentTime = millis();
        previousTime = currentTime;

        // Print to serial port
        Serial.println("New Client.");

        // String to hold data from client
        String currentLine = "";

        // Do while client is cponnected
        while (client.connected() && currentTime - previousTime <= timeoutTime)
        {
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("    <link rel=\"icon\" href=\"data:,\">");
                        client.println("    <style>");
                        client.println("        body {");
                        client.println("            text-align: center;");
                        client.println("            font-family: \"Trebuchet MS\", Arial;");
                        client.println("            margin-left: auto;");
                        client.println("            margin-right: auto;");
                        client.println("        .slider {");
                        client.println("            -webkit-appearance: none;");
                        client.println("            width: 300px;");
                        client.println("            height: 25px;");
                        client.println("            border-radius: 10px;");
                        client.println("            background: #dddddd;");
                        client.println("            outline: none;");
                        client.println("            opacity: 0.7;");
                        client.println("            -webkit-transition: .2s;");
                        client.println("            transition: opacity .2s;");
                        client.println("        .slider::-webkit-slider-thumb {");
                        client.println("            -webkit-appearance: none;");
                        client.println("            appearance: none;");
                        client.println("            width: 35px;");
                        client.println("            height: 35px;");
                        client.println("            border-radius: 50%;");
                        client.println("            background: #aaaaaa;");
                        client.println("            cursor: pointer;");
                        client.println("        }");
                        client.println("    </style>");
                        client.println("    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                        client.println("</head>");
                        client.println("<body style=\"background-color:#223c4a;\">");
                        client.println("    <h1 style=\"color:#dddddd;\">Arm Control</h1>");
                        client.println("    <div id=\"baseStepper\">");
                        client.println("        <h2 style=\"color:#dddddd;\">Base Stepper Position: <span id=\"baseStepperPos\">0</span>°</h2>");
                        client.println("        <input type=\"range\" min=\"0\" max=\"360\" class=\"slider\" id=\"baseStepperSlider\" value=\"0\">");
                        client.println("    </div>");
                        client.println("    <div id=\"shoulderServo\">");
                        client.println("        <h2 style=\"color:#dddddd;\">Shoulder Servo Position: <span id=\"shoulderServoPos\">0</span>°</h2>");
                        client.println("        <input type=\"range\" min=\"0\" max=\"270\" class=\"slider\" id=\"shoulderServoSlider\" value=\"0\">");
                        client.println("    </div>");
                        client.println("    <div id=\"elbowServo\">");
                        client.println("        <h2 style=\"color:#dddddd;\">Elbow Servo Position: <span id=\"elbowServoPos\">0</span>°</h2>");
                        client.println("        <input type=\"range\" min=\"0\" max=\"270\" class=\"slider\" id=\"elbowServoSlider\" value=\"0\">");
                        client.println("    </div>");
                        client.println("    <div id=\"wristPitchServo\">");
                        client.println("        <h2 style=\"color:#dddddd;\">Wrist Pitch Servo Position: <span id=\"wristPitchServoPos\">0</span>°</h2>");
                        client.println("        <input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"wristPitchServoSlider\" value=\"0\">");
                        client.println("    </div>");
                        client.println("    <div id=\"clawServo\">");
                        client.println("        <h2 style=\"color:#dddddd;\">Claw Servo Position: <span id=\"clawServoPos\">0</span>°</h2>");
                        client.println("        <input type=\"range\" min=\"0\" max=\"50\" class=\"slider\" id=\"clawServoSlider\" value=\"0\">");
                        client.println("    </div>");
                        client.println("    <button id=\"moveButton\" onclick=\"move(baseStepperSlider.value, shoulderServoSlider.value, elbowServoSlider.value, wristPitchServoSlider.value, clawServoSlider.value)\">");
                        client.println("        Move to Position");
                        client.println("    </button>");
                        client.println("    <script>");
                        client.println("        var baseStepperSlider = document.getElementById(\"baseStepperSlider\");");
                        client.println("        var shoulderServoSlider = document.getElementById(\"shoulderServoSlider\");");
                        client.println("        var elbowServoSlider = document.getElementById(\"elbowServoSlider\");");
                        client.println("        var wristPitchServoSlider = document.getElementById(\"wristPitchServoSlider\");");
                        client.println("        var clawServoSlider = document.getElementById(\"clawServoSlider\");");

                        client.println("        var baseStepperPos = document.getElementById(\"baseStepperPos\"); baseStepperPos.innerHTML = baseStepperSlider.value;");
                        client.println("        var shoulderServoPos = document.getElementById(\"shoulderServoPos\"); shoulderServoPos.innerHTML = shoulderServoSlider.value;");
                        client.println("        var elbowServoPos = document.getElementById(\"elbowServoPos\"); elbowServoPos.innerHTML = elbowServoSlider.value;");
                        client.println("        var wristPitchServoPos = document.getElementById(\"wristPitchServoPos\"); wristPitchServoPos.innerHTML = wristPitchServoSlider.value;");
                        client.println("        var clawServoPos = document.getElementById(\"clawServoPos\"); clawServoPos.innerHTML = clawServoSlider.value;");

                        client.println("        baseStepperSlider.oninput = function () { baseStepperSlider.value = this.value; baseStepperPos.innerHTML = this.value; }");
                        client.println("        shoulderServoSlider.oninput = function () { shoulderServoSlider.value = this.value; shoulderServoPos.innerHTML = this.value; }");
                        client.println("        elbowServoSlider.oninput = function () { elbowServoSlider.value = this.value; elbowServoPos.innerHTML = this.value; }");
                        client.println("        wristPitchServoSlider.oninput = function () { wristPitchServoSlider.value = this.value; wristPitchServoPos.innerHTML = this.value; }");
                        client.println("        clawServoSlider.oninput = function () { clawServoSlider.value = this.value; clawServoPos.innerHTML = this.value; }");
                        
                        client.println("        $.ajaxSetup({ timeout: 1000 }); ");
                        client.println("        function move(baseStepperPos, shoulderServoPos, elbowServoPos, wristPitchServoPos, clawServoPos) {");
                        //client.println("            $.get(\"/?baseStepperValue=\" + baseStepperPos + \"&shoulderServoValue=\" + shoulderServoPos + \"&elbowServoValue=\" + elbowServoPos + \"&\");");
                        client.println("            $.get(\"/?baseStepperValue=\" + baseStepperPos + \"&shoulderServoValue=\" + shoulderServoPos + \"&elbowServoValue=\" + elbowServoPos + \"&wristPitchServoValue=\" + wristPitchServoPos + \"&clawServoValue=\" + clawServoPos + \"&\");");
                        client.println("            { Connection: close };");
                        client.println("        }");
                        client.println("        </script>");
                        client.println("</body>");
                        client.println("</html>");

                        Serial.println(String(header));
                        // GET data
                        if(header.indexOf("baseStepperValue=") >= 0) {
                            pos1 = header.indexOf('baseStepperValue=');
                            pos2 = header.indexOf('&');

                            // String with motor position
                            valueString = header.substring(pos1 + 1, pos2);

                            baseStepperAngle = valueString.toInt();
                            angles.push_back(baseStepperAngle);

                            Serial.println("Base angle: " + String(baseStepperAngle));       
                        } 

                        // GET data
                        if(header.indexOf("shoulderServoValue=") >= 0) {
                            pos1 = header.indexOf('shoulderServoValue=', pos1 + 1);
                            pos2 = header.indexOf('&', pos2 + 1);

                            // String with motor position
                            valueString = header.substring(pos1 + 1, pos2);

                            shoulderServoAngle = map_to_normal(valueString.toInt(), 0, 270, 180);
                            angles.push_back(shoulderServoAngle);

                            Serial.println("Shoulder angle: " + String(shoulderServoAngle));       
                        }

                        // GET data
                        if(header.indexOf("elbowServoValue=") >= 0) {
                            pos1 = header.indexOf('elbowServoValue=', pos1 + 1);
                            pos2 = header.indexOf('&', pos2 + 1);

                            // String with motor position
                            valueString = header.substring(pos1 + 1, pos2);

                            elbowServoAngle = map_to_normal(valueString.toInt(), 0, 270, 180);
                            
                            angles.push_back(elbowServoAngle);

                            Serial.println("Elbow angle: " + String(elbowServoAngle));       
                        }

                        // GET data
                        if(header.indexOf("wristPitchServoValue=") >= 0) {
                            pos1 = header.indexOf('wristPitchServoValue=', pos1 + 1);
                            pos2 = header.indexOf('&', pos2 + 1);

                            // String with motor position
                            valueString = header.substring(pos1 + 1, pos2);

                            wristPitchServoAngle = map_to_normal(valueString.toInt(), 0, 180, 180);
                            
                            angles.push_back(wristPitchServoAngle);

                            Serial.println("Wrist pitch angle: " + String(wristPitchServoAngle));       
                        }

                        // GET data
                        if(header.indexOf("clawServoValue=") >= 0) {
                            pos1 = header.indexOf('clawServoValue=', pos1 + 1);
                            pos2 = header.indexOf('&', pos2 + 1);

                            // String with motor position
                            valueString = header.substring(pos1 + 1, pos2);

                            clawServoAngle = map_to_normal(valueString.toInt(), 0, 50, 50);
                            
                            angles.push_back(clawServoAngle);

                            Serial.println("Claw: " + String(clawServoAngle));       
                        }    

                        if (angles.size() > 0) {
                            // Move robot arm
                            Serial.println("Angles size: " + String(angles.size()));
                            motorcontroller.move_motors_smooth(angles);
                            angles.clear();
                        }

                        // The HTTP response ends with another blank line
                        client.println();
                        
                        // Break out of the while loop
                        break;
                    }
                    else
                    {
                        // New lline is received, clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}