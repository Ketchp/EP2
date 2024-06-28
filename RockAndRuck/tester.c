#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/capability.h>


int main(int argc, char *argv[]) {
    cap_t curr = cap_get_proc();
    if(curr == NULL)
        return EXIT_FAILURE;

    cap_value_t new_caps[] = {CAP_SYS_NICE};
    if(cap_set_flag(curr, CAP_INHERITABLE, 1, new_caps, CAP_SET) != 0) {
        cap_free(curr);
        return EXIT_FAILURE;
    }

    if(cap_set_proc(curr) != 0) {
        cap_free(curr);
        return EXIT_FAILURE;
    }

    cap_free(curr);

    if(cap_set_ambient(CAP_SYS_NICE, CAP_SET))
        return EXIT_FAILURE;

    const char **new_argv = (const char **)malloc((argc + 1) * sizeof(*new_argv));
    if(new_argv == NULL)
        return EXIT_FAILURE;

    for(int i = 0; i < argc; i++)
        new_argv[i] = argv[i];
    new_argv[argc] = NULL;

    execvp("./tester.py", argv);

    free((void *)new_argv);
    return EXIT_FAILURE;
}
