/*
 * Use:
 * LD_PRELOAD="./hijack.so" command
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <net/if.h>
#include <string.h>

int (*_ioctl)(int, unsigned long, ...);

int ioctl(int d, unsigned long request, ...) {
   _ioctl = (int (*)(int, unsigned long, ...)) dlsym(RTLD_NEXT, "ioctl");
   va_list ap;
   int r;
   va_start (ap, request);
   if (request == SIOCGIFHWADDR) {
      struct ifreq *ifreq = va_arg(ap, void *);
      if (strcmp(ifreq->ifr_name, "eth0"))
         r = -1;
      else {
         char mac[] = {0, 0x11, 0x2f, 0x47, 0xce, 0x51};
         int i;
         for (i = 0; i < 6; i++)
            ifreq->ifr_hwaddr.sa_data[i] = mac[i];
         r = 0;
      }
   }
   else
      r = _ioctl(d, request, ap);
   va_end (ap);
   return r;
}
