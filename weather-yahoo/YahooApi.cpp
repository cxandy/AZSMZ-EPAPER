#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "YahooApi.h"

YahooApi::YahooApi(boolean _isMetric) {
  isMetric = _isMetric;
}

void YahooApi::setMetric(boolean isMetric) {
  this->isMetric = isMetric;
}

String urll = "http://query.yahooapis.com/v1/public/yql?q=select%20astronomy,item,location,atmosphere,wind%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D'";
String urlr = "')%20and%20u%3D'c'&format=json&diagnostics=true&callback=";

void YahooApi::doUpdate(YQLWeather *weather, String url) {
  this->weatherInfo = weather;
  JsonStreamingParser parser;
  parser.setListener(this);

  HTTPClient http;
  
  http.begin(url);
  bool isBody = false;
  char c;
  int size;
  Serial.println("[HTTP] GET: " + url);
  // start connection and send HTTP header
  int httpCode = http.GET();
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  if(httpCode > 0) {
    WiFiClient * client = http.getStreamPtr();
    while(client->connected()) {
      while((size = client->available()) > 0) {
        c = client->read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
        if (isBody) {
          parser.parse(c);
        }
      }
    }
  }
  this->weatherInfo = nullptr;
}

void YahooApi::updateWeather(YQLWeather *weather, String language, String city){
  doUpdate(weather,urll+city+urlr);
}

void YahooApi::whitespace(char c) {
  Serial.println("whitespace");
}

void YahooApi::startDocument() {
  Serial.println("start document");
}

void YahooApi::key(String key) {
  currentKey = String(key);
  if (currentKey == "forecast") {
    groupKey = currentKey;
    currentForecastPeriod = 0;
  }  

  if (currentKey == "condition") {
    groupKey = currentKey;
  }    

  if (currentKey == "astronomy") {
    groupKey = currentKey;
  }  

  if (currentKey == "location") {
    groupKey = currentKey;
  }   

  if (currentKey == "atmosphere") {
    groupKey = currentKey;
  }   
   
}

int maxForecasts = 10;

void YahooApi::value(String value) {
  if (currentKey == "description")  {
    weatherInfo->desc = value;
  }

  if (currentKey == "humidity" && groupKey =="atmosphere")  {
    weatherInfo->humidity = value;
  }

  if (currentKey == "pressure" && groupKey =="atmosphere")  {
    weatherInfo->pressure = value;
  }


  if (currentKey == "city" && groupKey =="location")  {
    weatherInfo->city = value;
  }

  if (currentKey == "country" && groupKey =="location")  {
    weatherInfo->country = value;
  }
  
  if (currentKey == "sunrise" && groupKey =="astronomy")  {
    weatherInfo->sunrise = value;
  }

  if (currentKey == "sunset" && groupKey =="astronomy")  {
    weatherInfo->sunset = value;
  }

  if (currentKey == "code" && groupKey =="condition")  {
    weatherInfo->conditionCode = value;
    Serial.println("code: " + value);
  }

  if (currentKey == "date" && groupKey =="condition")  {
    weatherInfo->conditionDate = value;
  }
  if (currentKey == "temp" && groupKey =="condition")  {
    weatherInfo->conditionTemp = value;
  }
  if (currentKey == "text" && groupKey =="condition")  {
    weatherInfo->conditionText = value;
  }


  if (currentKey == "code" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].code = value;
  }

  if (currentKey == "date" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].date = value;
  }

  if (currentKey == "day" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].day = value;
  }
  if (currentKey == "high" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].high = value;
  }

  if (currentKey == "low" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].low = value;
  }

  if (currentKey == "text" && groupKey =="forecast" && currentForecastPeriod < maxForecasts)  {
    weatherInfo->forecasts[currentForecastPeriod].text = value;
    currentForecastPeriod++;
  }

/*
  if (currentKey == "wind_mph" && !isMetric) {
    conditions->windSpeed = value + "mph";
  }

  if (currentKey == "wind_kph" && isMetric) {
    conditions->windSpeed = value + "km/h";
  }

   if (currentKey == "wind_dir") {
    conditions->windDir = value;
  }

   if (currentKey == "local_time_rfc822") {
    conditions->date = value.substring(0, 16);
  }

  if (currentKey == "observation_time_rfc822") {
    conditions->observationDate = value.substring(0, 16);
  }

  if (currentKey == "observation_time") {
    conditions->observationTime = value;
  }


  if (currentKey == "temp_f" && !isMetric) {
    conditions->currentTemp = value;
  }
  if (currentKey == "temp_c" && isMetric) {
    conditions->currentTemp = value;
  }
  if (currentKey == "icon") {
    conditions->weatherIcon = value;
  }
  if (currentKey == "weather") {
    conditions->weatherText = value;
  }
  if (currentKey == "relative_humidity") {
    conditions->humidity = value;
  }
  if (currentKey == "pressure_mb" && isMetric) {
    conditions->pressure = value + "mb";
  }
  if (currentKey == "pressure_in" && !isMetric) {
    conditions->pressure = value + "in";
  }

  if (currentKey == "feelslike_f" && !isMetric) {
    conditions->feelslike = value;
  }

  if (currentKey == "feelslike_c" && isMetric) {
    conditions->feelslike = value;
  }

  if (currentKey == "UV") {
    conditions->UV = value;
  }

  if (currentKey == "dewpoint_f" && !isMetric) {
    conditions->dewPoint = value;
  }
  if (currentKey == "dewpoint_c" && isMetric) {
    conditions->dewPoint = value;
  }
  if (currentKey == "precip_today_metric" && isMetric) {
    conditions->precipitationToday = value + "mm";
  }
  if (currentKey == "precip_today_in" && !isMetric) {
    conditions->precipitationToday = value + "in";
  }
*/
}

