#include "gfc_callbacks.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

Callback *gfc_callback_new(void (*callback)(void *data),void *data)
{
    Callback *c;
    c = (Callback *)malloc(sizeof(Callback));
    if (!c)
    {
        slog("failed to allocate callback");
        return NULL;
    }
    memset(c,0,sizeof(Callback));
    c->callback = callback;
    c->data = data;
    return c;
}

void gfc_callback_free(Callback *callback)
{
    if (!callback)return;
    free(callback);
}

void gfc_callback_call(Callback *callback)
{
    if (!callback)return;
    if (callback->callback)
    {
        callback->callback(callback->data);
    }
    else
    {
        slog("callback not defined");
    }
}

/*eol@eof*/
