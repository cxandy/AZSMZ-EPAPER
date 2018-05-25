#pragma once

#include <JsonListener.h>
#include <JsonStreamingParser.h>

typedef struct YQLForecast {
  String code;
  String date;
  String day;
  String high;
  String low;
  String text;
} YQLForecast;

typedef struct YQLWeather {
  String city;
  String country;
  String conditionCode;
  String conditionDate;
  String conditionTemp;
  String conditionText;
  String sunrise;
  String sunset;
  String humidity;
  String pressure;
  String desc;
  YQLForecast forecasts[10];   
} YQLWeather;

class YahooApi: public JsonListener {
  private:
    boolean isMetric = true;
    String currentKey;
    String currentParent;
//    WGConditions *conditions;
    YQLWeather *weatherInfo;
//    YQLForecast *forecasts;    


//  void doUpdate(WGConditions *conditions, String url);
    void doUpdate(YQLWeather *weather, String url);
    bool isForecast = false;
    String groupKey;
    int currentForecastPeriod;

  public:
    YahooApi(boolean isMetric);

/*
    void updateConditions(WGConditions *conditions, String apiKey, String language, String country, String city);
    void updateConditions(WGConditions *conditions, String apiKey, String language, String zmwCode);
    void updateConditionsPWS(WGConditions *conditions, String apiKey, String language, String pws);
  */  
    void setMetric(boolean isMetric);
    String getMeteoconIcon(String icon);

    void updateWeather(YQLWeather *weather, String language, String city);
    
    virtual void whitespace(char c);

    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
};
