#include "Wire.h"
#include "MAX1704.h"
#include "U8glib.h"
#include <usb_keyboard.h>
#include "keyboard.h"

#define OLED_PRINTF(...) snprintf(printf_str, LINE_LENGTH, __VA_ARGS__); oled_printfln();

//#define DEBUG(x) x
#define DEBUG(x)

#define DPRINTF(...) DEBUG(OLED_PRINTF(__VA_ARGS__))
#define DPRINTCHAR(x) DEBUG(oled_print(x); oled_print(10); pic_loop();)

/*
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║x║ ║v║ ║c║ ║l║ ║n║ ║f║     ║R║ ║u║ ║o║ ║p║ ║y║ ║;║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║z║ ║d║ ║s║ ║t║ ║h║ ║r║     ║B║ ║S║ ║e║ ║i║ ║a║ ║k║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║T║ ║w║ ║b║ ║\║ ║m║ ║E║     ║'║ ║j║ ║g║ ║q║ ║,║ ║.║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 *             ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗            
 *             ║A║ ║C║ ║ ║     ║ ║ ║S║ ║G║            
 *             ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝
 *            
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║`║ ║1║ ║2║ ║3║ ║4║ ║5║     ║R║ ║H║ ║D║ ║U║ ║E║ ║ ║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║-║ ║6║ ║7║ ║8║ ║9║ ║0║     ║D║ ║L║ ║D║ ║U║ ║R║ ║ ║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║=║ ║ ║ ║ ║ ║\║ ║[║ ║]║     ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 *             ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗            
 *             ║T║ ║T║ ║T║     ║T║ ║T║ ║T║            
 *             ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝   
 *            
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║E║ ║q║ ║w║ ║e║ ║r║ ║t║     ║y║ ║u║ ║i║ ║o║ ║p║ ║B║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║T║ ║a║ ║s║ ║d║ ║f║ ║g║     ║h║ ║j║ ║k║ ║l║ ║;║ ║'║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 * ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗ ╔═╗
 * ║S║ ║z║ ║x║ ║c║ ║v║ ║b║     ║n║ ║m║ ║,║ ║.║ ║/║ ║R║
 * ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝ ╚═╝
 *             ╔═╗ ╔═╗ ╔═╗     ╔═╗ ╔═╗ ╔═╗            
 *             ║C║ ║S║ ║S║     ║S║ ║S║ ║C║            
 *             ╚═╝ ╚═╝ ╚═╝     ╚═╝ ╚═╝ ╚═╝          
 */

//  0: taken by UART
//  1: taken by UART
//  2: row pin
//  3: row pin
//  4: row pin
//  5: row pin
//  6: row pin
//  7: row pin
//  8: row pin
//  9: taken by SPI
// 10: taken by SPI
// 11: taken by SPI
// 12: row pin
// 13: taken by SPI
// 14: col pin
// 15: col pin
// 16: col pin
// 17: col pin
// 18: taken by I2C
// 19: taken by I2C
// 20: col pin
// 21: unused
// 22: col pin
// 23: usb detection

#define USB_DETECT_PIN 23

MAX1704 fg;

float charge;

U8GLIB_SSD1306_128X64 u8g(10, 9);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

#define LINES 7
#define TOTAL_LINES 20
#define LINE_LENGTH 26
#define LINE_SPACING u8g.getFontLineSpacing()
char lines[TOTAL_LINES][LINE_LENGTH + 1];

//uint8_t cmd_set = false;
uint8_t cur_line = LINES;
uint8_t cursor_pos = 0;
uint8_t start_line = 0;

void draw();

void pic_loop()
{
    u8g.firstPage();  
    do {
        draw();
    } while( u8g.nextPage() );
}

void oled_print(uint8_t a)
{
    if (a == 10)
    {
        for (; cursor_pos < LINE_LENGTH; cursor_pos++)
            lines[cur_line][cursor_pos] = ' ';
        cursor_pos = 0;
        start_line++; 
        if (start_line >= TOTAL_LINES)
            start_line -= TOTAL_LINES; 
        cur_line++;
        if (cur_line >= TOTAL_LINES)
        {
            cur_line -= TOTAL_LINES;
        } 
    }
    else if (cursor_pos < LINE_LENGTH)
    {
        lines[cur_line][cursor_pos] = a;
        cursor_pos++;
    }
}

