#ifndef XENBUS_H__
#define XENBUS_H__

#include <xen/interface/xen.h>
#include <xen/interface/io/xenbus.h>

typedef unsigned long xenbus_transaction_t;
#define XBT_NIL ((xenbus_transaction_t)0)

extern uint32_t xenbus_evtchn;

/* Initialize the XenBus system. */
void init_xenbus(void);
/* Finalize the XenBus system. */
void fini_xenbus(void);

/* Read the value associated with a path.  Returns a malloc'd error
   string on failure and sets *value to NULL.  On success, *value is
   set to a malloc'd copy of the value. */
char *xenbus_read(xenbus_transaction_t xbt, const char *path, char **value);

char *xenbus_wait_for_state_change(const char *path, XenbusState *state);
char *xenbus_switch_state(xenbus_transaction_t xbt, const char* path,
                          XenbusState state);


/* Associates a value with a path.  Returns a malloc'd error string on
   failure. */
char *xenbus_write(xenbus_transaction_t xbt, const char *path,
                   const char *value);

/* Removes the value associated with a path.  Returns a malloc'd error
   string on failure. */
char *xenbus_rm(xenbus_transaction_t xbt, const char *path);

/* List the contents of a directory.  Returns a malloc'd error string
   on failure and sets *contents to NULL.  On success, *contents is
   set to a malloc'd array of pointers to malloc'd strings.  The array
   is NULL terminated.  May block. */
char *xenbus_ls(xenbus_transaction_t xbt, const char *prefix, char ***contents);

/* Reads permissions associated with a path.  Returns a malloc'd error
   string on failure and sets *value to NULL.  On success, *value is
   set to a malloc'd copy of the value. */
char *xenbus_get_perms(xenbus_transaction_t xbt, const char *path, char **value);

/* Sets the permissions associated with a path.  Returns a malloc'd
   error string on failure. */
char *xenbus_set_perms(xenbus_transaction_t xbt, const char *path, domid_t dom,
                       char perm);

/* Start a xenbus transaction.  Returns the transaction in xbt on
   success or a malloc'd error string otherwise. */
char *xenbus_transaction_start(xenbus_transaction_t *xbt);

/* End a xenbus transaction.  Returns a malloc'd error string if it
   fails.  abort says whether the transaction should be aborted.
   Returns 1 in *retry iff the transaction should be retried. */
char *xenbus_transaction_end(xenbus_transaction_t, int abort,
			     int *retry);

/* Read path and parse it as an integer.  Returns -1 on error. */
int xenbus_read_integer(const char *path);

/* Read path and parse it as 16 byte uuid. Returns 1 if
 * read and parsing were successful, 0 if not */
int xenbus_read_uuid(const char* path, unsigned char uuid[16]);

/* Contraction of snprintf and xenbus_write(path/node). */
char* xenbus_printf(xenbus_transaction_t xbt,
                    const char* node, const char* path,
                    const char* fmt, ...)
    __attribute__((__format__(printf, 4, 5)));

/* Utility function to figure out our domain id */
domid_t xenbus_get_self_id(void);

#endif /* XENBUS_H__ */
