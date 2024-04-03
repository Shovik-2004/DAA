#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

struct MinHeapNode *newNode(char data, unsigned freq) {
    struct MinHeapNode *temp = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap *createMinHeap(unsigned capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(minHeap->capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap *minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);

    printf("\n");
}

int isLeaf(struct MinHeapNode *root) {
    return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(char data[], unsigned freq[], int size) {
    struct MinHeap *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;

    return minHeap;
}

struct MinHeapNode *buildHuffmanTree(char data[], unsigned freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodes(struct MinHeapNode *root, int arr[], int top) {
    if (isLeaf(root)) {
        printf("|    '%c'    |    %3u     | ", root->data, root->freq);
        printArr(arr, top);
    }
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
}

void combineCodes(struct MinHeapNode *root, int arr[], int top, char *combinedCodes) {
    if (isLeaf(root)) {
        for (int i = 0; i < top; i++) {
            char codeChar = arr[i] + '0';
            strncat(combinedCodes, &codeChar, 1);
        }
    }
    if (root->left) {
        arr[top] = 0;
        combineCodes(root->left, arr, top + 1, combinedCodes);
    }
    if (root->right) {
        arr[top] = 1;
        combineCodes(root->right, arr, top + 1, combinedCodes);
    }
}

void HuffmanCodes(char data[], unsigned freq[], int size) {
    struct MinHeapNode *root = buildHuffmanTree(data, freq, size);
    int arr[100], top = 0;

    printf("| Character | Frequency | Huffman Code |\n");
    printf("|-----------|------------|--------------|\n");

    printCodes(root, arr, top);

    char combinedCodes[256] = "";
    combineCodes(root, arr, top, combinedCodes);
    printf("Combined Huffman Codes: %s\n", combinedCodes);
}

int main() {
    char inputString[101];

    printf("Enter a string (up to 100 letters): ");
    fgets(inputString, sizeof(inputString), stdin);

    unsigned freq[256] = {0};

    for (int i = 0; i < strlen(inputString); ++i) {
        if (inputString[i] != '\0' && inputString[i] != '\n') {
            freq[(int)inputString[i]]++;
        }
    }

    char characters[256];
    unsigned nonZeroFreq[256];
    int j = 0;

    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            characters[j] = (char)i;
            nonZeroFreq[j] = freq[i];
            j++;
        }
    }

    HuffmanCodes(characters, nonZeroFreq, j);

    return 0;
}
