#ifndef SETTINGS_H
#define SETTINGS_H
#define MAX_STRING_LEN 100
int loadInt(char* name, _Bool *success);
void saveInt(char* name, int value);

int loadDouble(char *name, _Bool* success);
void saveDouble(char *name, double value);

#define SAVE_INT(v) saveInt(#v, v);

#endif // SETTINGS_H
