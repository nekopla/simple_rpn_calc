#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MAX_DISP_LEN  10

int i, j;
int pin_col[] = {5, 4, 0, 1};
int pin_row[] = {9, 8, 7, 6};

float x, y, z, t;
String x_disp;
int enter_flg = 0;
int operator_flg = 0;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.setTextSize(2.3);
    display.setCursor(4, 7);
    display.clearDisplay();
    display.print("0.00");
    display.display();

    for (i = 0; i < 4; i++) {
        pinMode(pin_row[i], OUTPUT);
        digitalWrite(pin_row[i], HIGH);
        pinMode(pin_col[i], INPUT_PULLUP);
    }

    x = y = z = t = 0;
    x_disp = "";
}

String key_scan() {
    
    for (j = 0; j < 4; j++) {
        digitalWrite(pin_row[j], HIGH);
    }

    for (j = 0; j < 4; j++) {
        digitalWrite(pin_row[j], LOW);

        for (i = 0; i < 4; i++) {
            if (digitalRead(pin_col[i]) == LOW) {
                if (i == 0 && j == 0) {
                    return "7";
                }
                else if (i == 1 && j == 0) {
                    return "8";
                }
                else if (i == 2 && j == 0) {
                    return "9";
                }
                else if (i == 3 && j == 0) {
                    return "/";
                }

                else if (i == 0 && j == 1) {
                    return "4";
                }
                else if (i == 1 && j == 1) {
                    return "5";
                }
                else if (i == 2 && j == 1) {
                    return "6";
                }
                else if (i == 3 && j == 1) {
                    return "*";
                }

                else if (i == 0 && j == 2) {
                    return "1";
                }
                else if (i == 1 && j == 2) {
                    return "2";
                }
                else if (i == 2 && j == 2) {
                    return "3";
                }
                else if (i == 3 && j == 2) {
                    return "-";
                }
                
                else if (i == 0 && j == 3) {
                    return "0";
                }
                else if (i == 1 && j == 3) {
                    return ".";
                }
                else if (i == 2 && j == 3) {
                    return "Ent";
                }
                else if (i == 3 && j == 3) {
                    return "+";
                }
            }
            delay(10);
        }
        digitalWrite(pin_row[j], HIGH);
    }
    return "non";
}

void push() {
    t = z;
    z = y;
    y = x;
}

void pop() {
    x = y;
    y = z;
    z = t;
}

void blink_display() {
    display.clearDisplay();
    display.display();
    delay(100);
}

void loop() {
    String key = key_scan();

    // Key detection
    if (!key.equals("non")) {

        // operator
        if (key.equals("+")) {
            float acc1 = x;
            pop();
            float acc2 = x;
            x = acc2 + acc1;
            x_disp = String(x);
            
            operator_flg = 1;
            blink_display();
        }
        else if (key.equals("-")) {
            float acc1 = x;
            pop();
            float acc2 = x;
            x = acc2 - acc1;
            x_disp = String(x);
            
            operator_flg = 1;
            blink_display();
        }
        else if (key.equals("*")) {
            float acc1 = x;
            pop();
            float acc2 = x;
            x = acc2 * acc1;
            x_disp = String(x);
            
            operator_flg = 1;
            blink_display();
        }
        else if (key.equals("/")) {
            float acc1 = x;
            pop();
            float acc2 = x;
            x = acc2 / acc1;
            x_disp = String(x);

            operator_flg = 1;
            blink_display();
        }
        // Enter
        else if (key.equals("Ent")) {
            push();
            x_disp = String(x);

            enter_flg = 1;
            operator_flg = 0;

            blink_display();
        }
        // numeral or .
        else {
            if (enter_flg) {
                x_disp = "";
                enter_flg = 0;
            }

            if (operator_flg) {
                push();
                x_disp = "";
                operator_flg = 0;
            }
            
            if (x_disp.indexOf(".") == -1 && !key.equals(".")) {
                if (x_disp.length() < MAX_DISP_LEN - 3) {
                    x_disp.concat(key);
                    x = x_disp.toFloat();
                }
            }
            else {
                if (x_disp.length() < MAX_DISP_LEN) {
                    x_disp.concat(key);
                    x = x_disp.toFloat();
                }
            }
        }

        // display
        display.clearDisplay();
        display.setCursor(4, 7);
        display.setTextSize(2.3);
        
        int length = x_disp.length();
        if (length > MAX_DISP_LEN) {
            display.print(x_disp.substring(length - MAX_DISP_LEN, length));

            // over flow
            display.setCursor(30, 23);
            display.setTextSize(1.5);
            display.print("e");
        }
        else {
            display.print(x_disp);
        }
        display.display();

        delay(200);
    }
}
