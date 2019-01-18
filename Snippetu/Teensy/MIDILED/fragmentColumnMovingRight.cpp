#include "fragmentColumnMovingRight.h"

ColumnMovingRight::ColumnMovingRight(LedUtils ledUtils, unsigned long time) : 
	ledUtils(ledUtils) {

    this->time = time;
    setColorProperties();
    setAnimationProperties();
}

void ColumnMovingRight::setColorProperties(uint8_t hue, uint8_t sat, uint8_t bri, double hueIncrement) {
    this->hue = hue;
    this->sat = sat;
    this->bri = bri;
    this->hueIncrement = hueIncrement;
}

void ColumnMovingRight::setAnimationProperties(int length, int start, int wait, int fade) {
    this->wait = wait;
    this->fade = fade;

    this->colCountLocal = colCount;
    if (length!=0) {
        this->colCountLocal = length;
    }
    this->currentCol = start;
}

void ColumnMovingRight::resetTimer(unsigned long time) {
    this->time = time;
}

void ColumnMovingRight::nextFrame(unsigned long currentTime, vector<PixelUpdate> &matrixUpdate, bool &animationFinished) {
    if (DEBUG) Serial.println((String) "nextFrame: currentTime " + currentTime + " > time " + this->time + " + wait " + this->wait + " = " + (this->time+this->wait) + " ... t/f " + (currentTime >= this->time+this->wait));

    // check if defined wait is over
    if (currentTime >= this->time+this->wait) {
        this->time = currentTime;
        // paint next frame
        if (this->currentCol<this->colCountLocal) {
            for (int currentRow=0; currentRow<rowCount; currentRow++) {
                if (this->hueIncrement > 0) hue = ledUtils.incrementHue(this->hue, this->hueIncrement);

                PixelUpdate onePixelUpdate;
                onePixelUpdate.col = this->currentCol;
                onePixelUpdate.row = currentRow;
                onePixelUpdate.hue = hue;
                onePixelUpdate.sat = this->sat;
                onePixelUpdate.bri = this->bri;
                onePixelUpdate.fade = this->fade;
                onePixelUpdate.time = currentTime;
                matrixUpdate.push_back(onePixelUpdate);
            }
            currentCol++;
        }
        else {
            animationFinished = true;
        }
    }
}