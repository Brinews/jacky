#include <stdio.h>
#include <string.h>

#include "snapshot.h"

#define BUFFER_SIZE 20

static snapshot head; /* the dummy head */
head.id = 0;
head.prev = head.next = &head;
static snapshot *current = &head;

enum COMMAND { CMD_BYE, CMD_HELP, CMD_LIST_KEYS, CMD_LIST_ENTRIES, 
    CMD_LIST_SNAPSHOTS, CMD_GET, CMD_DEL, CMD_PURGE, CMD_SET, 
    CMD_PUSH, CMD_APPEND, CMD_PICK, CMD_PLUCK, CMD_POP, CMD_DROP,
    CMD_ROLLBACK, CMD_CHECKOUT, CMD_SNAPSHOT, CMD_MIN, CMD_MAX,
    CMD_SUM, CMD_LEN, CMD_REV, CMD_UNIQ, CMD_SORT,
    CMD_END};

const char *COMMAND_STR[] = {
    "BYE", "HELP", "LIST KEYS", "LIST ENTRIES", "LIST SNAPSHOTS",
    "GET", "DEL", "PURGE", "SET", "PUSH",
    "APPEND", "PICK", "PLUCK", "POP", "DROP",
    "ROLLBACK", "CHECKOUT", "SNAPSHOT", "MIN", "MAX",
    "SUM", "LEN", "REV", "UNIQ", "SORT",
};

const char *COMMAND_HELP[] = {
    "BYE clear database and exit",
    "HELP display this help message",
    "LIST KEYS displays all keys in current state",
    "LIST ENTRIES displays all entries in current state",
    "LIST SNAPSHOTS displays all snapshots in the database",
    "GET <key> displays entry values",
    "DEL <key> deletes entry from current state",
    "PURGE <key> deletes entry from current state and snapshots",
    "SET <key> <value ...> sets entry values",
    "PUSH <key> <value ...> pushes each value to the front one at a time",

    "APPEND <key> <value ...> append each value to the back one at a time",
    "PICK <key> <index> displays entry value at index",
    "PLUCK <key> <index> displays and removes entry value at index",
    "POP <key> displays and removes the front entry value",
    "DROP <id> deletes snapshot",
    "ROLLBACK <id> restores to snapshot and deletes newer snapshots",
    "CHECKOUT <id> replaces current state with a copy of snapshot",
    "SNAPSHOT saves the current state as a snapshot",
    "MIN <key> displays minimum entry value",
    "MAX <key> displays maximum entry value",
    "SUM <key> displays sum of entry values",
    "LEN <key> displays number of entry values",
    "REV <key> reverses order of entry values",
    "UNIQ <key> removes repeated adjacent entry values",
    "SORT <key> sorts entry values in ascending order",
};

struct command {
    enum COMMAND tag;
    union {
        //CMD key
        struct {
            char *key;
        };

        //CMD key value ...
        struct {
            char *key;
            size_t num;
            int *value;
        };

        //CMD id
        struct {
            int id; 
        };
    };
};

/** Checks the return value from *alloc */
static void *check(void *ptr) {
    if (!ptr) {
        perror("*alloc failed");
        abort(); // no point cleaning up, just let the program die
    }
    return ptr;
}


static enum COMMAND find_command(const char *str) {
    for (enum COMMAND i = CMD_BYE; i < CMD_END; i++) {
        if (strcmp(str, COMMAND_STR[i]) == 0) {
            return i;
        }
    }

    return CMD_END;
}

#define list_for_each(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)

/* LIST KEYS */
static void sdb_list_keys(entry* entry_head)
{
    int has_num = 0;

    struct entry *ptr;
    list_for_each (ptr, entry_head) {
        printf("%s\n", ptr->key);
        has_num = 1;
    }

    if (has_num == 0) {
        printf("no keys\n");
    }
}

/* LIST ENTRIES */
static void sdb_list_entries(entry *entry_head)
{
    struct entry *e_ptr;
    struct value *v_ptr;

    int has_num = 0;

    list_for_each (e_ptr, entry_head) {

        struct value *value_head = e_ptr->values;
        printf("%s", e_ptr->key);

        printf("[");
        list_for_each(v_ptr, value_head) {
            printf(" %d", v_ptr->value);
        }
        printf("]\n");

        has_num = 1;
    }

    if (has_num == 0) {
        printf("no entries\n");
    }
}

