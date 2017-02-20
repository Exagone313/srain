#ifndef __SRAIN_WINDOW_H
#define __SRAIN_WINDOW_H

#include <gtk/gtk.h>
#include "srain_app.h"
#include "srain_chat.h"

#define SRAIN_TYPE_WINDOW (srain_window_get_type())
#define SRAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), SRAIN_TYPE_WINDOW, SrainWindow))
#define SRAIN_IS_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), SRAIN_TYPE_WINDOW))

typedef struct _SrainWindow SrainWindow;
typedef struct _SrainWindowClass SrainWindowClass;

GType srain_window_get_type(void);
SrainWindow *srain_window_new(SrainApp *app);

SrainChat* srain_window_add_chat(SrainWindow *win, SuiSession *sui, const char *name, const char *remark, ChatType type);
void srain_window_rm_chat(SrainWindow *win, SrainChat *chat);
SrainChat *srain_window_get_cur_chat(SrainWindow *win);
SrainChat *srain_window_get_chat(SrainWindow *win, const char *name, const char *remark);
GList* srain_window_get_chats_by_remark(SrainWindow *win, const char *remark);
void srain_window_spinner_toggle(SrainWindow *win, gboolean is_busy);
void srain_window_stack_sidebar_update(SrainWindow *win, SrainChat *chat, const char *nick, const char *msg);
int srain_window_is_active(SrainWindow *win);
void srain_window_tray_icon_stress(SrainWindow *win, int stress);

/* Only one SrainWindow instance in one application */
extern SrainWindow *srain_win;

#endif /* __SRAIN_WINDOW_H */
