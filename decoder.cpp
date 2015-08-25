//  Author: Matthew Kotila & Hooman Mohammadi

#include "decoder.h"
#include <iostream>
#include "BinaryHeap.h"
// constant
#define EMPTY -1
using namespace std;


Decoder::Decoder()
{
} // Decoder()


Decoder::~Decoder()
{
} // ~Decoder()


class node {
public:
  node(): ascii(EMPTY), freq(0), left(NULL), right(NULL) {}
  short ascii;
  int freq;
  node * left;
  node * right;
};


int outputDecoding(const unsigned char* encodedMessage, unsigned char* decodedMessage, node * root, int bitSize) {
  node * oldRoot = root;
  int dSize = 0;
  for (int i = 0, j = 0, k = 0; k < bitSize; i++)
  {
    for(int l = 128; l > 0 && k++ < bitSize; l >>= 1)
    {
      if(encodedMessage[i] & l)
      {
        root = root->right;
        if (root->ascii != EMPTY)
        {
          decodedMessage[j++] = root->ascii;
          dSize++;
          root = oldRoot;
        }
      }
      else
      {
        root = root->left;
        if (root->ascii != EMPTY)
        {
          decodedMessage[j++] = root->ascii;
          dSize++;
          root = oldRoot;
        }
      }
    }
  }
  return dSize;
}  // outputDecoding()


void Decoder::decode(const unsigned char* encodedMessage, const int encodedSize, 
  unsigned char* decodedMessage, int *decodedSize)
{
  // constructs leaves from encodedMessage
  node leaves[256];
  int start = 0;
  int bitSize = 0;
  for (; ; )
  {
    int asciiVal = encodedMessage[start];
    leaves[asciiVal].ascii = asciiVal;
    leaves[asciiVal].freq = encodedMessage[start + 1] - '0';
    int j = start + 2;
    for (; encodedMessage[j] != ',' && encodedMessage[j] != ':'; j++)
    {
      leaves[asciiVal].freq *= 10;
      leaves[asciiVal].freq += (encodedMessage[j] - '0');
    }
    start = j + 1;
    if (encodedMessage[start - 1] == ':')
    {
      bitSize = encodedMessage[start] - '0';
      int k = start + 1;
      for (; encodedMessage[k] != ':'; k++)
      {
        bitSize *= 10;
        bitSize += (encodedMessage[k] - '0');
       }
      start = k + 1;
      break;
    }
  }
  
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
  
  // traverses tree to fill decodedMessage with original message
  *decodedSize = outputDecoding(encodedMessage + start, decodedMessage, root, bitSize);
} // decode()