/* LIST SNAPSHOTS */
static void sdb_list_snapshots(snapshot *snapshot_head)
{
    snapshot *ptr;
    int has_num = 0;

    list_for_each(ptr, snapshot_head) {
        printf("%d\n", ptr->id);
        has_num = 1;
    }

    if (has_num == 0) {
        printf("no snapshots\n");
    }
}

/* common utilites */
static entry* get_entry(char *key, entry *entry_head)
{
    struct entry *ptr;

    list_for_each(ptr, entry_head) {
        if (strcmp(key, ptr->key) == 0) {
            return ptr;
        }
    }

    return NULL;
}

/* GET key */
static void sdb_get_key(char *key, entry *entry_head)
{
    struct entry *ptr = get_entry(key, entry_head);
    printf("[");
    if (NULL != ptr) {
        struct value *eptr, *value_head = ptr->values;
        list_for_each(eptr, value_head) {
            if (eptr != value_head->next) {
                printf(" ");
            }
            printf("%d", eptr->value);
        }
    }
    printf("]\n");
}

#define list_del(entry) do { \
                    (entry)->prev->next = (entry)->next; \
                    (entry)->next->prev = (entry)->prev; \
                    (entry)->next = (entry); \
                    (entry)->prev = (entry); \
                } while (0)

#define list_empty(entry) ((entry)->prev == (entry))

/* common utilites */
static value *pop_entry(entry *ety)
{
    value *ret = NULL;

    if (NULL != ety) {
        value *v_head = ety->values;
        if (!list_empty(v_head)) {
            ret = v_head->next;
            list_del(ret);
        }
    }

    return ret;
}

/* POP key */
static void sdb_pop_entry(char *key, entry *entry_head)
{
    entry *ptr = get_entry(key, entry_head);

    value *vptr = pop_entry(ptr);

    if (NULL != vptr) {
        printf("%d\n", vptr->value);
        free(vptr);
    } else {
        printf("nil\n");
    }
}

/*
 * free the space malloced by entry, common utilites
 */
static void free_entry(entry *ety)
{
    value *vptr;

    while ((vptr = pop_entry(ety)) != NULL) {
        free(vptr);
    }
    
    free(ety->values);
    free(ety);
}

/* DEL key */
static void sdb_del_key(char *key, entry *entry_head)
{
    struct entry *ptr = get_entry(key, entry_head);
    if (NULL == ptr) {
        printf("no such key\n");
        return;
    }

    list_del(ptr);
    free_entry(ptr);
}

/* common utilites */
static void free_snapshot(snapshot *snp)
{
    entry *eptr, *entry_head = snp->entries;

    eptr = entry_head->next;

    while (eptr != entry_head) {
        entry *p = eptr;
        eptr = eptr->next;

        list_del(p);
        free_entry(p);
    }

    free(entry_head);
    free(snp);
}

/* PURGE key */
static void sdb_purge_entry(char *key, snapshot *snapshot_head)
{
    snapshot *sptr;

    list_for_each(sptr, snapshot_head) {
        entry *eptr = get_entry(key, sptr->entries);

        if (NULL != eptr) {
            list_del(eptr);
            free_entry(eptr);
        }
    }

    printf("ok\n");
}

/* common utilites */
static snapshot *new_snapshot(int id)
{
    snapshot *snapshot = check(malloc(sizeof(snapshot)));
    snapshot->prev = snapshot->next = snapshot;
    snapshot->id = id;
    snapshot->entries = check(malloc(sizeof(entry)));

    snapshot->entries->prev 
        = snapshot->entries->next 
        = snapshot->entries;

    return snapshot;
}

/* common utilites */
static entry *new_entry(char *key)
{
    entry *ety = check(malloc(sizeof(entry)));
    ety->prev = ety->next = ety;

    strcpy(ety->key, key);

    ety->values = check(malloc(sizeof(value)));
    ety->values->prev = ety->values->next = ety->values;

    return ety;
}

