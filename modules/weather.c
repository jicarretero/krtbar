#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Icons @ https://openweathermap.org/weather-conditions */
/* https://www.alt-codes.net/weather-symbols.php */
#define ICLOUD ""

#define I1D "" /* 01d, 01n */
#define I1N "" /* 01d, 01n */
#define I2 "🌤"    /* 02d, 03d, 04d */
#define I4 ""
#define I9 "🌦"  /* 09d, 09n, 10d, 10n */
#define I10 "🌨" /* 13d, 13n */
#define I11 "⛈"
#define I13 ""
#define I50 "🌫" /* 50d, 50n */
#define I0 " "

const char *filename = "/home/jicg/.cache/krt-weather.json";

char *get_icon(const char *icon) {
  char d = icon[2];
  int n = atoi(icon);

  switch (n) {
  case 1:
    return d == 'd' ? I1D : I1N;
    break;
  case 2:
  case 3:
    return I2;
    break;
  case 4:
    return I4;
    break;
  case 9:
    return I9;
    break;
  case 10:
    return I10;
    break;
  case 11:
    return I11;
    break;
  case 13:
    return I13;
    break;
  case 50:
    return I50;
    break;
  }

  return I0;
}

void get_weather(char *output) {
  int max_size = 2048;
  struct stat st;
  stat(filename, &st);

  char c[2048];
  char *city;
  char *descr;
  float celsius;
  char *sicon;

  FILE *f = fopen(filename, "r");
  fread(c, 2048, 1, f);
  fclose(f);

  cJSON *json = cJSON_Parse(c);

  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
      fprintf(stderr, "Error before: %s\n", error_ptr);
    exit(EXIT_FAILURE);
  }

  cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
  if (cJSON_IsString(name) && (name->valuestring != NULL)) {
    // printf("Name: %s\n", name->valuestring);
    city = name->valuestring;
  }

  cJSON *weather_array = cJSON_GetObjectItemCaseSensitive(json, "weather");
  if (cJSON_IsArray(weather_array)) {
    // printf("Array len: %d\n", cJSON_GetArraySize(weather_array));

    cJSON *first = cJSON_GetArrayItem(weather_array, 0);

    cJSON *description = cJSON_GetObjectItemCaseSensitive(first, "description");
    if (cJSON_IsString(description) && (description->valuestring != NULL)) {
      // printf("Description: %s\n", description->valuestring);
      descr = description->valuestring;
    }
    cJSON *icon = cJSON_GetObjectItemCaseSensitive(first, "icon");
    if (cJSON_IsString(icon) && (icon->valuestring != NULL)) {
      // printf("icon: %s\n", icon->valuestring);
      sicon = icon->valuestring;
    }
  }

  cJSON *main_object = cJSON_GetObjectItemCaseSensitive(json, "main");
  if (cJSON_IsObject(main_object)) {
    cJSON *temp = cJSON_GetObjectItemCaseSensitive(main_object, "temp");
    if (cJSON_IsNumber(temp)) {
      // printf("Temp: %03.2f\n", temp->valuedouble - 273.15);
      celsius = temp->valuedouble - 273.15;
    }
  }

  snprintf(output, max_size, "%s  %s, %s %03.2fºC", get_icon(sicon), city,
           descr, celsius);
  cJSON_Delete(json);
}
