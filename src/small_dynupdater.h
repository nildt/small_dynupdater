#ifndef __small_dynupdater_h_
#define __small_dynupdater_h_

// #define NDEBUG
/* #define NO_LINENOS */

/* Credentials */
#define UNAME "YOURUSERNAME"
#define PASS "YOURRANDOMTOKEN"
#define SUBDOMAIN "https://api.twodns.de/hosts/YOURDYNNAME.de"

/* Time between IP-checks */
#define DELAY 300

int sigcatch = 1;

struct string {
    char *ptr;
    size_t len;
};

void signal_callback_handler(int signum);
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
size_t output_dummy(void *buffer, size_t size, size_t nmemb, void *s);
int extract_ip(struct string *s);
int update_dns(void);
int get_ip(struct string *s);
int cue(void);
#endif

