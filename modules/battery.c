#include "../defs.h"
#include "../util.h"
#include <string.h>

void get_battery_state(char *buffer) {
  char *bs[] = {"", "", "", "", ""};
  int mx[] = {90, 60, 40, 20, 10};
  int capacity;
  char status[12];
  pscanf("/sys/class/power_supply/BAT0/capacity", "%d", &capacity);
  pscanf("/sys/class/power_supply/BAT0/status", "%12[a-zA-Z ]", &status);

  char *r;
  char *s;

  for (int j = 0; j < 5; j++) {
    if (capacity > mx[j]) {
      r = bs[j];
      break;
    }
  }

  if (strncmp(status, "Discharging", 12) == 0)
    s = ""; /* "🔋"; */
  else
    s = "";

  /* char *s = "🔌 🔋   JIKONIK Hola mundo mundial!"; */
  snprintf(buffer, MAX_COMPONENT_BUFFER, "%s %s %d%%", s, r, capacity);
}
