// Author: Sean Davis
#ifndef ENCODER_H
#define	ENCODER_H


class Encoder
{
public:
  Encoder();
  void encode(const unsigned char *message, const int size, 
    unsigned char *encodedMessage, int *encodedSize);
  ~Encoder();
private:

};

#endif	/* ENCODER_H */

