#ifndef __SRAIN_USER_LIST_H
#define __SRAIN_USER_LIST_H

#include <gtk/gtk.h>

typedef enum {
    USER_CHIGUA,    // No prefix
    USER_OWNER,     // ~ mode +q
    USER_ADMIN,     // & mode +a
    USER_FULL_OP,   // @ mode +o
    USER_HALF_OP,   // % mode +h
    USER_VOICED,    // + mode +v
    /* ... */
    USER_TYPE_MAX
} UserType;

#define SRAIN_TYPE_USER_LIST (srain_user_list_get_type())
#define SRAIN_USER_LIST(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), SRAIN_TYPE_USER_LIST, SrainUserList))

typedef struct _SrainUserList SrainUserList;
typedef struct _SrainUserListClass SrainUserListClass;

GType srain_user_list_get_type(void);
SrainUserList *srain_user_list_new(void);
int srain_user_list_add(SrainUserList *list, const char *nick, UserType type);
int srain_user_list_rm(SrainUserList *list, const char *nick);
int srain_user_list_rename(SrainUserList *list, const char *old_nick,
                           const char *new_nick, UserType type);
void srain_user_list_clear(SrainUserList *list);

#endif /* __SRAIN_USER_LIST_H */
