#include "display_utils.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

void display_name(ePaperDisplay* device, String name, String position, String affiliation) {
    // Set landscape orientation
    device->setRotation(1);
    
    // Clear display
    device->clearDisplay();
    
    // Get display dimensions (after rotation)
    int width = device->width();
    int height = device->height();
    
    // Calculate vertical spacing
    int lineHeight = height / 3;
    
    // Line 1: Name (White background, Black text, Bold font)
    device->fillRect(0, 0, width, lineHeight, ePaper_WHITE);
    device->setFont(&FreeSansBold12pt7b);
    device->setTextColor(ePaper_BLACK);
    int16_t x1, y1;
    uint16_t w, h;
    device->getTextBounds(name, 0, 0, &x1, &y1, &w, &h);
    device->setCursor((width - w) / 2, lineHeight / 2 + h / 2);
    device->print(name);
    
    // Line 2: Position (Light Gray background, Black text)
    device->fillRect(0, lineHeight, width, lineHeight, ePaper_GRAY1);
    device->setFont(&FreeSans9pt7b);
    device->setTextColor(ePaper_BLACK);
    device->getTextBounds(position, 0, 0, &x1, &y1, &w, &h);
    device->setCursor((width - w) / 2, lineHeight + lineHeight / 2 + h / 2);
    device->print(position);
    
    // Line 3: Affiliation (Dark Gray background, White text)
    // Fill to the exact bottom to avoid pixel line issues
    device->fillRect(0, lineHeight * 2, width, height - (lineHeight * 2), ePaper_GRAY2);
    device->setFont(&FreeSans9pt7b);
    device->setTextColor(ePaper_WHITE);
    device->getTextBounds(affiliation, 0, 0, &x1, &y1, &w, &h);
    device->setCursor((width - w) / 2, lineHeight * 2 + lineHeight / 2 + h / 2);
    device->print(affiliation);
    
    // Refresh the display
    Serial.println("Updating display...");
    device->refreshDisplay();
    Serial.println("Display updated!");
}
