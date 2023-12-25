#include <stdio.h>
#include <string.h>
#include <sys/time.h> // 添加 sys/time.h 头文件
#define MAXLEN 1000

typedef struct
{
    int len;
    int num[MAXLEN];
} bign;

// 将字符串转换为高精度数
bign change(char *str)
{
    bign a;
    a.len = strlen(str);
    for (int i = 0; i < a.len; i++)
    {
        a.num[i] = str[a.len - i - 1] - '0';
    }
    return a;
}

// 高精度加法
bign add(bign a, bign b)
{
    bign c;
    int carry = 0;
    c.len = 0;
    for (int i = 0; i < a.len || i < b.len; i++)
    {
        int temp = a.num[i] + b.num[i] + carry;
        c.num[c.len++] = temp % 10;
        carry = temp / 10;
    }
    if (carry)
    {
        c.num[c.len++] = carry;
    }
    return c;
}
// 打印结果
void print(bign a)
{
    for (int i = a.len - 1; i >= 0; i--)
    {
        printf("%d", a.num[i]);
    }
    printf("\n");
}
double get_time_difference(struct timeval start, struct timeval end)
{
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;
}

int main()
{
    char str1[MAXLEN], str2[MAXLEN];
    printf("Please enter parameter 1:\n");
    scanf("%s", str1);
    printf("Please enter parameter 2:\n");
    scanf("%s", str2);
    bign a = change(str1);
    bign b = change(str2);
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // 记录开始时间
    bign c = add(a, b);
    gettimeofday(&end_time, NULL); // 记录结束时间
    double time_taken = get_time_difference(start_time, end_time);
    printf("Time taken by add function: %f seconds\n", time_taken);
    print(c);
    return 0;
}