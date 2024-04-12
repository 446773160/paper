#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3

const int M[ROWS][COLS] = {
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


void insertFront(Deque* deque, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = deque->front;
    newNode->prev = NULL;

    if (deque->front == NULL) {
        deque->front = newNode;
        deque->rear = newNode;
    } else {
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
    } else {
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
    } else {
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
    } else {
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



Deque* transform_m(Deque* msd, int b) {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    initDeque(deque);
    Node* current = msd->front; // 从队尾开始
    while (current != NULL) {
        insertRear(deque, M[b+1][current->data + 1]);
        current = current->next;
    }
    return deque;
}

Deque* decimalToBinary(long long n) {
    Deque* binaryDeque=(Deque*)malloc(sizeof(Deque));
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
    Deque* msdDeque=(Deque*)malloc(sizeof(Deque));
    initDeque(msdDeque);
	Node* current = binaryDeque->front;
	int is_positive=current->data;
    current=current->next;
    while (current != NULL) {
		if (is_positive){
			insertRear(msdDeque,-current->data);
		}
		else{
			insertRear(msdDeque,current->data);	
		}
        current=current->next;
    }
    return msdDeque;
}


int main(){
    long long n=-10;
	Deque* binary_queue =decimalToBinary(n);
    printDeque(binary_queue);
    Deque* msd_queue=binaryToMSD(binary_queue);
	printDeque(msd_queue);
    Deque* m_queue=transform_m(msd_queue,0);
    printDeque(m_queue);
	return 0;
}

