#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "math.h"
#define max(a, b) ((a) > (b) ? (a) : (b))
#define START_ADDRESS 0x43c00000
#define END_ADDRESS 0x43FB0000
#define STEP_SIZE 0x10000 // 假设步长为 0x10000
#define ARRAY_SIZE 60




const int M[3][3] = {
    {0, 1, 1},
    {0, 0, 0},
    {-1, 0, 1}
};

// 定义双端队列节点结构体
typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

// 定义双端队列结构体
typedef struct {
    Node* front; // 头部指针
    Node* rear;  // 尾部指针
    int size;    // 队列大小
} Deque;

// 初始化双端队列
void initDeque(Deque* deque) {
    deque->front = NULL;
    deque->rear = NULL;
    deque->size = 0;
}
void freeNode(Node* node) {
    free(node);
}

void freeDeque(Deque* deque) {
    // 释放队列中的每个节点
    Node* current = deque->front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        freeNode(temp);
    }
    // 释放队列结构体本身
    free(deque);
}



void insertFront(Deque* deque, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = deque->front;
    newNode->prev = NULL;

    if (deque->front == NULL) {
        deque->front = newNode;
        deque->rear = newNode;
    }
    else {
        deque->front->prev = newNode;
        deque->front = newNode;
    }

    deque->size++;
}


void insertRear(Deque* deque, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = deque->rear;

    if (deque->rear == NULL) {
        deque->front = newNode;
        deque->rear = newNode;
    }
    else {
        deque->rear->next = newNode;
        deque->rear = newNode;
    }

    deque->size++;
}


int deleteFront(Deque* deque) {
    if (deque->front == NULL) {
        printf("Deque is empty!\n");
        return -1;
    }

    int data = deque->front->data;
    Node* temp = deque->front;
    deque->front = deque->front->next;

    if (deque->front == NULL) {
        deque->rear = NULL;
    }
    else {
        deque->front->prev = NULL;
    }

    free(temp);
    deque->size--;
    return data;
}


int deleteRear(Deque* deque) {
    if (deque->rear == NULL) {
        printf("Deque is empty!\n");
        return -1;
    }

    int data = deque->rear->data;
    Node* temp = deque->rear;
    deque->rear = deque->rear->prev;

    if (deque->rear == NULL) {
        deque->front = NULL;
    }
    else {
        deque->rear->next = NULL;
    }

    free(temp);
    deque->size--;
    return data;
}

void printDeque(Deque* deque) {
    printf("Deque (size %d): ", deque->size);
    Node* current = deque->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}



Deque* transform_m(Deque* msd, int b, int bit_zero) {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    initDeque(deque);
    Node* current = msd->front; // 从队尾开始
    while (current != NULL) {
        insertRear(deque, M[b + 1][current->data + 1]);
        current = current->next;
    }
    if (bit_zero > 0) {
        insertRear(deque, 0);
        bit_zero--;
    }
    return deque;
}

Deque* decimalToBinary(long long n) {
    Deque* binaryDeque = (Deque*)malloc(sizeof(Deque));
    initDeque(binaryDeque);
    int positive = 0;
    if (n < 0) {
        positive = 1;
        n = -n;
    }
    while (n > 0) {
        int bit = n % 2;
        insertFront(binaryDeque, bit);
        n /= 2;
    }
    insertFront(binaryDeque, positive);
    return binaryDeque;
}

Deque* binaryToMSD(Deque* binaryDeque) {
    Deque* msdDeque = (Deque*)malloc(sizeof(Deque));
    initDeque(msdDeque);
    Node* current = binaryDeque->front;
    int is_positive = current->data;
    current = current->next;
    while (current != NULL) {
        if (is_positive) {
            insertRear(msdDeque, -current->data);
        }
        else {
            insertRear(msdDeque, current->data);
        }
        current = current->next;
    }
    freeDeque(binaryDeque);
    return msdDeque;
}

void decToBin(long long dec, int bin[])
{
    if (dec < 0)
    {
        dec *= -1;
        bin[0] = 1; // 0代表正，1代表负
    }
    else
    {
        bin[0] = 0;
    }
    long long num = dec;

    int temp[99];
    int i;
    for (i = 0; num != 0; i++)
    {
        temp[i] = num % 2; // 使用 % 运算符计算余数
        num /= 2;
    }
    for (int j = 1, index = i - 1; j <= i; j++, index--)
    {
        bin[j] = temp[index];
    }
    bin[i + 1] = -2; // 结束标志
}

void binToMsd(int bin[], int msd[])
{
    int index_msd = 0;
    if (bin[0] == 1)
    {
        for (int index_bin = 1; bin[index_bin] != -2; index_bin++, index_msd++)
        {
            msd[index_msd] = 0;
            if (bin[index_bin] == 1)
                msd[index_msd] = -1;
        }
        msd[index_msd] = -2; // 结束标志
    }
    else
    {
        for (int index_bin = 1; bin[index_bin] != -2; index_bin++, index_msd++)
        {
            msd[index_msd] = bin[index_bin];
        }
        msd[index_msd] = -2; // 结束标志
    }
}