#define list_add(after, before) do { \
    after->prev = before; \
    after->next = before->next; \
    after->next->prev = after; \
    after->prev->next = after; \
} while (0)

#define list_add_tail(tail, head) do { \
    tail->prev = head->prev; \
    tail->next = head; \
    tail->prev->next = tail; \
    tail->next->prev = tail; \
} while (0)

static value *new_value(int va)
{
    value *v = check(malloc(sizeof(value)));
    v->prev = v->next = v;
    v->value = va;

    return v;
}

/* SET key <values> 
 * If there is a duplicate key, then cover it
 */
static void sdb_set_entry(char *key, int num, int *values, snapshot *snapshot_head)
{
    entry *entry_head = snapshot_head->entries;

    entry *toadd = new_entry(key);
    value *vhead = toadd->values;

    for (int i = num-1; i >= 0; i--) {
        value *v = new_value(values[i]);
        list_add(v, vhead);
    }

    list_add(toadd, entry_head);
}

/* PUSH key <values> */
static void sdb_push_entry(char *key, int num, int *values, entry *head)
{
    entry *ptr = get_entry(key, head);

    if (NULL != ptr) {
        value *vhead = ptr->values;

        for (int i = 0; i < num; i++) {
            value *v = new_value(values[i]);
            list_add(v, vhead);
        }
    }
}

/* APPEND key <values> */
static void sdb_append_entry(char *key, int num, int *values, entry *head)
{
    entry *ptr = get_entry(key, head);
    if (NULL != ptr) {
        value *vhead = ptr->values;

        for (int i = 0; i < num; i++) {
            value *v = new_value(values[i]);
            list_add_tail(v, vhead);
        }
    }
}

/* common utilites : i is in-range */
static value *get_i_entry(entry *ety, int idx)
{
    value *ret = NULL;

    if (NULL != ety) {
        value *vhead = ety->values;
        value *ptr;
        int i = 0;

        list_for_each (ptr, vhead) {
            i++;
            if (i == idx) {
                ret = ptr;
                break;
            }
        }
    }

    return ret;
}

/* PICK key index */
static void sdb_pick_entry(char *key, int idx, entry *head)
{
    entry *ety = get_entry(key, head);
    value *v = get_i_entry(ety, idx);

    if (NULL == v) {
        printf("index out of range\n");
    } else {
        printf("%d\n", v->value);
    }
}

/* PLUCK key index */
static void sdb_pluck_entry(char *key, int idx, entry *head)
{
    entry *ety = get_entry(key, head);
    value *v = get_i_entry(ety, idx);

    if (NULL == v) {
        printf("index out of range\n");
    } else {
        printf("%d\n", v->value);
        list_del(v);
        free(v);
    }
}

/* find snapshot */
static snapshot *get_snapshot(int id, snapshot *head)
{
    snapshot *ptr;
    list_for_each(ptr, head) {
        if (ptr->id == id) {
            return ptr;
        }
    }

    return NULL;
}

/* DROP id */
static void sdb_drop_snapshot(int id, snapshot *head)
{
    snapshot *ptr = get_snapshot(id, head);
    if (NULL == ptr) {
        printf("no such snapshot\n");
    } else {
        list_del(ptr);
        free_snapshot(ptr);
        printf("ok\n");
    }
}

/* ROLLBACK id */
static snapshot *sdb_rollback_snapshot(int id, snapshot *head)
{
    snapshot *ptr = get_snapshot(id, head);

    if (NULL == ptr) {
        printf("no such snapshot\n");
        return current;
    } else {
        snapshot *p = ptr->next;
        while (p != head) {
            snapshot *now = p;
            p = p->next;

            list_del(now);
            free_snapshot(now);
        }

        current = ptr; // the current snapshot change

        return ptr;
    }
}

/* CHECKOUT id */
static snapshot *sdb_checkout_snapshot(int id, snapshot *head)
{
    snapshot *ptr = get_snapshot(id, head);

    if (NULL == ptr) {
        printf("no such snapshot\n");
        return current;
    } else {
        current = ptr; // change the current snapshot

        return ptr;
    }
}

