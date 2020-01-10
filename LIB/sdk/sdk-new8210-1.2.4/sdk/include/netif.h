/******************************************************************************
 * netif.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-08-13   1.0.0.0  Li.Hua       written
 ******************************************************************************/

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H


#include <net/if.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



int netif_get_flags(const char *ifname, struct ifreq *ifr);

int netif_get_hwaddr(const char *ifname, struct ifreq *ifr);

int netif_get_ipaddr(const char *ifname, struct ifreq *ifr);
int netif_set_ipaddr(const char *ifname,  struct sockaddr *ipaddr);

int netif_get_broadcast(const char *ifname, struct ifreq *ifr);
int netif_set_broadcast(const char *ifname,  struct sockaddr *broadcast);

int netif_set_hwaddr(const char *ifname, struct sockaddr *hwaddr);

int netif_get_mask(const char *ifname, struct ifreq *ifr);
int netif_set_mask(const char *ifname,  struct sockaddr *netmask);

int netif_get_list(char iflist[][IFNAMSIZ], int num);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end for NETWORK_INTERFACE_H */


