class NBOX_API {
  public:
    void setup(int clk, int stb, int dg) {
      this->clk = clk;
      this->stb = stb;
      this->dg = dg;
      pinMode(clk, OUTPUT);
      pinMode(dg, OUTPUT);
      pinMode(stb, OUTPUT);
      digitalWrite(stb, LOW);
      digitalWrite(dg, LOW);
      digitalWrite(clk, HIGH);
      delay(200);
    }

    void printClock(String clock) {
      segmentPrint(clock, 0, 1, 0, 0);
    }

    void setPowerLed(bool state) {
      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 1, 0, 0); //command 1
      digitalWrite(stb, HIGH);
      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 0, 0, 1); //command 2 // led power
      msg(0, 0, 0, 0, 0, 0, state, 0); //data
      digitalWrite(stb, HIGH);
    }

    void ledBrightness(bool onOff, uint8_t percent) {
      uint8_t power = map(percent, 0, 100, 0, 7);
      digitalWrite(stb, LOW);
      msg(1, 0, 0, 0, onOff, (power & (1 << 2)), (power & (1 << 1)), (power & (1 << 0))); //command 3 // swieci slabo
      digitalWrite(stb, HIGH);
    }

    void setAtLed(bool state) {
      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 1, 0, 0); //command 1
      digitalWrite(stb, HIGH);
      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 1, 0, 1); //command 2 // led malpa
      msg(0, 0, 0, 0, 0, 0, 0, state); //data
      digitalWrite(stb, HIGH);
    }

    void setAlarmLed(bool state) {
      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 1, 0, 0); //command 1
      digitalWrite(stb, HIGH);
      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 1, 1, 1); //command 2 // led wykrzyknik
      msg(0, 0, 0, 0, 0, 0, 0, state); //data
      digitalWrite(stb, HIGH);
    }

    void setClockLed(bool state) {
      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 1, 0, 0); //command 1
      digitalWrite(stb, HIGH);
      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 0, 1, 1); //command 2 // led zegar
      msg(0, 0, 0, 0, 0, 0, 0, state); //data
      digitalWrite(stb, HIGH);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void segmentPrint(String s, bool dot1, bool dot2, bool dot3, bool dot4) {
      uint8_t s1 = charToBit(s.charAt(0));
      uint8_t s2 = charToBit(s.charAt(1));
      uint8_t s3 = charToBit(s.charAt(2));
      uint8_t s4 = charToBit(s.charAt(3));

      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 1, 0, 0); //command 1
      digitalWrite(stb, HIGH);

      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 0, 0, 0); //command 2 //digit 1
      msg(dot1, (s1 & (1 << 0)), (s1 & (1 << 1)), (s1 & (1 << 2)), (s1 & (1 << 3)), (s1 & (1 << 4)), (s1 & (1 << 5)), (s1 & (1 << 6))); //data   - pierwsza cyfra
      digitalWrite(stb, HIGH);

      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 0, 1, 0); //command 2 // digit 2
      msg(dot2, (s2 & (1 << 0)), (s2 & (1 << 1)), (s2 & (1 << 2)), (s2 & (1 << 3)), (s2 & (1 << 4)), (s2 & (1 << 5)), (s2 & (1 << 6))); //data
      digitalWrite(stb, HIGH);

      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 1, 0, 0); //command 2 // digit 3
      msg(dot3, (s3 & (1 << 0)), (s3 & (1 << 1)), (s3 & (1 << 2)), (s3 & (1 << 3)), (s3 & (1 << 4)), (s3 & (1 << 5)), (s3 & (1 << 6))); //data
      digitalWrite(stb, HIGH);

      digitalWrite(stb, LOW);
      msg(1, 1, 0, 0, 0, 1, 1, 0); //command 2 // digit 4
      msg(dot4, (s4 & (1 << 0)), (s4 & (1 << 1)), (s4 & (1 << 2)), (s4 & (1 << 3)), (s4 & (1 << 4)), (s4 & (1 << 5)), (s4 & (1 << 6))); //data
      digitalWrite(stb, HIGH);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void notifyWhenButtonChange() {
      static int buttonState = 0;
      digitalWrite(stb, LOW);
      msg(0, 1, 0, 0, 0, 0, 1, 0); //command 1 // read

      int tempButtonState = msg_read();

      if (tempButtonState != buttonState) {
        buttonState = tempButtonState;
        Serial.print("KEY_PAD:");
        Serial.println(buttonState);
      }
      digitalWrite(stb, HIGH);
    }
  private:
    int clk; int stb; int dg;
    uint8_t charToBit(char a) {
      switch (a) {
        case '0': return 0b1111110;  //  ABCDEFG  Segments      7-segment map:
        case '1': return 0b0110000;  //                           AAA
        case '2': return 0b1101101;  //                          F   B
        case '3': return 0b1111001;  //                          F   B
        case '4': return 0b0110011;  //                           GGG
        case '5': return 0b1011011;  //                          E   C
        case '6': return 0b1011111;  //                          E   C
        case '7': return 0b1110000;  //                           DDD  DP
        case '8': return 0b1111111;
        case '9': return 0b1111011;
        case '-': return 0b0000001;
        case 'a': return 0b1110111;
        case 'b': return 0b0011111;
        case 'c': return 0b1001110;
        case 'd': return 0b0111101;
        case 'e': return 0b1001111;
        case 'f': return 0b1000111;
        default: return 0b0000000;
      }
    }
    void msg(bool b7, bool b6, bool b5, bool b4, bool b3, bool b2, bool b1, bool b0) {
      digitalWrite(clk, LOW);
      digitalWrite(dg, b0);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b1);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b2);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b3);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b4);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b5);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b6);
      digitalWrite(clk, HIGH);

      digitalWrite(clk, LOW);
      digitalWrite(dg, b7);
      digitalWrite(clk, HIGH);
    }

    int msg_read() {
      int pins = 0;
      pinMode(dg, INPUT);

      for (char i = 0; i < 12; i++) {
        digitalWrite(clk, LOW);
        digitalWrite(clk, HIGH);
        bitWrite(pins, i, digitalRead(dg));
      }
      pinMode(dg, OUTPUT);
      return pins;
    }

};
