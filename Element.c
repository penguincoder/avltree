#include <stdlib.h>

#include "Element.h"

short Compare(Element Data1, Element Data2)
{
    if(*Data1 > *Data2)
        return 1;
    else if(*Data1 < *Data2)
        return -1;
    return 0;
}

void FreeElement(Element Data)
{
    free(Data);
}

