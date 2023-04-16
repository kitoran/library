#ifndef SETTINGS_H
#define SETTINGS_H
#define MAX_STRING_LEN 100
int loadInt(char* name, _Bool *success);
void saveInt(char* name, int value);

double loadDouble(char *name, _Bool* success);
void saveDouble(char *name, double value);

#define SAVE_INT(v) saveInt(#v, v);
#define LOAD_INT(v, s) v=loadInt(#v, s);

#endif // SETTINGS_H
