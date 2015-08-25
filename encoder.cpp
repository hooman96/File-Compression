//  Author: Matthew Kotila & Hooman Mohammadi

#include "encoder.h"
#include <iostream>
#include <cstring>
#include "BinaryHeap.h"
#define EMPTY -1
using namespace std;


Encoder::Encoder()
{
} // Encoder()


Encoder::~Encoder()
{
} // ~Encoder()


class node {
public:
  node(): ascii(EMPTY), freq(0), left(NULL), right(NULL) {}
  short ascii;
  int freq;
  node * left;
  node * right;
};


void getEncoding(node * root, char * arr, int index, char codes[256][21]) {
  if (root->ascii != EMPTY)
    memcpy(&codes[root->ascii][0], arr, index);
  if (root->left)
  {
    arr[index] = '0';
    getEncoding(root->left, arr, index + 1, codes);
  }
  if (root->right)
  {
    arr[index] = '1';
    getEncoding(root->right, arr, index + 1, codes);
  }
}  // getEncoding() assembles the binary Huffman tree path to each unique ascii


int insertFreq(unsigned char * encodedMessage, int f, int start) {
  short digits = 0;
  for (int temp = f; temp > 0; temp /= 10, digits++);
  for (short temp = digits - 1; f > 0; temp--)
  {
    encodedMessage[start + temp] = (f % 10) + '0';
    f /= 10;
  }
  encodedMessage[start + digits] = ',';
  return start + digits + 1;
}  // insertFreq() converts freq to char array, reverses it, and returns updated index


int saveEncoding(node * leaves, unsigned char * encodedMessage, int * encodedSize) {
  int i = 0;
  for (int j = 0; j < 256; j++)
  {
    if (leaves[j].ascii != EMPTY)
    {
      encodedMessage[i] = leaves[j].ascii;
      i = insertFreq(encodedMessage, leaves[j].freq, i + 1);
    }
  }
  encodedMessage[i - 1] = ':';
  return i;
}  // saveEncoding() puts each unique ascii and its frequency at the front of encodedMessage


int getBitSize(node * root, char codes[256][21]) {
  if(root == NULL)
    return 0;
  if(!root->left && !root->right)
    return root->freq * (int)strlen(codes[root->ascii]);
  else
    return getBitSize(root->left, codes) + getBitSize(root->right, codes);
}


int insertBitSize(unsigned char *encodedMessage, int bitSize) {
  short digits = 0;
  for (int temp = bitSize; temp > 0; temp /= 10, digits++);
  for (short temp = digits - 1; bitSize > 0; temp--)
  {
    encodedMessage[temp] = (bitSize % 10) + '0';
    bitSize /= 10;
  }
  encodedMessage[digits] = ':';
  return digits + 1;
}


int inputEncoding(const unsigned char *message, const int size, unsigned char *encodedMessage, int *encodedSize, char codes[256][21]) {
  unsigned int buffer = 0;
  short bufferLeft = 8;
  int eSize = 0;
  for (int j = 0, k = 0; j < size; j++)
  {
    // iterate through code and assign to buffer
    char * code = codes[message[j]];
    for (int i = 0; i < (int)strlen(code); i++)
    {
      if (code[i] - '0')
      {
        buffer |= 1 << (bufferLeft - 1);
      }
      bufferLeft--;
      if (bufferLeft == 0)
      {
        eSize++;
        encodedMessage[k++] = buffer;
        buffer >>= 8;
        bufferLeft += 8;
      }
    }
    if (j == size - 1)
    {
      eSize++;
      encodedMessage[k++] = buffer;
    }
  }
  return eSize;
} // inputCodes() converts char array codes into bits and inserts into encodedMessage


void Encoder::encode(const unsigned char *message, const int size, unsigned char *encodedMessage, int *encodedSize) {
  // constructs leaves from message
  node leaves[256];
  for (int i = 0; i < size; i++)
  {
    int asciiVal = message[i];
    leaves[asciiVal].ascii = asciiVal;
    leaves[asciiVal].freq += 1;
  }
  
  // saves ascii values and frequencies at beginning of encoded message for tree-reconstruciton
  int start = saveEncoding(leaves, encodedMessage, encodedSize);
  
  // inserts leaves into heap based on frequency
  BinaryHeap<node*> heap;
  for (int i = 0; i < 256; i++)
    if (leaves[i].ascii != EMPTY)
      heap.insert(&leaves[i]);
  
  // constructs huffman tree based on frequency
  node * root = NULL;
  while (heap.currentSize > 1)
  {
    node * iNode = new node;
    iNode->left = new node;
    iNode->right = new node;
    *iNode->left = *heap.findMin(); heap.deleteMin();
    *iNode->right = *heap.findMin(); heap.deleteMin();
    iNode->freq = iNode->left->freq + iNode->right->freq;
    heap.insert(iNode);
    root = iNode;
  }
  
  // constructs encoding for each ascii
  char encoding[8];
  char codes[256][21] = { {0} };
  getEncoding(root, encoding, 0, codes);
  
  // calculates total bitSize
  int bitSize = getBitSize(root, codes);
  start += insertBitSize(encodedMessage + start, bitSize);
  
  // inserts bit-encoding to encodedMessage
  *encodedSize = inputEncoding(message, size, encodedMessage + start, encodedSize, codes);
}  // encode()
