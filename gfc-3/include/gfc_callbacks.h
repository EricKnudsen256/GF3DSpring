#ifndef __GFC_CALLBACKS_H__
#define __GFC_CALLBACKS_H__

typedef struct
{
    void *data;
    void (*callback)(void *data);
}Callback;

Callback *gfc_callback_new(void (*callback)(void *data),void *data);
void gfc_callback_free(Callback *callback);
void gfc_callback_call(Callback *callback);

#endif
