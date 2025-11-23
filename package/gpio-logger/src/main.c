#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define CHIP_PATH "/dev/gpiochip0"
#define LOG_PATH  "/var/log/gpio-logger.log"
#define CONFIG_PATH "/etc/gpio-logger.conf"
#define GPIO_A 2
#define GPIO_B 3

// --- Config structure -----------------------------------------------------

typedef struct {
    int gpio;
    char name[64];
} gpio_entry;

static gpio_entry config[32];
static int config_count = 0;

// Read "GPIO=name" pairs from CONFIG_PATH
static void load_config(void) {
    FILE *f = fopen(CONFIG_PATH, "r");
    if (!f) {
        perror("open config");
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        int gpio;
        char name[64];

        if (line[0] == '#' || strlen(line) < 3)
            continue;

        if (sscanf(line, "%d=%63s", &gpio, name) == 2) {
            config[config_count].gpio = gpio;
            strncpy(config[config_count].name, name, sizeof(config[config_count].name));
            config_count++;
        }
    }

    fclose(f);
}

static const char* get_name_for_gpio(int gpio) {
    for (int i = 0; i < config_count; i++) {
        if (config[i].gpio == gpio)
            return config[i].name;
    }
    static char fallback[32];
    snprintf(fallback, sizeof(fallback), "GPIO_%d", gpio);
    return fallback;
}

// --- Timestamp -------------------------------------------------------------

static void timestamp(char *buf, size_t len) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm tm;
    localtime_r(&ts.tv_sec, &tm);

    snprintf(
        buf, len,
        "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ts.tv_nsec / 1000000
    );
}

// --- Main ------------------------------------------------------------------

int main(void)
{
    load_config();

    const char *nameA = get_name_for_gpio(GPIO_A);
    const char *nameB = get_name_for_gpio(GPIO_B);

    FILE *log = fopen(LOG_PATH, "a");
    if (!log) {
        perror("fopen log");
        return 1;
    }

    struct gpiod_chip *chip = gpiod_chip_open(CHIP_PATH);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    struct gpiod_line *lineA = gpiod_chip_get_line(chip, GPIO_A);
    struct gpiod_line *lineB = gpiod_chip_get_line(chip, GPIO_B);

    if (!lineA || !lineB) {
        fprintf(stderr, "Failed getting GPIO lines\n");
        return 1;
    }

    if (gpiod_line_request_input(lineA, "gpio-logger") < 0 ||
        gpiod_line_request_input(lineB, "gpio-logger") < 0) {
        perror("gpiod_line_request_input");
        return 1;
    }

    while (1) {
        char tbuf[64];

        int valA = gpiod_line_get_value(lineA);
        int valB = gpiod_line_get_value(lineB);

        timestamp(tbuf, sizeof(tbuf));

        fprintf(log, "%s %s → %d\n", tbuf, nameA, valA);
        fprintf(log, "%s %s → %d\n", tbuf, nameB, valB);

        fflush(log);
        sleep(1);
    }

    gpiod_line_release(lineA);
    gpiod_line_release(lineB);
    gpiod_chip_close(chip);
    fclose(log);
    return 0;
}