static char printf_str[LINE_LENGTH];
void oled_printf()
{
    for (uint8_t i = 0; i < strlen(printf_str); i++)
    {
        oled_print(printf_str[i]);
    }
}

void oled_printfln()
{
    for (uint8_t i = 0; i < strlen(printf_str); i++)
    {
        oled_print(printf_str[i]);
    }
    oled_print(10);
    pic_loop();
}


#define ROWS 8
#define COLS 6

const uint8_t row_pins[] = {12, 8, 7, 6, 5, 4, 3, 2};
const uint8_t col_pins[] = {14, 15, 16, 17, 22, 21};

typedef void (*callback_t)(uint8_t, uint8_t, uint8_t, uint8_t);

struct layer_t
{
        const uint8_t (*data)[COLS];
        const callback_t (*callbacks)[COLS];
        char name[12];
};

#define NUMBER_OF_LAYERS 3
layer_t layers[NUMBER_OF_LAYERS];
#define LAYER_STACK_SIZE 32
layer_t *layer_stack[LAYER_STACK_SIZE];
uint8_t layer_stack_index = 0;

uint8_t matrix[LAYER_STACK_SIZE][ROWS][COLS];
//int bounce_time[ROWS][COLS];

//uint8_t modifier_byte = 0;
//uint8_t scancode_bytes[6];
uint8_t keyboard_keys_used_index = 0;

void oled_print(uint8_t);

void send_shorthand()
{
    Serial1.write(0xFE);
    Serial1.write(keyboard_keys_used_index + 2);
    Serial1.write(keyboard_modifier_keys);

    DPRINTCHAR('a');
    for (uint8_t i = 0; i <= keyboard_keys_used_index; i++)
    {
        Serial1.write(keyboard_keys[i]);
    }
    
    DPRINTCHAR('b');
    if (digitalRead(USB_DETECT_PIN) == HIGH)
    {
        usb_keyboard_send();
        Serial.print(keyboard_modifier_keys);
        for (uint8_t i = 0; i <= keyboard_keys_used_index; i++)
        {
            Serial.print(' ');
            Serial.print(keyboard_keys[i]);
        }
        Serial.println();
    }
    DPRINTCHAR('c');
}

#define DEFAULT_LAYER_INDEX 0
#define FN1_LAYER_INDEX 1
#define GAME_LAYER_INDEX 2
const uint8_t default_layer_data[ROWS][COLS] = {{KEY_RETURN, KEY_U, KEY_O, KEY_P, KEY_Y, KEY_SEMICOLON},
                                                {KEY_BACKSPACE, KEY_SPACE, KEY_E, KEY_I, KEY_A, KEY_K},
                                                {KEY_APOSTROPHE, KEY_J, KEY_G, KEY_Q, KEY_COMMA, KEY_PERIOD},
                                                {FN1_LAYER_INDEX, MODIFIER_SHIFT_LEFT, MODIFIER_GUI_LEFT, GAME_LAYER_INDEX, 0, 0},
                                                {KEY_F, KEY_N, KEY_L, KEY_C, KEY_V, KEY_X},
                                                {KEY_R, KEY_H, KEY_T, KEY_S, KEY_D, KEY_Z},
                                                {KEY_ESCAPE, KEY_M, KEY_SLASH, KEY_B, KEY_W, KEY_TAB},
                                                {FN1_LAYER_INDEX, MODIFIER_CONTROL_LEFT, MODIFIER_ALT_LEFT, 0, 0, 0}};
const uint8_t fn1_layer_data[ROWS][COLS] = {{KEY_RETURN, KEY_HOME, KEY_PAGE_DOWN, KEY_PAGE_UP, KEY_END, 0},
                                            {KEY_DELETE, KEY_ARROW_LEFT, KEY_ARROW_DOWN, KEY_ARROW_UP, KEY_ARROW_RIGHT, 0},
                                            {0, 0, 0, 0, 0, 0},
                                            {FN1_LAYER_INDEX, 0, 0, 0, 0, 0},
                                            {KEY_5, KEY_4, KEY_3, KEY_2, KEY_1, KEY_GRAVE},
                                            {KEY_0, KEY_9, KEY_8, KEY_7, KEY_6, KEY_MINUS},
                                            {KEY_BRACKET_RIGHT, KEY_BRACKET_LEFT, KEY_BACKSLASH, 0, 0, KEY_EQUAL},
                                            {FN1_LAYER_INDEX, 0, 0, 0, 0, 0}};
