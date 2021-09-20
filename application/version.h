#ifndef VERSION_H_
#define VERSION_H_

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define __VERSION_TO_STR(s) #s
#define _VERSION_TO_STR(s) __VERSION_TO_STR(s)
#define VERSION_STR                                                            \
  "v"_VERSION_TO_STR(VERSION_MAJOR) "."_VERSION_TO_STR(VERSION_MINOR)

#endif // VERSION_H_
