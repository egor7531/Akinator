#ifndef TREEDUMP_H_INCLUDED
#define TREEDUMP_H_INCLUDED

#include "Tree.h"

void tree_status_errors(int err);
void tree_graphic_dump(const Tree * tree, const char* nameFileDot, const char* nameFilePng);

#endif //TREEDUMP_H_INCLUDED
