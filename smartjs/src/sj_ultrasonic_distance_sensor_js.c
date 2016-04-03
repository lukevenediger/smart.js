#include "v7/v7.h"
#include "sj_v7_ext.h"
#include "sj_ultrasonic_distance_sensor_js.h"
#include "sj_gpio.h"
#include "sj_common.h"
#include "common/cs_time.h"

static struct v7 *s_v7;

/**
 * Read a distance measurement from the sensor
 */
SJ_PRIVATE enum v7_err DISTANCE_readDistance(struct v7 *v7, v7_val_t *res) {
    v7_val_t jsTriggerPin = v7_arg(v7, 0);
    v7_val_t jsEchoPin = v7_arg(v7, 1);

    if (!v7_is_number(jsTriggerPin)) {
      printf("Trigger pin is non-numeric\n");
      *res = v7_mk_undefined();
    }
    if (!v7_is_number(jsEchoPin)) {
      printf("Echo pin is non-numeric\n");
      *res = v7_mk_undefined();
    }

    int triggerPin = v7_to_number(jsTriggerPin);
    int echoPin = v7_to_number(jsEchoPin);

    sj_gpio_write(triggerPin, 0);
    sj_usleep(10);
    sj_gpio_write(triggerPin, 1);
    sj_usleep(10);
    sj_gpio_write(triggerPin, 0);

    double startTime = 0,
        endTime = 0;
    // 5 second timeout
    double timeout = cs_time() + 5;

    // Wait for pin to go high
    while (1) {
        if (sj_gpio_read(echoPin) == 1) {
            startTime = cs_time();
            break;
        }
        if (cs_time() > timeout) {
            *res = v7_mk_number(-1);
            return V7_OK;
        }
    }

    // Wait for pin to go low
    while (1) {
        if (sj_gpio_read(echoPin) == 0) {
            endTime = cs_time();
            break;
        }
        if (cs_time() > timeout) {
            *res = v7_mk_number(-2);
            return V7_OK;
        }
    }

    // Set the result as the distance in centimeters
    *res = v7_mk_number((endTime - startTime / 2 / 29));

    return V7_OK;
}

/**
 * Initialise the ultrasonic distance sensor library
 */
void sj_ultrasonic_distance_sensor_setup(struct v7 *v7) {
  s_v7 = v7;
  v7_val_t distance = v7_mk_object(v7);
  v7_set(v7, v7_get_global(v7), "DIST", ~0, distance);
  v7_set_method(v7, distance, "readDistance", DISTANCE_readDistance);
}
