#include <config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

params_type GLOBALS;
void read_config() {
  const int PARAMS_MAX_LEN = 65535;
  static char key_room[PARAMS_MAX_LEN];
  static char val_room[PARAMS_MAX_LEN];
  char *key = key_room, *val = val_room;
  FILE *fd = fopen("config", "r");
  while (fscanf(fd, "%s %s", key, val) != EOF) {
    GLOBALS[key] = val;
    key += strlen(key) + 1;
    val += strlen(val) + 1;
  }
  fclose(fd);
}
