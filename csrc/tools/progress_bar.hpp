// copy from https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf

#ifndef CSRC_TOOLS_PROGRESS_BAR
#define CSRC_TOOLS_PROGRESS_BAR

#include <cstdio>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void PrintProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [\033[32m%.*s%*s\033[0m]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

#endif /* CSRC_TOOLS_PROGRESS_BAR */