void YahooApi::endArray() {

}


void YahooApi::startObject() {
  currentParent = currentKey;
}

void YahooApi::endObject() {
  currentParent = "";
}

void YahooApi::endDocument() {

}

void YahooApi::startArray() {

}

String YahooApi::getMeteoconIcon(String iconText) {
  iconText.trim();
  Serial.println(iconText);
  if (iconText == "sunny") return "B";
  if (iconText == "clear") return "B";

  if (iconText == "31") return "B";   // 晴
  if (iconText == "32") return "B";
  if (iconText == "33") return "B";
  if (iconText == "34") return "B";

  
  if (iconText == "hazy") return "E";  // 雾
  if (iconText == "nt_hazy") return "E";

  if (iconText == "19") return "E";
  if (iconText == "20") return "E";
  if (iconText == "21") return "E";
  if (iconText == "22") return "E";
  
  if (iconText == "chanceflurries") return "F";
  if (iconText == "nt_chanceflurries") return "F";
  if (iconText == "flurries") return "F";
  if (iconText == "23") return "F";
  if (iconText == "24") return "F";
  
  if (iconText == "mostlysunny") return "H";
  if (iconText == "partlycloudy") return "H";

  if (iconText == "44") return "H";  

    if (iconText == "partlysunny") return "J";
  
  if (iconText == "fog") return "M";
  if (iconText == "nt_fog") return "M";
  
  if (iconText == "chancerain") return "Q";

  if (iconText == "8") return "Q";
  if (iconText == "9") return "Q";
  if (iconText == "10") return "Q";
  if (iconText == "11") return "Q";
  if (iconText == "12") return "Q";

  if (iconText == "45") return "Q";
  if (iconText == "46") return "Q";
  if (iconText == "47") return "Q";
  
  if (iconText == "rain") return "R";
  
  if (iconText == "chancetstorms") return "S";
  
  if (iconText == "chancesnow") return "V";
  if (iconText == "13") return "V";
  if (iconText == "14") return "V";
  
  if (iconText == "chancesleet") return "W";
  if (iconText == "snow") return "W";
  if (iconText == "sleet") return "W";
  if (iconText == "15") return "W";
  if (iconText == "16") return "W";
  if (iconText == "17") return "W";
  if (iconText == "18") return "W";
  
  if (iconText == "35") return "W";
  if (iconText == "41") return "W";
  if (iconText == "42") return "W";
  if (iconText == "43") return "W";
  
  if (iconText == "cloudy") return "Y";
  if (iconText == "mostlycloudy") return "Y";
  if (iconText == "nt_cloudy") return "Y";

  if (iconText == "26") return "Y";
  if (iconText == "27") return "Y";
  if (iconText == "28") return "Y";
  if (iconText == "29") return "Y";
  if (iconText == "30") return "Y";

  

  if (iconText == "tstorms") return "0";

  if (iconText == "0") return "0";
  if (iconText == "1") return "0";
  if (iconText == "2") return "0";
  if (iconText == "3") return "0";
  if (iconText == "4") return "0";

  if (iconText == "37") return "0";
  if (iconText == "38") return "0";
  if (iconText == "39") return "0";
   
  if (iconText == "nt_clear") return "2";
  if (iconText == "nt_mostlysunny") return "3";
  if (iconText == "nt_partlycloudy") return "4";
  if (iconText == "nt_partlysunny") return "4";
  if (iconText == "nt_sunny") return "4";
  if (iconText == "nt_mostlycloudy") return "5";
  if (iconText == "nt_chancerain") return "7";
  if (iconText == "40") return "7";
  
  
  if (iconText == "nt_rain") return "7";
  if (iconText == "nt_flurries") return "9";



  
  if (iconText == "nt_sleet") return "9";

  if (iconText == "nt_chancesleet") return "#";
  if (iconText == "nt_chancesnow") return "#";

  if (iconText == "5") return "#";
  if (iconText == "6") return "#";
  if (iconText == "7") return "#";
  
  if (iconText == "nt_chancetstorms") return "&";

  if (iconText == "nt_snow") return "#";
  if (iconText == "nt_tstorms") return "&";

  return ")";
}
