#ifndef UTILS_H
#define UTILS_H

#if DEBUG == 1
#define LOG(x) Serial.println("[DEBUG] " + String(x))
#else
#define LOG(x)
#endif

#endif
