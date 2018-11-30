#include "fragmentColumnMovingRight.h"

ColumnMovingRight::ColumnMovingRight(LedUtils ledUtils) : 
	ledUtils(ledUtils) {

    setColorProperties();
    setAnimationProperties();
}

void ColumnMovingRight::setColorProperties(double hue=0, int sat=0, int bri=255, double hueIncrement=0) {
    this->hue = hue;
    this->sat = sat;
    this->bri = bri;
    this->hueIncrement = hueIncrement;
}

void ColumnMovingRight::setAnimationProperties(int length=0, int start=0, int wait=10, int fade=250) {
    this->wait = wait;
    this->fade = fade;

    this->colCountLocal = colCount;
    if (length!=0) {
        this->colCountLocal = length;
    }
    this->currentCol = start;
}

std::vector<PixelUpdate> ColumnMovingRight::nextFrame() {
    
    std::vector<PixelUpdate> matrixUpdate;

    if (currentCol<colCountLocal) {
        for (int currentRow=0; currentRow<rowCount; currentRow++) {
            // Todo hueIncrement not working??
            if (hueIncrement > 0) hue = ledUtils.incrementHue(hue, hueIncrement);

            PixelUpdate onePixelUpdate;
            onePixelUpdate.col = currentCol;
            onePixelUpdate.row = currentRow;
            onePixelUpdate.hue = hue;
            onePixelUpdate.sat = sat;
            onePixelUpdate.bri = bri;
            onePixelUpdate.fade = fade;
            matrixUpdate.push_back(onePixelUpdate);
        }
    //     delay(wait);    // ToDo: calculate time since last time
        currentCol++;
    }

    return matrixUpdate;
}