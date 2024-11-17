#include <stdio.h>
#include <string.h>

#include "net/af.h"
#include "net/protnum.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "fmt.h"

uint8_t buf[7];

int main(void)
{
    //// Temperatura inicio /////////////////////////////////////////////////////
    dht_params_t my_params;
    my_params.pin = GPIO_PIN(0, 0); // Asegúrate de que el pin es correcto
    my_params.type = DHT11;         // Especifica que estás utilizando un DHT11
    my_params.in_mode = DHT_PARAM_PULL;

    dht_t dev;
    if (dht_init(&dev, &my_params) == DHT_OK)
    {
        printf("DHT11 sensor connected\n");
    }
    else
    {
        printf("Failed to connect to DHT11 sensor\n");
        return 1;
    }
    /// Temperatura final   /////////////////////////////////////////////////////////

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_t sock;

    local.port = 12345; // 0xabcd;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0)
    {
        puts("Error creating UDP sock");
        return 1;
    }

    while (1)
    {

        ////// Temperatura inicio

        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK)
        {
            printf(" ");
        }
        char temp_s[10];
        size_t n = fmt_s16_dfp(temp_s, temp, -1);
        temp_s[n] = '\0';
        char *temperatura = temp_s;

        char hum_s[10];
        n = fmt_s16_dfp(hum_s, hum, -1);
        hum_s[n] = '\0';
        char *humedad = hum_s;

        ///// Temperatura final
        sock_udp_ep_t remote = {.family = AF_INET6};
        ssize_t res;

        remote.port = 12345;
        ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
                                          IPV6_ADDR_MCAST_SCP_LINK_LOCAL);
        if (sock_udp_send(&sock, temperatura, sizeof(temperatura), &remote) < 0)
        {
            puts("Error sending message");
            sock_udp_close(&sock);
            return 1;
        }

        if (sock_udp_send(&sock, humedad, sizeof(humedad), &remote) < 0)
        {
            puts("Error sending message");
            sock_udp_close(&sock);
            return 1;
        }

        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), 1 * US_PER_SEC,
                                 NULL)) < 0)
        {
            if (res == -ETIMEDOUT)
            {
                puts(" "); // Timed out");
            }
            else
            {
                puts("Error receiving message");
            }
        }
        xtimer_sleep(2);
    }

    return 0;
}
