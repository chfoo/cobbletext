#include <stdio.h>

#include "example_app.h"

int main(int argc, char const *argv[]) {
    printf("Start\n");

    ExampleApp app = {NULL};

    example_app_run(&app);

    printf("Done\n");

    return 0;
}
