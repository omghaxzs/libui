// 6 april 2015
#include <stdlib.h>

extern uiInitOptions options;

extern void *uiAlloc(size_t);
#define uiNew(T) ((T *) uiAlloc(sizeof (T)))
extern void *uiRealloc(void *, size_t);
extern void uiFree(void *);

extern void complain(const char *, ...);

extern uiContainer *newBin(void);
extern void binSetMainControl(uiContainer *, uiControl *);
extern void binSetMargins(uiContainer *, intmax_t, intmax_t, intmax_t, intmax_t);
extern void binSetParent(uiContainer *, uintptr_t);
