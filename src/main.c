/**
 * Copyright (c) 2022 Yanderemine54
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <libcpuid/libcpuid.h>
#include "colors.h"

#define BUFFER_SIZE 1024

const char* is_intel_or_amd(const char* vendor_str) {
    if (strncmp(vendor_str, "GenuineIntel", 13) == 0) {
        return FG_BLUE;
    } else {
        return FG_RED;
    }
}

int main(int argc, const char* argv[]) {
    if (!cpuid_present()) {
        printf("Sorry, your cpu isn\'t supported.\n");
        return EXIT_FAILURE;
    }

    struct cpu_raw_data_t raw;
    struct cpu_id_t data;
    struct sysinfo info;
    char gpu_info_buffer[BUFFER_SIZE];
    FILE* gpu_cmd_output;
    char* gpu_cmd = "lspci | grep VGA | cut -d \" \" -f5-";
    struct utsname uname_info;
    char* distro_cmd = "cat /etc/issue | cut -d \" \" -f1,2 | tr -d \"\\n\"";
    FILE* distro_cmd_output;
    char distro_buffer[BUFFER_SIZE];

    if ((distro_cmd_output = popen(distro_cmd, "r")) == NULL) {
        printf("Error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    while (fgets(distro_buffer, BUFFER_SIZE, distro_cmd_output) != NULL) {
        printf("%sOS%s:     %s\n", FG_CYAN, RESET, distro_buffer);
    }
    pclose(distro_cmd_output);

    if (uname(&uname_info) < 0) {
        printf("Sorry, cannot get the kernel information.\n");
        return EXIT_FAILURE;
    }
    printf("%sKernel%s: %s %s\n", FG_YELLOW, RESET, uname_info.sysname, uname_info.release);

    if (cpuid_get_raw_data(&raw) < 0) {
        printf("Sorry, cannot get the CPUID raw data.\n");
		printf("Error: %s\n", cpuid_error());
		return EXIT_FAILURE;
    }

    if (cpu_identify(&raw, &data) < 0) {
        printf("Sorrry, CPU identification failed.\n");
		printf("Error: %s\n", cpuid_error());
		return EXIT_FAILURE;
    }

    printf("%sCPU%s:    %s\n", is_intel_or_amd(data.vendor_str), RESET, data.brand_str);

    if ((gpu_cmd_output = popen(gpu_cmd, "r")) == NULL) {
        printf("Sorry, cannot get the GPU information.\n");
        return EXIT_FAILURE;
    }
    while (fgets(gpu_info_buffer, BUFFER_SIZE, gpu_cmd_output) != NULL) {
        printf("%sGPU%s:    %s", FG_MAGENTA, RESET, gpu_info_buffer);
    }
    pclose(gpu_cmd_output);

    if (sysinfo(&info) < 0) {
        printf("Sorry, cannot get the system information.\n");
        return EXIT_FAILURE;
    }
    sysinfo(&info);
    printf("%sRAM%s:    %lu MiB free / %lu MiB total\n", FG_GREEN, RESET, (info.freeram / 1024) / 1024, (info.totalram / 1024) / 1024);
    printf("\n\n");
    printf("    %s   %s   %s   %s   %s   %s   %s   %s   %s\n", BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE, RESET);
    printf("    %s   %s   %s   %s   %s   %s   %s   %s   %s\n", BG_BRIGHT_BLACK, BG_BRIGHT_RED, BG_BRIGHT_GREEN, BG_BRIGHT_YELLOW, BG_BRIGHT_BLUE, BG_BRIGHT_MAGENTA, BG_BRIGHT_CYAN, BG_BRIGHT_WHITE, RESET);
    return EXIT_SUCCESS;
}