#ifndef CONFIG_H
#define CONFIG_H

#if defined Q_WS_MAC
#define MAC
#endif
#if defined Q_WS_WIN
#define WINDOWS
#endif
#if defined Q_WS_X11
#define LINUX
#endif


#define VERSION "1.2.20110726"

// #define TESTING

#endif // CONFIG_H