const uint8_t game_layer_data[ROWS][COLS] = {{KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSPACE},
                                             {KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE},
                                             {KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RETURN},
                                             {KEY_SPACE, KEY_SPACE, MODIFIER_CONTROL_RIGHT, GAME_LAYER_INDEX, 0, 0},
                                             {KEY_T, KEY_R, KEY_E, KEY_W, KEY_Q, KEY_ESCAPE},
                                             {KEY_G, KEY_F, KEY_D, KEY_S, KEY_A, KEY_TAB},
                                             {KEY_B, KEY_V, KEY_C, KEY_X, KEY_Z, MODIFIER_SHIFT_LEFT},
                                             {KEY_SPACE, KEY_SPACE, MODIFIER_CONTROL_LEFT, 0, 0, 0}};

void null_callback(uint8_t, uint8_t, uint8_t, uint8_t) {}

void debug_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    if (!raised) oled_print(' ');
    //oled_print(chars[row][col]);
    oled_print(10);
    //Serial.println(chars[row][col]);
    delay(30);
}

void key_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    if (raised)
    {
        uint8_t key = layer_stack[working_stack_index]->data[row][col];
        for (uint8_t i = 0; i <= keyboard_keys_used_index; i++)
        {
            if (keyboard_keys[i] == key)
            {
                if (i == keyboard_keys_used_index && keyboard_keys_used_index)
                {
                    keyboard_keys_used_index--;
                }
                keyboard_keys[i] = 0;
                break;
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            if (!keyboard_keys[i])
            {
                keyboard_keys_used_index = max(keyboard_keys_used_index, i);
                keyboard_keys[i] = layer_stack[working_stack_index]->data[row][col];
                break;
            }
        }
    }
    send_shorthand();
}

void modifier_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    if (raised)
    {
        keyboard_modifier_keys = keyboard_modifier_keys & (~layer_stack[working_stack_index]->data[row][col]);
    }
    else
    {
        keyboard_modifier_keys = keyboard_modifier_keys | layer_stack[working_stack_index]->data[row][col];
    }
    send_shorthand();
}

void transparent_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    callback_t callback;
    for (uint8_t i = working_stack_index - 1; i >= 0; i--)
    {
        callback = layer_stack[i]->callbacks[row][col];
        if (callback != transparent_callback)
        {
            callback(row, col, raised, i);
            break;
        }
    }
}

void reset_keys_callback(uint8_t, uint8_t, uint8_t raised, uint8_t working_stack_index)
{
    if (!raised)
    {
        keyboard_modifier_keys = 0;
        for (uint8_t i = 0; i < 6; i++)
        {
            keyboard_keys[i] = 0;
        }
        send_shorthand();
    }
}

void layer_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    if (raised)
    {
        layer_t* layer_to_remove = &layers[layer_stack[working_stack_index]->data[row][col]];
        for (uint8_t i = layer_stack_index; i > 0; i--)
        {
            if (layer_stack[i] == layer_to_remove)
            {
                OLED_PRINTF("pop %s to %s", layer_to_remove->name, layer_stack[i - 1]->name);
                OLED_PRINTF("     depth %u", layer_stack_index - 1);
                for (uint8_t j = i; j < layer_stack_index; j++)
                {
                    layer_stack[j] = layer_stack[j + 1];
                }
                layer_stack_index--;
                callback_t callback_to_check;
                for (uint8_t r = 0; r < ROWS; r++)
                {
                    for (uint8_t c = 0; c < COLS; c++)
                    {
                        callback_to_check = layer_stack[layer_stack_index + 1]->callbacks[r][c];
                        if (callback_to_check != layer_callback && callback_to_check != transparent_callback)
                            callback_to_check(r, c, HIGH, layer_stack_index + 1);
                        matrix[layer_stack_index][r][c] = matrix[layer_stack_index + 1][r][c];
                    }
                }
                break;
            }
        }
    }
    else
    {
        if (layer_stack_index + 1 >= LAYER_STACK_SIZE)
        {
            layer_stack_index = LAYER_STACK_SIZE - 1;
        }
        else
        {
            layer_stack[layer_stack_index + 1] = &layers[layer_stack[working_stack_index]->data[row][col]];
            layer_stack_index++;
            OLED_PRINTF("push %s over %s", layer_stack[layer_stack_index]->name, layer_stack[layer_stack_index - 1]->name);
            OLED_PRINTF("     depth %u", layer_stack_index);
            for (uint8_t r = 0; r < ROWS; r++)
            {
                for (uint8_t c = 0; c < COLS; c++)
                {
                    matrix[layer_stack_index][r][c] = matrix[layer_stack_index - 1][r][c];
                }
            }
        }
    }
    //reset_keys_callback(0, 0, 0, working_stack_index);
}

