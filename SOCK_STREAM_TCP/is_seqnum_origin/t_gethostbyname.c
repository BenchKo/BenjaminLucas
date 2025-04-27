#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tlpi_hdr.h"

#define _DEFAULT_SOURCE
/* Nimmt eine Domain bzw einen Hostnamen und gibt uns die IP Addresse dieser wieder + ALIASSE falls vorhanden
Nimmt Ipv6 als auch ipv4. Diese Funktion ist allerdings obsolet. Doch auf bestehenden Systemen oft
vorzufinden         :w ;) */

int main(int argc, char* argv[]) {

    struct hostent *h;
    char** pp;
    char str[INET6_ADDRSTRLEN];

    for(argv++; *argv != NULL; argv++){
        h = gethostbyname(*argv);
        if(h == NULL){
            fprintf(stderr, "gethostbyname() failed for '%s': %s\n", *argv, hstrerror(h_errno));
            continue;
        }
        printf("Offizieller Name: %s\n", h->h_name);

        printf("Alias(se): ");
        for(pp = h->h_aliases; *pp != NULL; pp++){
            printf("%s * ", *pp);
        }
        printf("\n");

        printf("AdressTypus:%s\n", (h->h_addrtype == AF_INET) ? "AF_INET(ipv4)" : (h->h_addrtype == AF_INET6) ? "AF_INET6(ipv6)" : "???");

        if(h->h_addrtype == AF_INET || h->h_addrtype == AF_INET6){
            for(pp = h->h_addr_list; *pp != NULL; pp++){
                printf("%s * ", inet_ntop(h->h_addrtype, *pp, str, INET6_ADDRSTRLEN));
            }
            printf("\n");
        }
    }
    exit(EXIT_SUCCESS);
}