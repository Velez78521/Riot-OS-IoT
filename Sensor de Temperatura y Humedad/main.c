#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "fmt.h"

int main(void)
{

    dht_params_t my_params;
    my_params.pin = GPIO_PIN(0, 0); // Asegúrate de que el pin es correcto
    my_params.type = DHT11;         // Especifica que estás utilizando un DHT11
    my_params.in_mode = DHT_PARAM_PULL;

    dht_t dev;
    if (dht_init(&dev, &my_params) == DHT_OK) {
        printf("DHT11 sensor connected\n");
    } else {
        printf("Failed to connect to DHT11 sensor\n");
        return 1;
    }

    while (1) {


        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11 sensor\n");
        }
char temp_s[10];
size_t n = fmt_s16_dfp(temp_s, temp, -1);
temp_s[n] = '\0';

char hum_s[10];
n = fmt_s16_dfp(hum_s,hum,-1);
hum_s[n]='\0';

printf("T: %s C  H: %s%%\n", temp_s, hum_s);
        xtimer_sleep(2); // Espera 2 segundos antes de la próxima lectura
    }

    return 0;
}
