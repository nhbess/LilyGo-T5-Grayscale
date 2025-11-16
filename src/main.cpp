#include <ePaperDriver.h>
#include "cellular_automaton.h"
#include "cellular_multy.h"
#include "display_utils.h"

ePaperDisplay *device = nullptr;
CellularAutomaton *ca = nullptr;
//MNCA *ca = nullptr;

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
    
    // Clean up existing CA if present
    if (ca != nullptr) {
        delete ca;
        ca = nullptr;
    }
    
    int width = device->width();
    int height = device->height();

    ca = new CellularAutomaton(width, height);
    //ca = new MNCA(width, height);

    // Run CA for 50 steps
    for (int i = 0; i < 50; i++) {
        ca->step();
        
        if (i % 10 == 0) {  // Print progress every 10 steps
            Serial.print("CA Step: ");
            Serial.println(i);
        }
    }
    
    // Plot the final state to the ePaper display
    Serial.println("Plotting CA state to ePaper display...");
    uint8_t* final_state = ca->state();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t cell_value = final_state[y * width + x];
            uint16_t color;
            
            // CA produces binary values (0 or 1)
            // Map to grayscale: 0 -> white, 1 -> black
            if (cell_value == 0) {
                color = ePaper_WHITE;
            } else {
                color = ePaper_BLACK;
            }
            
            device->drawPixel(x, y, color);
        }
    }
    
    device->refreshDisplay();
    Serial.println("CA display complete.");
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
    
}
