#include <ePaperDriver.h>
#include "cellular_automaton.h"
#include "display_utils.h"

ePaperDisplay *device = nullptr;
CellularAutomaton *ca = nullptr;

#define SPI_MOSI            (23)
#define SPI_MISO            (-1)
#define SPI_CLK             (18)

#define ELINK_SS            (5)
#define ELINK_BUSY          (4)
#define ELINK_RESET         (16)
#define ELINK_DC            (17)

#define SDCARD_SS           (13)
#define SDCARD_CLK          (14)
#define SDCARD_MOSI         (15)
#define SDCARD_MISO         (2)

#define BUTTON_PIN          (39)








void run_and_plot_ca(ePaperDisplay* device) {

    Serial.println("Initializing CA...");
    
    int width = device->width();
    int height = device->height();

    ca = new CellularAutomaton(width, height);


    for (int i = 0; i < 100; i++) {  // Run for 100 steps
        uint8_t* old_state = ca->state();
        uint8_t* old_copy = new uint8_t[width * height];
        memcpy(old_copy, old_state, width * height);

        uint8_t* new_state = ca->step();

        Serial.print("CA Step: ");
        Serial.println(i);

        // Update display - only changed pixels
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;
                if (old_copy[idx] != new_state[idx]) {
                    device->drawPixel(x, y, new_state[idx] ? ePaper_BLACK : ePaper_WHITE);
                }
            }
        }
    
        //device->refreshDisplay();
        delete[] old_copy;

    }
}





void setup()
{
    Serial.begin(115200);
    Serial.println("Starting 4-color grayscale demo...");
    
    device = new ePaperDisplay(GDEW0213T5, ELINK_BUSY, ELINK_RESET, ELINK_DC, ELINK_SS);
    
    // Clear the display first
    device->clearDisplay();
    device->refreshDisplay();

    // Display example badge
    display_name(device, "John Doe", "Software Engineer", "Tech Corp");
}

void loop()
{
    // Enter light sleep to save energy while waiting for button
    Serial.println("Going to light sleep. Press button to wake.");
    
    // Configure wake up on button press
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, LOW);
    
    // Enter light sleep (wakes up instantly, continues from here)
    esp_light_sleep_start();
    
    // When button is pressed, we wake up here!
    Serial.println("Button pressed! Woke from light sleep.");
    delay(200); // Debounce
    
    device->setRotation(1);
    int width = device->width();
    int height = device->height();
    
    //CA Logic

    run_and_plot_ca(device);
    



    // End CA Logic
    
    
    
    
    // Wait for button release
    while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
    }
    
    // Loop back to light sleep
}
