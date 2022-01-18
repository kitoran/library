#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <libconfig.h>
#include "stb_ds.h"


int main(int argc, char**argv)
{

    char appName[] = "additive";

    char path[PATH_MAX] = {};
    char* home = getenv("HOME");
    assert(home);
    strncat(path, home, PATH_MAX);
    strncat(path, "/.config/", PATH_MAX);
    struct stat sb;
    if(stat(path, &sb) != 0 || (! S_ISDIR(sb.st_mode))) {
        abort();
    }
    strncat(path, appName, PATH_MAX);
    if(stat(path, &sb) != 0 || (! S_ISDIR(sb.st_mode))) {
        mkdir(path, 0777);
    }
    strncat(path, "/", PATH_MAX);
    strncat(path, appName, PATH_MAX);
    strncat(path, ".conf", PATH_MAX);


    config_t cfg;
    config_setting_t *setting, *setting2;
    const char *str;

    config_init(&cfg);

    /* Read the file. If there is an error, report it and exit. */
//    if(! config_read_file(&cfg, path))
//    {
//        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
//                config_error_line(&cfg), config_error_text(&cfg));
//        config_destroy(&cfg);
//        return(EXIT_FAILURE);
//    }

    setting = config_root_setting(&cfg);
    setting2 = config_setting_add(setting, "title", CONFIG_TYPE_STRING);
    config_setting_set_string(setting2, "Buckaroo Banzai");

    if(! config_write_file(&cfg, path))
    {
        fprintf(stderr, "Error while writing file.\n");
        config_destroy(&cfg);
        return(EXIT_FAILURE);
    }
    fprintf(stderr, "wrote to %s\n", path);

//    config_write_file(&cfg, path);
    config_destroy(&cfg);

    printf("Hello World!\n");
    return 0;
}
