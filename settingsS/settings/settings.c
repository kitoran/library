﻿#include "settings.h"
#include "getline.h"
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include "misc.h"
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
//#include <libconfig.h>
#include <stdlib.h>
#include "stb_ds.h"
#ifdef WIN32
#include "windows.h"
#endif
extern const char* appName;
#ifdef _MSC_VER
#define PATH_MAX _MAX_PATH
#endif
static char path[PATH_MAX] = {0};
static char path_back[PATH_MAX+1] = {0};
char* fileContent;

static struct Pair {
    char* key;
    char* value;
} * hmap = NULL;
typedef struct Pair Pair;
#ifndef _MSC_VER
#include <unistd.h>
#include <linux/limits.h>
static void init() {
    static bool initialized = false;
    if(initialized) {
        return;
    }
    //return;
    char* home = getenv("HOME");
    assert(home);
// this is a wrong use of strncat; i do care but not idk
#warning strncat doesnt check overflow
    strncat(path, home, PATH_MAX-1);
    strncat(path, "/.config/", PATH_MAX-1);
    struct stat sb;
    if(stat(path, &sb) != 0 || (! S_ISDIR(sb.st_mode))) {
        abort();
    }
    strncat(path, appName, PATH_MAX-1);
    if(stat(path, &sb) != 0 || (! S_ISDIR(sb.st_mode))) {
        mkdir(path, 0777);
    }
    strncat(path, "/", PATH_MAX-1);
    strncat(path, appName, PATH_MAX-1);
    strncat(path, ".conf", PATH_MAX-1);
    snprintf(path_back, PATH_MAX+1,
             "%s~", path);
//    int fd
    stat(path, &sb);
    if(stat(path, &sb) != 0) {
        goto leave;
//        int fd = open(path, O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
//        close(fd);
    }
    fileContent = malloc(sb.st_size);
    FILE* f = fopen(path, "r");
    char* line;
    size_t len = 0;
    while(getline(&line, &len, f) > 0) {
        char* key; char* value;
        if(sscanf(line, " %ms = %ms ", &key, &value) > 0) {
            shput(hmap, key, value);
        }
    }
    fclose(f);

//    int index = 0;
//    while(index < sb.st_size) {

//    }


//    const char *str;

//    config_init(&cfg);
//    setting = config_root_setting(&cfg);

//    if(! config_read_file(&cfg, path))
//    {
//        fprintf(stderr, "Error while reading settings file.\n");
//        config_destroy(&cfg);
//        abort();
//    }
    leave:
    initialized = true;
}
#else
static void init() {
    static bool initialized = false;
    if(initialized) {
        return;
    }
    char p[]="settingsSettingsGuiSettings.ini";
    strncpy(path, p, sizeof(p)+1);
    strncpy(path_back, "settingsSettingsGuiSettings.ini~", sizeof(p)+2);
    struct stat sb;
//    stat(path, &sb);
    if(stat(path, &sb) != 0) {
        goto leave;
    }
    fileContent = malloc(sb.st_size);
    FILE* f = fopen(path, "r");
    char* line=NULL;
    size_t len = 0;
    while(getline(&line, &len, f) > 0) {
        char *key = malloc(MAX_STRING_LEN), *value = malloc(MAX_STRING_LEN);
        if(sscanf(line, " %"STR(MAX_STRING_LEN)"s = %"STR(MAX_STRING_LEN)"s ", key, value) > 0) {
            shput(hmap, key, value);


//            int ind = shgeti(hmap, key);
//            DEBUG_PRINT(ind, "%d");
        } else ABORT("settings file is corrupted");

//        int ind = shgeti(hmap, "bpm");
//        DEBUG_PRINT(ind, "%d");
    }

//    int ind = shgeti(hmap, "bpm");
//    DEBUG_PRINT(ind, "%d");
    fclose(f);

//    ind = shgeti(hmap, "bpm");
//    DEBUG_PRINT(ind, "%d");
    leave:
    initialized = true;

//    ind = shgeti(hmap, "bpm");
//    DEBUG_PRINT(ind, "%d");
}
#endif
static void save() {
    FILE* f = fopen(path_back, "w");
//    char* line;
//    int len = 0;
    for (int i=0; i < shlen(hmap); ++i) {
        fprintf(f, "%s = %s\n", hmap[i].key, hmap[i].value);
    }
    fclose(f);
#ifndef WIN32
    rename(path_back, path);
#else
    ReplaceFileA(path, path_back, NULL, 0,0,0);
#endif
}
int loadInt(char *name, bool* success)
{
    init();
    int index = (int)shgeti(hmap, name);
    if(index >= 0) {
        *success = true;
        char* s = hmap[index].value;
        int res;
        sscanf(s, " %d ", &res);
        return res;
    } else {
        *success = false;
        return 0;
    }
}

void saveInt(char *name, int value)
{
    init();
    int index = (int)shgeti(hmap, name);
    if(index >= 0) {
        free(hmap[index].value);
    }
    char * string = malloc(20);
    snprintf(string, 20, "%d", value);
    shput(hmap, name, string);
    save();
    return;
}

double loadDouble(char *name, bool* success)
{
    init();
    int index = (int)shgeti(hmap, name);
    if(index >= 0) {
        *success = true;
        char* s = hmap[index].value;
        double res;
        sscanf(s, " %lf ", &res);
        return res;
    } else {
        *success = false;
        return 0;
    }
}
void saveDouble(char *name, double value)
{
    init();
    int index = (int)shgeti(hmap, name);
    if(index >= 0) {
        free(hmap[index].value);
    }
    char * string = malloc(20);
    snprintf(string, 20, "%lf", value);
    shput(hmap, name, string);

    save();
    return;
}
