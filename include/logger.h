#ifndef LOGGER_H
#define LOGGER_H

#ifdef DEBUG
#define FILELINE() \
    printf("@ %s:%d\n", __FILE__, __LINE__)
#else 
#define FILELINE()
#endif // DEBUG

#endif // LOGGER_H
