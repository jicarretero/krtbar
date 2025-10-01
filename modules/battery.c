#include "../defs.h"
#include "../util.h"
#include <string.h>
#include <unistd.h>

int low_notified = 0;
int high_notified = 0;

void get_battery_state(char *buffer) {
  char *bs[] = {"", "", "", "", ""};
  int mx[] = {90, 60, 40, 20, 0};
  int capacity;
  char status[12];
  if (access("/sys/class/power_supply/BAT0/capacity", F_OK) == 0) {
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

    if (strncmp(status, "Discharging", 12) == 0) {
      if (capacity <= 20 && !low_notified) {
        exec_command("notify-send -u critical \"Battery Too Low!\"", buffer);
        low_notified = 1;
      }
      s = ""; /* "🔋"; */
    } else {
      low_notified = 0;
      s = "";
    }

    if (strncmp(status, "Charging", 9) == 0) {
      if (capacity >= 80 && !high_notified) {
        exec_command("notify-send -u critical \"Battery load above 80%!\"", buffer);
        high_notified = 1;
      }
      s = "";
    } else {
      high_notified = 0;
      s = ""; /* "🔋"; */
    }

    /* char *s = "🔌 🔋   JIKONIK Hola mundo mundial!"; */
    snprintf(buffer, MAX_COMPONENT_BUFFER, "%s %s  %d%%", s, r, capacity);
  } else {
    snprintf(buffer, MAX_COMPONENT_BUFFER, " 🔌 - unknown");
  }
}