/* SNAPSHOT */
static int sdb_do_snapshot(snapshot *head)
{
    int last_id = head->prev->id;
    snapshot *new_ns = new_snapshot(++last_id);
    entry *new_ety_head = new_ns->entries;

    snapshot *last_ns = head->prev;
    entry *ehead = last_ns->entries;
    entry *eptr;

    list_for_each(eptr, ehead) {
        entry *new_ety = new_entry(eptr->key);
        value *new_val_head = new_ety->values;

        value *vhead = eptr->values;
        value *vptr;
        list_for_each(vptr, vhead) {
            value *new_val = new_value(vptr->value);
            list_add_tail(new_val, new_val_head); // add value list
        }

        list_add_tail(new_ety, new_ety_head); // add entry list
    }

    printf("save as snapshot %d\n", last_id);

    current = new_ns;

    return last_id;
}

/* MIN key */
static int sdb_min_entry(char *key, entry *head)
{
    entry *ptr = get_entry(key, head);

    if (NULL == ptr) {
        printf("no such key\n");
    } else {
        int min = 0x7fffffff;
        value *vptr, *vhead = ptr->values;

        list_for_each(vptr, vhead) {
            if (vptr->value < min) {
                min = vptr->value;
            }
        }

        printf("%d\n", min);
    }
}

/* MAX key */
static int sdb_max_entry(char *key, entry *head)
{
    entry *ptr = get_entry(key, head);

    if (NULL == ptr) {
        printf("no such key\n");
    } else {
        int max = -0x7fffffff;
        value *vptr, *vhead = ptr->values;

        list_for_each(vptr, vhead) {
            if (vptr->value > max) {
                max = vptr->value;
            }
        }

        printf("%d\n", max);
    }
}

/* SUM key */
static int sdb_sum_entry(char *key, entry *head)
{
    entry *ptr = get_entry(key, head);

    if (NULL == ptr) {
        printf("no such key\n");
    } else {
        int sum = 0;
        value *vptr, *vhead = ptr->values;

        list_for_each(vptr, vhead) {
            sum += vptr->value;
        }

        printf("%d\n", sum);
    }
}

/* LEN key */
static int sdb_len_entry(char *key, entry *head)
{
    entry *ptr = get_entry(key, head);

    if (NULL == ptr) {
        printf("no such key\n");
    } else {
        int len = 0;
        value *vptr, *vhead = ptr->values;

        list_for_each(vptr, vhead) {
            len++;
        }

        printf("%d\n", len);
    }
}

/* REV key */
static void sdb_rev_entry(char *key, entry *head)
{
    entry *ety = get_entry(key, head);
    if (NULL == ety) {
        printf("no such key\n");
    } else {
        value *vptr, *vhead = ety->values;
        value *temp;

        vptr = vhead->next;
        while (vptr != vhead) {
            value *vp = vptr;
            vptr = vptr->next;

            /* swap */
            temp = vp->prev;
            vp->prev = vp->next;
            vp->next = temp;
        }

        /* swap */
        temp = vhead->prev;
        vhead->prev = vhead->next;
        vhaed->next = temp;

        printf("ok\n");
    }
}

/* UNIQ key */
static void std_uniq_entry(char *key, entry *head)
{
    entry *ety = get_entry(key, head);

    if (NULL == ety) {
        printf("no such key\n");
    } else {
        value *vptr, *vhead = ety->values;
        value *vp = NULL;

        list_for_each(vptr, vhead) {
            if (NULL != vp && vp->value == vptr->value) {
                list_del(vp);
                free(vp);
            }
            
            vp = vptr;
        }
    }
}

/* SORT key */
static void sdb_sort_entry(char *key, entry *head)
{
    entry *ety = get_entry(key, head);

    if (NULL == ety) {
        printf("no such entry\n");
    } else {
        /* bubble sort */
        value *vptr1, *vptr2, *vhead = ety->values;

        for (vptr1 = vhead->next; vptr1 != vhead; vptr1 = vptr1->next) {
            for (vptr2 = vptr1->next; vptr2 != vhead; vptr2 = vptr2->next) {
                if (vptr2->value < vptr1->value) {
                    int tmp = vptr2->value;
                    vaptr2->value = vptr1->value;
                    vptr1->value = tmp;
                }
            }
        }

        printf("ok\n");
    }
}
