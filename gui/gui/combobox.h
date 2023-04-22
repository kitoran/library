#ifndef COMBOBOX_H
#define COMBOBOX_H
//int guiComboBox(
#include <gui.h>
extern Painter dropDownPainter;
bool guiComboBoxZT(Painter *p, const char * const *elements, int* current);
#define guiEnumComboBox(p, en, cur)  guiComboBoxZT(p, en ## Names, (int*)(cur))
void proccessComboBox();
//#define guiEnumComboBox(p, en, cur)  guiComboBox(p, en # Names
#endif // COMBOBOX_H
