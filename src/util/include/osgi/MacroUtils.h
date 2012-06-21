
#pragma once
#ifndef OSGI_MACROUTILS_H_
#define OSGI_MACROUTILS_H_

#define OSGI_JOIN(x, y) OSGI_JOIN_AGAIN(x, y)
#define OSGI_JOIN_AGAIN(x, y) x ## y

#define OSGI_XSTR(x) OSGI_STR(x)
#define OSGI_STR(x) #x

#endif // OSGI_MACROUTILS_H_
