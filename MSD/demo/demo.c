#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 将无符号整数转换为二进制字符串
void cdecToBin(unsigned value, int bits, char *binaryArray)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        binaryArray[bits - 1 - i] = ((value >> i) & 1) + '0';
    }
    binaryArray[bits] = '\0'; // 添加 null 终止符
}

void binaryStringToMSD(char *binaryString, int *intArray, int length)
{
    for (int i = 0; i < length; i++)
    {
        char tmp[3];
        strncpy(tmp, binaryString + i * 2, 2);
        tmp[2] = '\0';

        if (strcmp(tmp, "00") == 0)
        {
            intArray[i] = 0;
        }
        else if (strcmp(tmp, "01") == 0)
        {
            intArray[i] = 1;
        }
        else if (strcmp(tmp, "10") == 0)
        {
            intArray[i] = -1;
        }
        // Add more conditions as needed
    }
}

int *removeLeadingZeros(int *arr)
{
    int startIndex = 0;

    // 找到第一个非0元素的位置
    while (arr[startIndex] != -2 && arr[startIndex] == 0)
    {
        startIndex++;
    }
    int endIndex = startIndex;

    while (arr[endIndex] != -2)
    {
        endIndex++;
    }

    // 计算新数组的大小
    int newSize = endIndex - startIndex + 1;

    // 分配新数组的内存
    int *newArr = (int *)malloc(newSize * sizeof(int));
    if (newArr == NULL)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // 复制元素到新数组
    for (int i = 0; i < newSize - 1; i++)
    {
        newArr[i] = arr[startIndex + i];
    }
    newArr[newSize - 1] = -2;
    return newArr;
}

int main()
{
    int array[36] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, -2};
    int *new_array = removeLeadingZeros(array);
    for (int i = 0; new_array[i] != -2; i++)
    {
        printf("%d ", new_array[i]);
    }
    return 0;
}
