#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include "tlpi_hdr.h"
int inetConnect(const char* host, const char* service, int type);

int inetListen(const char* service, int backlog, socklen_t* addrlen);

int inetBind(const char* service, int type, socklen_t* addrlen);

char* inetAddressStr(const struct sockaddr* addr, socklen_t addrlen, char* addrStr, int addrStrlen);
static int inetPassiveSocket(const char* service, int type, socklen_t* addrlen, Boolean doListen, int backlog);

#define IS_ADDR_STR_LEN 4096

/* empfohlene Länge für String Puffer
    dessen Puffer der caller an inetAddressStr() geben soll.
    Muss größer sein als (NI_MAXHOST + NI_MAX_SERV + 4 )*/


/*

### **Häufig verwendete Optionen mit `setsockopt`**
1. **`SO_REUSEADDR` (Socket-Level):**
   - Ermöglicht das Wiederverwenden einer Adresse, auch wenn der Socket noch nicht vollständig freigegeben wurde.
   - Wird häufig bei Server-Sockets verwendet.

2. **`TCP_NODELAY` (TCP-Level):**
   - Deaktiviert den Nagle-Algorithmus, um kleine Pakete sofort zu senden.

3. **`SO_KEEPALIVE` (Socket-Level):**
   - Aktiviert Keep-Alive-Pakete, um die Verbindung aktiv zu halten und inaktive Verbindungen zu erkennen.

4. **`SO_RCVBUF` und `SO_SNDBUF` (Socket-Level):**
   - Setzt die Größe der Empfangs- und Sendepuffer.

5. **`SO_LINGER` (Socket-Level):**
   - Konfiguriert das Verhalten beim Schließen des Sockets (z. B. ob ausstehende Daten gesendet werden sollen).
   */