void layer_toggle_callback(uint8_t row, uint8_t col, uint8_t raised, uint8_t working_stack_index)
{
    if (raised)
    {
        // this space intentionally left blank 
    }
    else
    {
        layer_t* layer = &layers[layer_stack[working_stack_index]->data[row][col]];
        if (layer_stack[layer_stack_index] == layer && layer_stack_index > 0)
        {
            callback_t callback_to_check;
            for (uint8_t r = 0; r < ROWS; r++)
            {
                for (uint8_t c = 0; c < COLS; c++)
                {
                    callback_to_check = layer_stack[layer_stack_index]->callbacks[r][c];
                    if (callback_to_check != layer_callback && callback_to_check != transparent_callback)
                        callback_to_check(r, c, HIGH, layer_stack_index);
                }
            }
            layer_stack_index--;
            OLED_PRINTF("pop %s to %s", layer->name, layer_stack[layer_stack_index]->name);
            OLED_PRINTF("     depth %u", layer_stack_index);
        }
        else
        {
            if (layer_stack_index + 1 >= LAYER_STACK_SIZE)
            {
                layer_stack_index = LAYER_STACK_SIZE - 1;
            }
            else
            {
                OLED_PRINTF("  %u", layer_stack_index);
                layer_stack[layer_stack_index + 1] = layer;
                layer_stack_index++;
                OLED_PRINTF("push %s over %s", layer->name, layer_stack[layer_stack_index - 1]->name);
                OLED_PRINTF("     depth %u", layer_stack_index);
                for (uint8_t r = 0; r < ROWS; r++)
                {
                    for (uint8_t c = 0; c < COLS; c++)
                    {
                        matrix[layer_stack_index][r][c] = matrix[layer_stack_index - 1][r][c];
                    }
                }
            }   
        }
    }
}

