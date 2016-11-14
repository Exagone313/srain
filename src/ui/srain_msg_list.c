/**
 * @file srain_msg_list.c
 * @brief A auto-scrolling, dynamic loading listbox used to display messages
 * @author Shengyu Zhang <lastavengers@outlook.com>
 * @version 1.0
 * @date 2016-05-19
 *
 * Note: Unlike SrainUserList, SrainMagList is subclass of GtkScrolledWindow
 */

#define __LOG_ON

#include <gtk/gtk.h>
#include <string.h>

#include "ui.h"
#include "ui_common.h"
#include "ui_hdr.h"
#include "srain_msg_list.h"
#include "srain_window.h"
#include "srain_msg.h"

#include "srv_session.h"

#include "i18n.h"
#include "markup.h"
#include "log.h"

#define MAX_MSG_COUNT 100

struct _SrainMsgList {
    GtkScrolledWindow parent;

    int vis_row_num;
    GtkListBox *list_box;
    GtkWidget *last_msg;
};

struct _SrainMsgListClass {
    GtkScrolledWindowClass parent_class;
};

G_DEFINE_TYPE(SrainMsgList, srain_msg_list, GTK_TYPE_SCROLLED_WINDOW);

static int get_list_box_length(GtkListBox *list_box){
    if (GTK_IS_LIST_BOX(list_box)){
        return g_list_length(
                gtk_container_get_children(GTK_CONTAINER(list_box)));
    }

    return 0;
}

/* scrolled_window_on_edge_overshot() and scrolled_window_on_edge_reached ()
 * are used for implement dynamic hide&load messages */

static void scrolled_window_on_edge_overshot(GtkScrolledWindow *swin,
        GtkPositionType pos, gpointer user_data){
    int i;
    SrainMsgList *list;
    GtkListBoxRow *row;

    if (pos != GTK_POS_TOP) return;

    DBG_FR("Overshot");

    list = user_data;

    for (i = MAX_MSG_COUNT - 1;
            list->vis_row_num >= 0 && i >= 0;
            list->vis_row_num--, i--){
        row = gtk_list_box_get_row_at_index(
                list->list_box, list->vis_row_num);
        DBG_FR("Hide row %p", row);
        if (GTK_IS_LIST_BOX_ROW(row)){
            gtk_widget_set_visible(GTK_WIDGET(row), TRUE);
        }
    }
}

static void scrolled_window_on_edge_reached(GtkScrolledWindow *swin,
               GtkPositionType pos, gpointer user_data){
    int len;
    SrainMsgList *list;
    GtkListBoxRow *row;

    if (pos != GTK_POS_BOTTOM) return;

    DBG_FR("Reached");
    list = user_data;

    len = get_list_box_length(list->list_box);
    for ( ;list->vis_row_num < len - MAX_MSG_COUNT;
            list->vis_row_num++){
        row = gtk_list_box_get_row_at_index(
                list->list_box, list->vis_row_num);
        DBG_FR("Hide row %p", row);
        if (GTK_IS_LIST_BOX_ROW(row)){
            gtk_widget_set_visible(GTK_WIDGET(row), FALSE);
        }
    }
}

/**
 * @brief scroll_to_bottom_idle
 *
 * @param list
 *
 * @return Always return FALSE
 *
 * This function must be called as a idle.
 *
 */
static gboolean scroll_to_bottom_idle(SrainMsgList *list){
    GtkAdjustment *adj;

    /* if this instance has been freed */
    g_return_val_if_fail(SRAIN_IS_MSG_LIST(list), FALSE);

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(list));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) -
            gtk_adjustment_get_page_size(adj));
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(list), adj);

    return FALSE;
}

/**
 * @brief smart_scroll
 *
 * @param list
 * @param force If force = 1, scroll to bottom anyway.
 *
 * If force != 1,
 * and the top-level window is visible,
 * and `list` is belonged to the current SrainChat,
 * and the value of scrolled window's adjustment (scrollbar):
 *      value + page size > max value (upper - page size),
 * scroll the list to the bottom.
 *
 */
static void smart_scroll(SrainMsgList *list, int force){
    double val;
    double max_val;
    double page_size;
    GtkAdjustment *adj;
    SrainWindow *win;
    SrainChat *chat;

    win = SRAIN_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(list)));
    if (!SRAIN_IS_WINDOW(win)){
        ERR_FR("Top level widget is not SrainWindow");
        return;
    }

    chat = srain_window_get_cur_chat(win);
    if (!SRAIN_IS_CHAT(chat)){
        ERR_FR("Current chat is no a SrainChat");
        return;
    }

    if (force){
        gdk_threads_add_idle((GSourceFunc)scroll_to_bottom_idle, list);
        return;
    }

    if (gtk_widget_get_visible(GTK_WIDGET(win))
            && srain_chat_get_msg_list(chat) == list){

        adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(list));
        val = gtk_adjustment_get_value(adj);
        page_size = gtk_adjustment_get_page_size(adj);
        max_val = gtk_adjustment_get_upper(adj) - page_size;

        if (val + page_size > max_val){
            gdk_threads_add_idle((GSourceFunc)scroll_to_bottom_idle, list);
        }
    }
}

