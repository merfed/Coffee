#ifndef COLORS_H
#define COLORS_H

#ifdef _WIN32//Disable colors, as Win32 console don't support them
#define KNRM  ""//Normal
#define KRED  ""//Red
#define KGRN  ""//Green
#define KYEL  ""//Yellow
#define KBLU  ""//Blue
#define KMAG  ""//Magenta
#define KCYN  ""//Cyan
#define KWHT  ""//White
#define RESET ""//Reset

#else
#define KNRM  "\x1B[0m"//Normal
#define KRED  "\x1B[31m"//Red
#define KGRN  "\x1B[32m"//Green
#define KYEL  "\x1B[33m"//Yellow
#define KBLU  "\x1B[34m"//Blue
#define KMAG  "\x1B[35m"//Magenta
#define KCYN  "\x1B[36m"//Cyan
#define KWHT  "\x1B[37m"//White
#define RESET "\033[0m"//Reset
#endif

#endif