const callback_t default_layer_callbacks[ROWS][COLS] = {{key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {layer_callback, modifier_callback, modifier_callback, layer_toggle_callback, reset_keys_callback, null_callback},
                                                        {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                        {layer_callback, modifier_callback, modifier_callback, null_callback, null_callback, null_callback}};
const callback_t fn1_layer_callbacks[ROWS][COLS] = {{key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {layer_callback, transparent_callback, transparent_callback, transparent_callback, transparent_callback, transparent_callback},
                                                    {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                    {layer_callback, transparent_callback, transparent_callback, transparent_callback, transparent_callback, transparent_callback}};

const callback_t game_layer_callbacks[ROWS][COLS] = {{key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                     {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                     {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                     {key_callback, key_callback, modifier_callback, layer_toggle_callback, transparent_callback, transparent_callback},
                                                     {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                     {key_callback, key_callback, key_callback, key_callback, key_callback, key_callback},
                                                     {key_callback, key_callback, key_callback, key_callback, key_callback, modifier_callback},
                                                     {key_callback, key_callback, modifier_callback, transparent_callback, transparent_callback, transparent_callback,}};




void setup()  
{
    for (uint8_t i = 0; i < ROWS; i++)
        pinMode(row_pins[i], INPUT_PULLUP);
    for (uint8_t i = 0; i < COLS; i++)
    {
        pinMode(col_pins[i], OUTPUT);
        digitalWrite(col_pins[i], HIGH);
    }
    
    for (uint8_t r = 0; r < ROWS; r++)
    {
        for (uint8_t c = 0; c < COLS; c++)
        {
            for (uint8_t i = 0; i < LAYER_STACK_SIZE; i++)
            {
                matrix[i][r][c] = HIGH;
            }
//bounce_time[r][c] = 0;
        }
    }   
  
    pinMode(USB_DETECT_PIN, INPUT);

    if (digitalRead(USB_DETECT_PIN) == HIGH)
    {
        Serial.begin(9600);
        //usb_init();
    }
    
    Wire.begin(); 
    fg.reset();
    fg.quickStart();
    fg.showConfig();
  
    Serial1.begin(115200);
    delay(100);
    Serial1.print("$");
    Serial1.print("$");
    Serial1.print("$");
    delay(100);
    Serial1.println("U,9600,N");
    delay(100);
//while (Serial1.available()) {}
    Serial1.begin(9600);
  
    u8g.setColorIndex(1);
    u8g.setRot180();
  
    for (char i = 0; i < TOTAL_LINES; i++)
    {
        for (char j = 0; j < LINE_LENGTH; j++)
            lines[i][j] = ' ';
        lines[i][LINE_LENGTH] = 0;
    }

    if (digitalRead(USB_DETECT_PIN) == HIGH)
    {
        //while (!usb_configured()) {}
    }
    oled_print('x');
    oled_print(10);

    layers[DEFAULT_LAYER_INDEX] = (layer_t){default_layer_data, default_layer_callbacks, "default"};
    layers[FN1_LAYER_INDEX] = (layer_t){fn1_layer_data, fn1_layer_callbacks, "fn1"};
    layers[GAME_LAYER_INDEX] = (layer_t){game_layer_data, game_layer_callbacks, "game"};
    layer_stack[0] = &(layers[0]);
}

void read_matrix()
{
    uint8_t key = 0;
    for (uint8_t c = 0; c < COLS; c++)
    {
        digitalWrite(col_pins[c], LOW);
        for (uint8_t r = 0; r < ROWS; r++)
        {
            key = digitalRead(row_pins[r]);
            if (matrix[layer_stack_index][r][c] != key)
            {
                DPRINTCHAR('h');
                matrix[layer_stack_index][r][c] = key;
                DEBUG(snprintf(printf_str, LINE_LENGTH, "%u", default_layer_data[r][c]); oled_printfln(););
                layer_stack[layer_stack_index]->callbacks[r][c](r, c, key, layer_stack_index);
                DPRINTCHAR('i');
            }
        }
        digitalWrite(col_pins[c], HIGH);
    }


}

void draw()
{
    int j = 6 + LINE_SPACING;
    u8g.setFont(u8g_font_baby);
    for (char i = start_line; i < LINES + start_line; i++)
    {
        u8g.drawStr(0, j, lines[i % TOTAL_LINES]); 
        j += LINE_SPACING;
    }
  
    u8g.drawBox(0, 0, 128, 7);
    u8g.setColorIndex(0);

    u8g.setPrintPos(0, LINE_SPACING - 2);
    u8g.printf("%u  %u %u %u %u %u %u  d:%u", keyboard_modifier_keys, keyboard_keys[0], keyboard_keys[1], keyboard_keys[2], keyboard_keys[3], keyboard_keys[4], keyboard_keys[5], layer_stack_index);
  
    u8g.setPrintPos(90, LINE_SPACING - 2);
    if (digitalRead(USB_DETECT_PIN) == HIGH)
    {
        u8g.print('$');
    }
  
    u8g.setPrintPos(101, LINE_SPACING - 2);
    if (charge < 0.001 || charge > 101)
    {
        u8g.print("  ERR");
    }
    else
    {
        u8g.print(charge); 
        u8g.print('%'); 
    }
    u8g.setColorIndex(1);
}


uint8_t x = 0;

void loop()
{
    charge = fg.stateOfCharge();
    pic_loop();
    read_matrix();

    // sprintf(printf_str, "%03u     %u  %u %u %u %u %u %u", x++, keyboard_modifier_keys, keyboard_keys[0], keyboard_keys[1], keyboard_keys[2], keyboard_keys[3], keyboard_keys[4], keyboard_keys[5]);
    // oled_printfln();
    
//delay(100);
}

