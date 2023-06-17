#include <misc.h>
#include <windows.h>
#include <stdio.h>
_Bool aborted = false;
void myAbort(char* message) {
    aborted = true;
    char general[] = "sorry. Hope it didn't affect the rest of Reaper";
    char whole[sizeof(general)+1000];
    snprintf(whole, sizeof(whole),
             MY_PATH " %s\n"
             "the error string is:\n\n"
             "%s\n\n\n"
             "would be real cool if you messaged the developer about this and "
             "included the above error string at kitttoran@gmail.com\n"
             "(did you know you can press Ctrl+V to copy all the text in a messagebox? I didn't)",
             general, message);
    MessageBoxA(
      NULL,
      whole,
      "microtonal midi editor stopped because the developer did an oopsie",
      MB_OK
    );
    HANDLE ct = GetCurrentThread();
    TerminateThread(ct, 1);
}
