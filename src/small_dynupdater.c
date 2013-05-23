#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <curl/curl.h>
/* Important before dbg.h; because of some #defines... */
#include "small_dynupdater.h"
#include "dbg.h"

int main(void)
{
    signal(SIGINT, signal_callback_handler);

    /* Return-Variable, curl_easy_strerror() */
    CURLcode rc;

    /* General Information */
    log_info("[+] Will check if IP-Adress has to be updated...\n[INFO] [+] Written by Nils Wildt 2013");
    log_info("[+] Press enter to continue");
	char enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }

    /* Needed to init libcurl, need SSL... */
    rc = curl_global_init(CURL_GLOBAL_ALL);     // CURL_GLOBAL_SSL
    check(rc == 0, "[-] Could not initialize the libcurl.");

    /* starts */
    rc = cue();
    debug("Return value from get_ip is %d", rc);
    check(rc == 0, "[-] Aborting!");

    log_info("[+] Closing now.");
    /* Always: */
    curl_global_cleanup();
    return EXIT_SUCCESS;

  error:
    /* Prints Curl Error Code */
    curl_easy_strerror(rc);
    /* clean config */
    curl_global_cleanup();
    /* Exit with Error (1) */
    return EXIT_FAILURE;
}

void signal_callback_handler(int signum)
{
#ifndef NDEBUG
    printf("Caught signal %d\n", signum);
#endif
    sigcatch = 0;
    log_info("[+] Will try to cleanup...");
}

/* Needed to read the IP - String... */
void init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    
    s->ptr[0] = '\0';

}

/* gets from the libcurl the webpage... */
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(s->ptr + s->len, ptr, size * nmemb);
    
    /* Manual putting the termination */
    s->ptr[new_len] = '\0';
    
    
    s->len = new_len;

    return size * nmemb;
}

size_t output_dummy(void *buffer, size_t size, size_t nmemb, void *s)
{
    /* Added to supress further output! */
    return size * nmemb;
}


/* I admit, it's confusing */
int extract_ip(struct string *s)
{
    /* Our case: */
    unsigned int iplength = (s->len) - 90;
    char ip[iplength];

    strncpy(ip, s->ptr + 76, iplength);
    ip[iplength] = 0;	
    strncpy(s->ptr, ip, iplength + 1);
    return 0;
}

int update_dns(void)
{
    /* handler */
    CURL *dns;

    /* Error Variable */
    CURLcode rc;

    /* Init session */
    dns = curl_easy_init();

    /* Json- Object */
    char *json = "{\"ip_address\": \"auto\"}";
    unsigned int length = strlen(json);

    if (dns) {

        /* Allow redirect */
        curl_easy_setopt(dns, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(dns, CURLOPT_POSTREDIR, 1);
        curl_easy_setopt(dns, CURLOPT_POSTFIELDSIZE, length);

#ifndef NDEBUG
        /* Be Verbose! & Fail on errors! */
        curl_easy_setopt(dns, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(dns, CURLOPT_FAILONERROR, 1L);
#endif

        /* Prevent the output */
        curl_easy_setopt(dns, CURLOPT_WRITEFUNCTION, output_dummy);

        /* The URL */
        curl_easy_setopt(dns, CURLOPT_URL, SUBDOMAIN);

        /* Credentials */
        curl_easy_setopt(dns, CURLOPT_USERNAME, UNAME);
        curl_easy_setopt(dns, CURLOPT_PASSWORD, PASS);

        /* Requesttype */
        curl_easy_setopt(dns, CURLOPT_CUSTOMREQUEST, "PUT");

        /* Define the Post input [json variable] */
        curl_easy_setopt(dns, CURLOPT_POSTFIELDS, json);

        /* set the file with the certs vaildating the server */
     
/*        curl_easy_setopt(dns, CURLOPT_CAPATH, "Whatever...");*/

        /* Disable SSL Verification */
        curl_easy_setopt(dns, CURLOPT_SSL_VERIFYPEER, 0);

        /* some servers don't like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(dns, CURLOPT_USERAGENT, "curl/7.29.0");

        /* Perform the request, res will get the return code */
        rc = curl_easy_perform(dns);

        check(rc == CURLE_OK, "[-] updating IP failed. Exit with code: %s", curl_easy_strerror(rc));
    }
    /* Ever */
    curl_easy_cleanup(dns);

    return 0;

  error:
    /* Ever */
    curl_easy_cleanup(dns);
    return 1;
}


int cue(void)
{

	/* Return value */
    int rc = 1;
    
    /* Carries the data from get ip */
    struct string s;
    
    /* Initialises String */ 
    init_string(&s);
    
    rc = get_ip(&s);
    if (rc == 1) {
    	log_info("[-] Unable to fetch IP.");
    }
    
    /* will temporary save the IP */
    char temp[s.len];
    
    log_info("[+] Your IP: %s", s.ptr);
    rc = update_dns();
    if (rc == 1){
        log_info("[-] Could not perform correct DynIP-Update.");
    }
    
    log_info("[+] Performed first dynupdate.");

	/* Copy IP to  temp */
	strncpy(temp,s.ptr,strlen(s.ptr)+1);

	/* Not pressing ctr+c */
    while (sigcatch) {

		/* old is the new? */
        if (strcmp(s.ptr, temp) == 0) {
   			
   			/* Check actual ip */
   			rc = get_ip(&s);
			if (rc == 1) {
				log_info("[-] Unable to fetch IP.");
			}
        
            log_info("[+] Your IP didn't change. Nothing to do.");
            
            sleep(DELAY);   

        } else {

            rc = update_dns();
            if (rc == 1){
            	log_info("[-] Could not perform correct DynIP-Update.");
            }
            
            /* Hence new value; old = new... unil changes ;D */
			strncpy(temp,s.ptr,strlen(s.ptr)+1);
            
            log_info("[+] Your IP NOW: %s", s.ptr);
            log_info("[+] Performed DynIP update!");
            
            sleep(DELAY);
        }
    }

    if (s.ptr)
        free(s.ptr);

    return 0;

}

int get_ip(struct string *s)
{
	
	/* Reset -- very important!!!!!!!! */
    s->len = 0;
    s->ptr[0] = '\0';
    
	
    /* handler */
    CURL *curl;

    /* Error Variable */
    CURLcode rc;
    
    /* Init session */
    curl = curl_easy_init();

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "http://checkip.twodns.de");

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);



        /* Trying to save the request to extract IP */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);

        /* some servers don't like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* Perform the request, res will get the return code */
        rc = curl_easy_perform(curl);


        if (rc == CURLE_OK){
        
            /* Will extract the IP */
        	rc = extract_ip(s);
	        check(rc == 0, "[-] couldn't extract the IP");
	        
        } else {
        
        	 log_info("[-] getting actual IP failed. Exit with code: %s", curl_easy_strerror(rc));
        	 
        }


    }
    
    
    
    /* always cleanup */
    curl_easy_cleanup(curl);

    return 0;

  error:

    curl_easy_cleanup(curl);
    return 1;
}

