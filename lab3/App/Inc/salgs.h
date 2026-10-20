// header for additional statistic algorithms
#ifndef SALGS_H
#define SALGS_H
#include <cstdint>

inline uint16_t median_u16(const uint16_t *src, uint32_t n)
{
    // max input src length, do not exceed
    uint16_t a[256];
    if (n == 0) return 0;
    for (uint32_t i = 0; i < n; i++) a[i] = src[i];

    const int32_t k = (int32_t)(n / 2);
    int32_t l = 0, m = (int32_t)n - 1;

    while (l < m)
    {
        uint16_t x = a[k];
        int32_t  i = l, j = m;
        do {
            while (a[i] < x) i++;
            while (x < a[j]) j--;
            if (i <= j) {
                uint16_t t = a[i]; a[i] = a[j]; a[j] = t;
                i++; j--;
            }
        } while (i <= j);
        if (j < k) l = i;
        if (k < i) m = j;
    }

    if (n & 1U) return a[k];

    uint16_t lo = a[0];
    for (int32_t i = 1; i < k; i++) if (a[i] > lo) lo = a[i];
    return (uint16_t)((lo + a[k] + 1U) / 2U);
}


#endif // SALGS_H
