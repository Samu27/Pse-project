#include "Xtea.hh"

// FSM
void Xtea::fsm()
{
    NEXT_STATUS = STATUS;
    switch (STATUS) {
      case START:
          NEXT_STATUS = INITIAL;
          break;
      
      case INITIAL:
          if (input_ready.read() == 1)
              NEXT_STATUS = ASSIGN;
          else
              NEXT_STATUS = INITIAL;
          break;
      
      case ASSIGN:
          if (mode.read() == 0)
              NEXT_STATUS = ENCRYPT_1;
          else if (mode.read() == 1)
              NEXT_STATUS = DECRYPT_1;
          break;
      
      case ENCRYPT_1:
          NEXT_STATUS = ENCRYPT_2;
          break;
      
      case ENCRYPT_2:
          if (counter.read() == 31)
              NEXT_STATUS = TERM;
          else
              NEXT_STATUS = ENCRYPT_1;
          break;
      
      case DECRYPT_1:
          NEXT_STATUS = DECRYPT_2;
          break;
      
      case DECRYPT_2:
          if (counter.read() == 31)
              NEXT_STATUS = TERM;
          else
              NEXT_STATUS = DECRYPT_1;
          break;
      
      case TERM:
          NEXT_STATUS = INITIAL;
          break;
    }
}


// Datapath
void Xtea::datapath()
{
    if (rst.read() == 1) {
        STATUS = START;
    } else if (clk.read() == 1) {
      STATUS = NEXT_STATUS;
      
      switch (STATUS) {
        case START:
            output_ready.write(0);
            output0.write(0);
            output1.write(0);
            delta.write(0x9e3779b9);
            break;

        case INITIAL:
            output_ready.write(0);
            counter.write(0);
            word0_temp.write(0);
            word1_temp.write(0);
            break;
        
        case ASSIGN:
            word0_temp.write(word0.read());
            word1_temp.write(word1.read());
            k1.write(key1.read());
            k2.write(key2.read());
            k3.write(key3.read());
            k4.write(key4.read());

            if (mode.read() == 0) {
              sum.write(0);
            } else {
              sum.write(delta.read() * 32);
            }
            break;
        
        // Encryption, first part
        case ENCRYPT_1:
            encryption1();
            break;

        // Encryption, second part
        case ENCRYPT_2:
            encryption2();
            break;

        // Decryption, first part
        case DECRYPT_1:
            decryption1();
            break;

        // Dencryption, second part
        case DECRYPT_2:
            decryption2();
            break;
        
        case TERM:
            output0.write(word0_temp.read());
            output1.write(word1_temp.read());
            output_ready.write(1);
            break;
      }
    }
}


void Xtea::encryption1() {
  if ((sum.read() & 3) == 0) {
    word0_temp.write(word0_temp.read() + ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k1.read())));
  } else if ((sum.read() & 3) == 1) {
    word0_temp.write(word0_temp.read() + (((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k2.read()))));
  } else if ((sum.read() & 3) == 2) {
    word0_temp.write(word0_temp.read() + ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k3.read())));
  } else {
    word0_temp.write(word0_temp.read() + ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k4.read())));
  }

  sum.write(sum.read() + delta.read());
}

void Xtea::encryption2() {
  if ((sum.read() >> 11 & 3) == 0) {
    word1_temp.write(word1_temp.read() + ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k1.read())));
  } else if ((sum.read() >> 11 & 3) == 1) {
    word1_temp.write(word1_temp.read() + (((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k2.read()))));
  } else if ((sum.read() >> 11 & 3) == 2) {
    word1_temp.write(word1_temp.read() + ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k3.read())));
  } else {
    word1_temp.write(word1_temp.read() + ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k4.read())));
  }

  counter.write(counter.read() + 1);
}

void Xtea::decryption1() {
  if ((sum.read() >> 11 & 3) == 0) {
    word1_temp.write(word1_temp.read() - ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k1.read())));
  } else if ((sum.read() >> 11 & 3) == 1) {
    word1_temp.write(word1_temp.read() - (((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k2.read()))));
  } else if ((sum.read() >> 11 & 3) == 2) {
    word1_temp.write(word1_temp.read() - ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k3.read())));
  } else {
    word1_temp.write(word1_temp.read() - ((((word0_temp.read() << 4) ^ (word0_temp.read() >> 5)) +
                    word0_temp.read()) ^ (sum.read() + k4.read())));
  }

  sum.write(sum.read() - delta.read());
}

void Xtea::decryption2() {
  if ((sum.read() & 3) == 0) {
    word0_temp.write(word0_temp.read() - ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k1.read())));
  } else if ((sum.read() & 3) == 1) {
    word0_temp.write(word0_temp.read() - ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k2.read())));
  } else if ((sum.read() & 3) == 2) {
    word0_temp.write(word0_temp.read() - ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k3.read())));
  } else {
    word0_temp.write(word0_temp.read() - ((((word1_temp.read() << 4) ^ (word1_temp.read() >> 5)) +
                    word1_temp.read()) ^ (sum.read() + k4.read())));
  }
  
  counter.write(counter.read() + 1);
}