#include "Endpoints.h"
#include "HVAC.h"

void setupEndpoints() {
  server.on("/", handleRoot);
  server.on("/cool", HTTP_POST, handleCool);
  server.on("/heat", HTTP_POST, handleHeat);
  server.on("/off", HTTP_POST, handleOff);
  server.on("/setpoint", HTTP_POST, handleSetpoint);
  server.on("/hysteresis", HTTP_POST, handleHysteresis);
  server.on("/statechangedelay", HTTP_POST, handleStateChangeDelay);
  server.onNotFound(handleNotFound);
  server.begin();
}

void handleRoot() {
  String message = "<html>\n";
  message += "<head>\n";
  message += "<title>Thermostat</title>\n";
  message += "<style>\n";
  message += "body { font-size: 48px; }\n";
  message += ".button { border-radius:20px; padding:15px; min-height:80px; min-width: 120px; font-size: 48px; }\n";
  message += ".cool_action { background-color:#4488ff; font-size: 72px; }\n";
  message += ".heat_action { background-color:#ff8800; font-size: 72px; }\n";
  message += ".input_text { font-size: 48px; }\n";
  message += "</style>\n";
  message += "</head>\n";
  message += "<body>\n";
  message += "<h1>Thermostat</h1>\n";
  message += "<h2>"; message += getTemperature(); message += "&#8457; &rarr; "; message += temperature_target; message += "&plusmn;"; message += temperature_hysteresis; message += "&#8457;</h2>\n";
  message += "<h2>"; message += getControlDescription(); message += "</h2>\n";
  if (offActionAvailable()) {
    message += "<form action=\"off\" method=\"post\">\n";
    message += "<input type=\"submit\" class=\"button off_action\" value=\"Off\">\n";
    message += "</form>\n";
  }
  if (coolActionAvailable()) {
    message += "<form action=\"cool\" method=\"post\">\n";
    message += "<input type=\"submit\" class=\"button cool_action\" value=\"Cool\">\n";
    message += "</form>\n";
  }
  if (heatActionAvailable()) {
    message += "<form action=\"heat\" method=\"post\">\n";
    message += "<input type=\"submit\" class=\"button heat_action\" value=\"Heat\">\n";
    message += "</form>\n";
  }
  message += "<form action=\"setpoint\" method=\"post\">\n";
  message += "<input class=\"input_text\" name=\"setpoint\" size=\"5\" value=\""; message += temperature_target; message += "\">&#8457;\n";
  message += "<input type=\"submit\" class=\"button setpoint_action\" value=\"Set target temperature\">\n";
  message += "</form>\n";
  message += "<form action=\"hysteresis\" method=\"post\">\n";
  message += "<input class=\"input_text\" name=\"hysteresis\" size=\"5\" value=\""; message += temperature_hysteresis; message += "\">&#8457;\n";
  message += "<input type=\"submit\" class=\"button hysteresis_action\" value=\"Set temperature accuracy\">\n";
  message += "</form>\n";
  message += "<form action=\"statechangedelay\" method=\"post\">\n";
  float current_delay = (float)state_change_delay / 60.0f / 1000.0f;
  message += "<input class=\"input_text\" name=\"delay\" size=\"5\" value=\""; message += current_delay; message += "\"> minutes\n";
  message += "<input type=\"submit\" class=\"button statechangedelay_action\" value=\"Set compressor delay\">\n";
  message += "</form>\n";
  message += "</body>\n";
  message += "</html>";
  server.send(200, "text/html", message);
}

void handleCool() {
  setUserIntent(HVAC_COOL);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "Cooling mode set.");
}

void handleHeat() {
  setUserIntent(HVAC_HEAT);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "Heating mode set.");
}

void handleOff() {
  setUserIntent(HVAC_OFF);
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "Off mode set.");
}

void handleSetpoint() {
  if (!server.hasArg("setpoint")) {
    server.send(400, "text/plain", "Missing setpoint form data");
    return;
  }
  float new_setpoint = server.arg("setpoint").toFloat();
  if (new_setpoint == 0.0f) {
    server.send(400, "text/plain", "Invalid setpoint (must be a floating point value of degrees F)");
    return;
  }
  temperature_target = new_setpoint;
  String message = "Temperature target set to "; message += temperature_target; message += " F.";
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", message);
}

void handleHysteresis() {
  if (!server.hasArg("hysteresis")) {
    server.send(400, "text/plain", "Missing hysteresis form data");
    return;
  }
  float new_hysteresis = server.arg("hysteresis").toFloat();
  if (new_hysteresis == 0.0f) {
    server.send(400, "text/plain", "Invalid hysteresis (must be a floating point value of degrees F)");
    return;
  }
  temperature_hysteresis = new_hysteresis;
  String message = "Temperature hysteresis set to "; message += temperature_hysteresis; message += " F.";
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", message);
}

void handleStateChangeDelay() {
  if (!server.hasArg("delay")) {
    server.send(400, "text/plain", "Missing delay form data");
    return;
  }
  float new_statechangedelay = server.arg("delay").toFloat();
  if (new_statechangedelay == 0.0f) {
    server.send(400, "text/plain", "Invalid delay (must be a floating point value of minutes)");
    return;
  }
  state_change_delay = (unsigned long)(new_statechangedelay * 60 * 1000);
  String message = "State change delay set to "; message += state_change_delay; message += " ms.";
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
