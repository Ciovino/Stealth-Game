#include "../header/face_color.h"
#include <stdlib.h>
#include <windows.h>

struct faceColor{
    char face;
    COLOR text;
    COLOR background;
};

struct fcArray{
    FACE_COLOR* array;
    int size;
    int stored;
};

void Textcolor(COLOR color) // Funzione per i colori
{
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hconsole == INVALID_HANDLE_VALUE)
        return;
    SetConsoleTextAttribute(hconsole, color);
}

void BackGroundAndText(COLOR BackGround, COLOR Text)
{
    Textcolor(BackGround * 16 + Text);
}

FACE_COLOR NewFaceColor(char face, COLOR text, COLOR background)
{
    FACE_COLOR fc = malloc(sizeof(*fc));

    fc->face = face;
    fc->text = text;
    fc->background = background;

    return fc;
}

void FreeFaceColor(FACE_COLOR fc)
{
    free(fc);
}

char GetFace(FACE_COLOR fc)
{
    return fc->face;
}

// Changed because of conflicts with GetTextColor() in widgit.h
COLOR GetColorText(FACE_COLOR fc)
{
    return fc->text;
}

COLOR GetBackgroundColor(FACE_COLOR fc)
{
    return fc->background;
}

FC_ARRAY NewFCArray()
{
    FC_ARRAY fca = malloc(sizeof(*fca));

    fca->array = NULL;
    fca->size = fca->stored = 0;

    return fca;
}

void FreeFCArray(FC_ARRAY fca)
{
    for(int i = 0; i < fca->stored; i++)
        FreeFaceColor(fca->array[i]);

    free(fca->array);
    free(fca);
}

static int FCEquals(FACE_COLOR f1, FACE_COLOR f2)
{
    return f1->face == f2->face;
}

static int InFCArraay(FC_ARRAY fca, FACE_COLOR fc)
{
    for(int i = 0; i < fca->stored; i++)
    {
        if(FCEquals(fca->array[i], fc))
            return 1;
    }
    return 0;
}

void AddFC(FC_ARRAY fca, FACE_COLOR fc)
{
    // Already saved
    if(InFCArraay(fca, fc))
        return;

    // Check for empty array
    if(fca->array == NULL)
    {
        // Create one
        fca->size = 5;
        fca->array = malloc(fca->size * sizeof(*fca->array));
    }

    // Check for enough storage
    if(fca->stored == fca->size)
    {
        FC_ARRAY doubleFCA = NewFCArray();

        // Double space
        doubleFCA->size = 2 * fca->size;
        doubleFCA->stored = fca->stored;
        doubleFCA->array = malloc(doubleFCA->size * sizeof(*doubleFCA->array));

        // Copy info into new array
        for(int i = 0; i < fca->stored; i++) doubleFCA->array[i] = fca->array[i];

        FreeFCArray(fca);
        fca = doubleFCA;
    }

    // Save FC
    fca->array[fca->stored++] = fc;
}

FACE_COLOR GetFCFromFace(FC_ARRAY fca, char face)
{
    for(int i = 0; i < fca->stored; i++)
    {
        if(fca->array[i]->face == face)
            return fca->array[i];
    }
    return NewFaceColor(' ', COL_NORMAL, COL_BLACK);
}