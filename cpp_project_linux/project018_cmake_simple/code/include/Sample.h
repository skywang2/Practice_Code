#ifndef SAMPLE_H
#define SAMPLE_H

// 返回较小值
int min(int a, int b);

template <typename T>
T min(const T &l, const T &r)
{
    return (l <= r) ? l : r;
}

#endif