int bin_to_dec(char bin[], int len)
{
    int result = 0, base = 1;
    for (int i = len - 1; i >= 0; i--, base = base * 2)
        result = result + (bin[i] - '0') * base;
    return result;
}

long long msdToDec(int msd[])
{
    long long result = 0;
    int len = 0;
    for (; msd[len] != -2; len++)
        ;

    for (int i = 0, j = len - 1; i < len; i++, j--)
    {
        long long t = 1;
        for (int k = 0; k < j; k++)
        {
            t *= 2;
        }
        result += msd[i] * t;
    }
    return result;
}

void convertToBinaryString(int* msd, char* result)
{
    int i = 0;
    while (msd[i] != -2)
    {
        if (msd[i] == 0)
        {
            strcat(result, "00");
        }
        else if (msd[i] == 1)
        {
            strcat(result, "01");
        }
        else if (msd[i] == -1)
        {
            strcat(result, "10");
        }
        i++;
    }
}

void binaryStringToMSD(char* binaryString, int* intArray, int length)
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

void cdecToBin(unsigned value, int bits, char* binaryArray)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        binaryArray[bits - 1 - i] = ((value >> i) & 1) + '0';
    }
    binaryArray[bits] = '\0'; // 添加 null 终止符
}

void write_reg(unsigned int* array, int* msd) {
    char z[67] = "";
    convertToBinaryString(msd, z);
    printf("convertToBinaryString success\n");
    int len_z = strlen(z);
    if (len_z <= 32)
    {
        array[0] = bin_to_dec(z, len_z);
    }
    else if (len_z > 32 && len_z <= 64)
    {
        array[0] = bin_to_dec(z + len_z - 32, 32);
        array[1] = bin_to_dec(z, len_z - 32);
    }
    else if (len_z > 64 && len_z <= 96)
    {
        array[0] = bin_to_dec(z + len_z - 32, 32);
        array[1] = bin_to_dec(z + len_z - 64, 32);
        array[2] = bin_to_dec(z, len_z - 64);
    }
}
void printUnsignedArray(unsigned int array[][3], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%u ", array[i][j]);
        }
        printf("\n");
    }
}

void regToDec(unsigned int decresult[][3], long long* ansDec, int index) {
    char ans_c[70] = "";
    cdecToBin(decresult[index][2], 6, ans_c);
    cdecToBin(decresult[index][1], 32, ans_c + 6);
    cdecToBin(decresult[index][0], 32, ans_c + 38);
    int msd_c[40];

    int length = strlen(ans_c) / 2; // Assuming the length is even
    binaryStringToMSD(ans_c, msd_c, length);
    msd_c[length] = -2;
    ansDec[index] = msdToDec(msd_c);
}


void printLongLongArray(long long array[], int length) {
    for (int i = 0; i < length; ++i) {
        printf("%lld ", array[i]);
    }
    printf("\n");
}

void printIntArray(int array[], int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int* dequeToArray(Deque* msdDeque) {
    int* msdArray = (int*)malloc(sizeof(int) * (msdDeque->size+1));
    Node* current = msdDeque->front;
    for (int i = 0; i < msdDeque->size; i++) {
        msdArray[i] = current->data;
        current = current->next;
    }
    freeDeque(msdDeque);
    msdArray[msdDeque->size]=-2;
    return msdArray;
}

int main() {
    long long m = -10;
    long long n = 5;
    Deque* binary_queue1 = decimalToBinary(m);
    Deque* binary_queue2 = decimalToBinary(n);
    Deque* msd_queue1 = binaryToMSD(binary_queue1);
    Deque* msd_queue2 = binaryToMSD(binary_queue2);
    int size = msd_queue2->size;
    Deque* partial_product[size];
    Node* current = msd_queue2->front;
    for (int i = msd_queue2->size - 1; i >= 0 && current != NULL; i--) {
        Deque* m_queue = transform_m(msd_queue1, current->data, i);
        partial_product[i] = m_queue;
        current = current->next;
    }
    for (int i = 0; i < msd_queue2->size; i++) {
        printDeque(partial_product[i]);
    }
//    printf("print  array success\n");
//
//    int k_reg[9];
//    for (int i = 0; i < 9; i++) {
//        k_reg[i] = i * 4;
//    }
//    printf("init  reg success\n");
//    printf("start transform array\n");
//    int* mid_msd = dequeToArray(partial_product[0]);
//    printf("end transform arra\n");
//    unsigned int decresult[3] = { 0 };
//    for (int i = 1; i < msd_queue2->size; i++) {
//        unsigned int reg_a[3] = { 0 };
//        unsigned int reg_b[3] = { 0 };
//        printf("start write reg\n");
//        printIntArray(mid_msd, 4);
//        write_reg(reg_a, mid_msd);
//        write_reg(reg_b, dequeToArray(partial_product[i]));
//        printf("over write reg\n");
//    }
    return 0;
}

