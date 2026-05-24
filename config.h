#ifndef CONFIG_H
#define CONFIG_H

#define DHT_PIN       4
#define LDR_PIN       34
#define LED_R_PIN     25
#define LED_G_PIN     26
#define LED_B_PIN     27
#define BUZZER_PIN    14

#define TEMP_WARN     30.0f
#define TEMP_ALERT    35.0f
#define TEMP_SAFE     28.0f
#define HUM_WARN      70.0f
#define HUM_ALERT     75.0f
#define HUM_SAFE      65.0f
#define HSI_WARN      30.0f
#define LDR_DARK      1500
#define DARK_OFFSET   3.0f

#define POLL_NORMAL_MS     500
#define POLL_FAST_MS       100
#define SENSOR_TIMEOUT_MS  3000
#define HYSTERESIS_MS      2000
#define FAIL_RECOVERY_MS   5000
#define BUFFER_SIZE        3

#endif
