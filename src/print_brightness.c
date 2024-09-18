#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "i3status.h"

#define STRING_SIZE 10

void print_brightness(brightness_ctx_t *ctx) {
    const char *format = ctx->format;
    char *outwalk = ctx->buf;

    int actual_brightness;
    int max_brightness;

    FILE *actual_ptr = fopen("/sys/class/backlight/intel_backlight/actual_brightness", "r");

    if (!actual_ptr) {
        OUTPUT_FULL_TEXT("can't read intel backlight brightness");
        fputs("i3status: Cannot read intel backlight brightness using /sys/class/backlight/intel_backlight/actual_brightness\n", stderr);
    }

    fscanf(actual_ptr, "%d", &actual_brightness);
    fclose(actual_ptr);

    FILE *max_ptr = fopen("/sys/class/backlight/intel_backlight/max_brightness", "r");

    if (!max_ptr) {
        OUTPUT_FULL_TEXT("can't read intel backlight max brightness");
        fputs("i3status: Cannot read intel backlight max brightness using /sys/class/backlight/intel_backlight/max_brightness\n", stderr);
    }

    fscanf(max_ptr, "%d", &max_brightness);
    fclose(max_ptr);

    int percentage = round(100.0 * actual_brightness / max_brightness);
    char string_percentage[STRING_SIZE];
    snprintf(string_percentage, STRING_SIZE, "%d%%", percentage);

    char string_errno[STRING_SIZE];
    sprintf(string_errno, "%d", errno);

    placeholder_t placeholders[] = {
        {.name = "%percentage", .value = string_percentage},
        {.name = "%errno", .value = string_errno},
        {.name = "%error", .value = strerror(errno)}};

    const size_t num = sizeof(placeholders) / sizeof(placeholder_t);
    char *formatted = format_placeholders(format, &placeholders[0], num);
    OUTPUT_FORMATTED;
    free(formatted);

    END_COLOR;
    OUTPUT_FULL_TEXT(ctx->buf);
}
