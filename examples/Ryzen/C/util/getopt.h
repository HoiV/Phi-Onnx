// Copyright © 2024 Advanced Micro Devices, Inc. All rights reserved.

#ifndef GETOPT_H
#define GETOPT_H
extern int opterr, optind, optopt, optreset;
extern char *optarg;
int getopt(int nargc, char *const nargv[], const char *ostr);
#endif