void srain_msg_list_scroll_up(SrainMsgList *list, double step){
    GtkAdjustment *adj;

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(list));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_value(adj) - step);
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(list), adj);

    while (gtk_events_pending()) gtk_main_iteration();
}

void srain_msg_list_scroll_down(SrainMsgList *list, double step){
    GtkAdjustment *adj;

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(list));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_value(adj) + step);
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(list), adj);

    while (gtk_events_pending()) gtk_main_iteration();
}

static void srain_msg_list_init(SrainMsgList *self){
    gtk_widget_init_template(GTK_WIDGET(self));

    self->vis_row_num = 0;
    g_signal_connect(self, "edge-overshot",
            G_CALLBACK(scrolled_window_on_edge_overshot), self);
    g_signal_connect(self, "edge-reached",
            G_CALLBACK(scrolled_window_on_edge_reached), self);
}

static void srain_msg_list_class_init(SrainMsgListClass *class){
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
            "/org/gtk/srain/msg_list.glade");

    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SrainMsgList, list_box);
}

SrainMsgList* srain_msg_list_new(void){
    return g_object_new(SRAIN_TYPE_MSG_LIST, NULL);
}

void srain_msg_list_sys_msg_add(SrainMsgList *list, const char *msg,
        SysMsgType type, SrainMsgFlag flag){
    GtkListBoxRow *row;
    SrainSysMsg *smsg;

    smsg = srain_sys_msg_new(msg, type);
    row = gtk_list_box_add_unfocusable_row(list->list_box, GTK_WIDGET(smsg));

    if (flag & SRAIN_MSG_MENTIONED){
       gtk_widget_set_name(GTK_WIDGET(row), "mentioned_msg");
    }

    list->last_msg = GTK_WIDGET(smsg);

    smart_scroll(list, 0);
}


void srain_msg_list_send_msg_add(SrainMsgList *list, const char *msg, SrainMsgFlag flag){
    SrainSendMsg *smsg;

    smsg = srain_send_msg_new(msg);
    gtk_list_box_add_unfocusable_row(list->list_box, GTK_WIDGET(smsg));

    list->last_msg = GTK_WIDGET(smsg);

    smart_scroll(list, 1);
}

static void _srain_msg_list_recv_msg_add(SrainMsgList *list, const char *nick,
        const char *id, const char *msg, SrainMsgFlag flag){
    GtkListBoxRow *row;
    SrainRecvMsg *smsg;

    smsg = srain_recv_msg_new(nick, id, msg);
    row = gtk_list_box_add_unfocusable_row(list->list_box, GTK_WIDGET(smsg));
    if (flag & SRAIN_MSG_MENTIONED){
       gtk_widget_set_name(GTK_WIDGET(row), "mentioned_msg");
    }

    list->last_msg = GTK_WIDGET(smsg);

    smart_scroll(list, 0);
}

/* Add a SrainRecvMsg into SrainMsgList.
 * If its time is same to the last msg, combine them.
 */
void srain_msg_list_recv_msg_add(SrainMsgList *list, const char *nick,
        const char *id, const char *msg, SrainMsgFlag flag){
    char timestr[32];
    const char *old_msg;
    const char *old_timestr;
    const char *old_nick;
    GString *new_msg;
    GString *markuped_msg;
    SrainRecvMsg *last_recv_msg;

    get_cur_time(timestr);

    if (list->last_msg && SRAIN_IS_RECV_MSG(list->last_msg)){
        last_recv_msg = SRAIN_RECV_MSG(list->last_msg);
        old_msg = gtk_label_get_text(last_recv_msg->msg_label);
        old_timestr = gtk_label_get_text(last_recv_msg->time_label);
        old_nick = gtk_label_get_text(last_recv_msg->nick_label);

        /* A message that
         *  - send by the same people
         *  - send in same minute
         *  - less then 512 char
         * can be combine
         */
        if (!flag & SRAIN_MSG_MENTIONED
                && strncmp(timestr, old_timestr, 32) == 0
                && strncmp(nick, old_nick, NICK_LEN) == 0
                && strlen(old_msg) < 512){
            new_msg = g_string_new(old_msg);
            g_string_append(new_msg, "\n");
            g_string_append(new_msg, msg);

            markuped_msg = markup(new_msg->str, NULL);
            if (markuped_msg){
                gtk_label_set_markup(last_recv_msg->msg_label, markuped_msg->str);
                g_string_free(markuped_msg, TRUE);
            } else {
                gtk_label_set_text(last_recv_msg->msg_label, new_msg->str);
            }

            g_string_free(new_msg, TRUE);
            smart_scroll(list, 0);

            return;
        }
    }

    _srain_msg_list_recv_msg_add(list, nick, id, msg, flag);
}
