#ifndef FRAGMENTCOLUMNMOVINGRIGHT
#define FRAGMENTCOLUMNMOVINGRIGHT

#include "fragment.h"

class ColumnMovingRight : public Fragment
{
    private:
        // shape
        int lengthLocal, colCountLocal, currentCol, currentLed, lengthCounter;
        LedUtils ledUtils;
        unsigned long time;
        bool reverse;
    public:
        ColumnMovingRight(LedUtils ledUtils, unsigned long time);
        virtual void setAnimationProperties(int wait=10, int fade=250, bool reverse=false, int length=0, int start=0);
        void setColorProperties(uint8_t hue=0, uint8_t sat=0, uint8_t bri=255, double hueIncrement=0);
        void nextFrame(unsigned long currentTime, vector<PixelUpdate> &matrixUpdate, int &animationRunning);
        void resetTimer(unsigned long time);
        String getValues();
};

#endif 
