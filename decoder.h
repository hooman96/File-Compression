// Author: Sean Davis
#ifndef DECODER_H
#define	DECODER_H


class Decoder
{
public:
  Decoder();
  ~Decoder();
  void decode(const unsigned char* encodedMessage, const int encodedSize, 
    unsigned char* decodedMessage, int *decodedSize);
private:

};

#endif	/* DECODER_H */

