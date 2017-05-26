/*
 * Alien Advance
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "cpu_speed.h"
#include "usb_serial.h"
#include "lcd.h"
#include "graphics.h"

typedef struct GameState {
    int lives;
    int score;
    int time_m;
    int time_s;
    int time_ms;
} GameState;

int game_over = 0;
GameState gameState = {10, 0, 0, 0, 0};

int ship_x, ship_y;
int orient; // 0:up, 1:right, 2:down, 3:left

int alien_num = 5;
int alien_x[10], alien_y[10];
int alien_timer[10];
int speed_x[10], speed_y[10];

typedef struct Missle {
    int pos_x;
    int pos_y;
    int valid;
    int speed;
} Missle;

int MCNT = 15;
Missle missles[15];
int mis_count = 0;

int press_count = 0;

/**
 * Function implementations
 */
// Completed functions used in the game
void count_down(void);
void set_up(void);
void draw_status_frame(void);
void draw_gameover(void);
void draw_spaceship(void);
void draw_alien(void);
void draw_missle(int);

void update_alien(int t);
void update_missle(void);
void get_inputs(void);

// Other helper functions
void init_hardware(void);
void draw_centred(unsigned char y, char* string);
void send(char* string);
void send_line(char* string);
void recv_line(char* buff, unsigned char max_length);

unsigned char debounced_right_press();
unsigned char debounced_left_press();
void check_presses_debounced(void);

void reset_game_state(void)
{
    gameState.lives = 10;
    gameState.score = 0;
    gameState.time_m = 0;
    gameState.time_s = 0;
    gameState.time_ms = 0;
}

unsigned char debounced_right_press() {
    if ((PINF >> PF5) & 1) {
        _delay_ms(100);
        while ((PINF >> PF5) & 1);
        return 1;
    } else {
        return 0;
    }
}

unsigned char debounced_left_press() {
    if ((PINF >> PF6) & 1) {
        _delay_ms(100);
        while ((PINF >> PF6) & 1);
        return 1;
    } else {
        return 0;
    }
}

/**
 * Main - Run the game which repeatedly loops through trying to find the gold
 */
int main(void) {

    set_clock_speed(CPU_8MHz);
    init_hardware();

    // Wait until the 'player' is attached...
    //while (!usb_configured() || !usb_serial_get_control());

    // Run the main game loop
    while (1) {

        //debounced_left_press() ;
        //debounced_right_press() ;

        // Game start screen
        clear_screen();

        game_over = 0;

        draw_centred(4, "Alien Advance");
        draw_centred(14, "Alvin Yang   ");
        draw_centred(24, "n9345710     ");
        draw_centred(34, "Press to continue");

        show_screen();
        //send_line("Press 's' to start...");
        //
        //check_presses_debounced();
        while (!((PIND>>1)&0b1)) ;

        count_down(); // count down timer

        set_up();
        reset_game_state();

        while (!game_over) {
            clear_screen();

            draw_status_frame();
            draw_spaceship();

            draw_alien();

            update_alien(100);
            update_missle();

            get_inputs();

            _delay_ms(300);


            // Check for time.
            gameState.time_ms += 300;

            if (gameState.time_ms >= 1000){
                gameState.time_s += 1;
                gameState.time_ms = 0;
            }

            if (gameState.time_s >= 60){
                gameState.time_m += 1;
                gameState.time_s = 0;
            }

            show_screen();
        }

        draw_gameover();

        while (!((PIND>>1)&0b1)) ;
        //check_presses_debounced();
    }

    return 0;
}

void init_hardware(void) {

    DDRB |= ((1<<2) | (1<<3));
    DDRF &= ~((1<<5) | (1<<6));
    DDRB &= ~((0b1<<0) | (0b1<<1) | (0b1<<7));
    PORTB &= 0b00000000;
    DDRD &= ~((0b1<<0) | (0b1<<1));
    DDRF &= ~((0b1<<1) | (0b1<<0));

    lcd_init(LCD_DEFAULT_CONTRAST);

    clear_screen();
    //DDRF &= ~((1 << PF5) | (1 << PF6));
    // Initialise the USB serial
    //usb_init();
}

void draw_centred(unsigned char y, char* string) {
    // Draw a string centred in the LCD when you don't know the string length
    unsigned char l = 0, i = 0;
    while (string[i] != '\0') {
        l++;
        i++;
    }
    char x = 42 - (l * 5 / 2);
    draw_string((x > 0) ? x : 0, y, string);
}

void send(char* string) {
    // Send all of the characters in the string
    unsigned char char_count = 0;
    while (*string != '\0') {
        usb_serial_putchar(*string);
        string++;
        char_count++;
    }
}

void send_line(char* string) {
    // Send all of the characters in the string
    unsigned char char_count = 0;
    while (*string != '\0') {
        usb_serial_putchar(*string);
        string++;
        char_count++;
    }

    // Go to a new line (force this to be the start of the line)
    usb_serial_putchar('\r');
    usb_serial_putchar('\n');
}

void recv_line(char* buff, unsigned char max_length) {
    // Loop over storing characters until the buffer is full or a newline character is received
    unsigned char char_count = 0;
    int16_t curr_char;
    do {
        // BLOCK here until a character is received
        do {
            curr_char = usb_serial_getchar();
        } while (curr_char == -1);

        // Add to the buffer if it wasn't a newline (accepts other gibberish that may not necessarily be a character!)
        if (curr_char != '\n' && curr_char != '\r') {
            buff[char_count] = curr_char;
            char_count++;
        }
    } while (curr_char != '\n' && curr_char != '\r' && char_count < max_length - 1);

    // Add the null terminator to the end of the string
    buff[char_count] = '\0';
}

void count_down(void) 
{
    char counter[2];
    counter[0] = 0;
    counter[1] = 0;

    for (int i = 3; i > 0; i--) {
        clear_screen();

        counter[0] = i + '0';

        draw_centred(18, counter);
        show_screen();
        _delay_ms(300);
    }

    _delay_ms(300);
}

void draw_status_frame(void)
{
    draw_line(0, 8, 0, LCD_Y - 2);
    draw_line(0, LCD_Y - 2, LCD_X - 2, LCD_Y - 2);
    draw_line(LCD_X - 2, LCD_Y - 2, LCD_X - 2, 8);
    draw_line(LCD_X - 2, 8, 0, 8);

    char buf[20];
    int padding = (LCD_X+3)/4;

    sprintf(buf, "L:%d", gameState.lives);
    draw_string(2, 0, buf);

    sprintf(buf, "S:%d", gameState.score);
    draw_string(padding, 0, buf);

    sprintf(buf, "T:%02d:%02d", gameState.time_m, gameState.time_s);
    draw_string(2*padding, 0, buf);
}

int overlap_with_ship(int i)
{
    if ((alien_x[i] >= ship_x && alien_x[i]<=ship_x+5)
            || (alien_x[i]+3 >= ship_x && alien_x[i]+3 <= ship_x+5)) {
        if ((alien_y[i] >= ship_y && alien_y[i] <= ship_y+3)
                || (alien_y[i]+3>=ship_y && alien_y[i]+3 <= ship_y+3))
            return 1;
    }

    return 0;
}

int overlap(int i)
{
    int ret = 0;

    if (overlap_with_ship(i)) return 1;

    for (int j = 0; j < i; j++) {
        if ((alien_x[i] >= alien_x[j] && alien_x[i]<=alien_x[j]+3)
                || (alien_x[i]+3 >= alien_x[j] && alien_x[i]+3 <= alien_x[j]+3)) {
            if ((alien_y[i] >= alien_y[j] && alien_y[i] <= alien_y[j]+3)
                    || (alien_y[i]+3>=alien_y[j] && alien_y[i]+3 <= alien_y[j]+3))
                return 1;
        }

    }

    return ret;
}

void set_up(void)
{
    srand(time(NULL));

    ship_x = rand()%LCD_X-6;
    ship_y = rand()%LCD_Y-11;

    orient = 0; // 0:up, 1:right, 2:down, 3:left

    for (int i = 0; i < alien_num; i++) {
        do {
            alien_x[i] = rand()%LCD_X-3;
            alien_y[i] = rand()%LCD_Y-11;
        } while (overlap(i));

        alien_timer[i] = 1000*(rand()%3+2); /* reset timer */
        speed_y[i] = 1;
        speed_x[i] = 1;
    }

    for (int i = 0; i < MCNT; i++) {
        missles[i].valid = 0;
    }
}

void draw_gameover(void)
{
    clear_screen();

    draw_centred(14, " Game Over ");
    draw_centred(24, "Play Again?");

    show_screen();
}

void draw_spaceship(void)
{
    set_pixel(ship_x+2, ship_y+9, 1);
    set_pixel(ship_x+1, ship_y+10, 1);
    set_pixel(ship_x+2, ship_y+10, 1);
    set_pixel(ship_x+3, ship_y+10, 1);
    set_pixel(ship_x+0, ship_y+11, 1);
    set_pixel(ship_x+1, ship_y+11, 1);
    set_pixel(ship_x+2, ship_y+11, 1);
    set_pixel(ship_x+3, ship_y+11, 1);
    set_pixel(ship_x+4, ship_y+11, 1);
    /*
    draw_string(ship_x, ship_y+1, "   |   ");
    draw_string(ship_x, ship_y+2, "  /o\\  ");
    draw_string(ship_x, ship_y+3, "|/___\\|");
    */
}

void draw_single_alien(int x, int y)
{
    set_pixel(x+1, y+9, 1);
    set_pixel(x+0, y+10, 1);
    set_pixel(x+1, y+10, 1);
    set_pixel(x+2, y+10, 1);
    set_pixel(x+1, y+11, 1);
    /*
    draw_string(x, y+1, " + ");
    draw_string(x, y+2, "+V+");
    draw_string(x, y+3, " + ");
    */
}

void draw_alien(void)
{
    int count = 0;
    for (int i = 0; i < alien_num; i++) {
        if (alien_x[i] == LCD_X + 10) {
            count ++;
        } else {
            draw_single_alien(alien_x[i], alien_y[i]);
        }
    }

    if (count == alien_num) { // all die, must be re-appear together
        for (int i = 0; i < alien_num; i++) {
            do {
                alien_x[i] = rand()%(LCD_X-5)+2;
                alien_y[i] = rand()%LCD_Y-11;
            } while (overlap(i));

            alien_timer[i] = 1000*(rand()%3+2); /* reset timer */
            speed_y[i] = 1;
            speed_x[i] = 1;

            draw_single_alien(alien_x[i], alien_y[i]);
        }
    }
}

void check_collision(int i)
{
    if (overlap_with_ship(i)) {
        gameState.lives -= 1;

        if (gameState.lives <= 0) {
            game_over = 1;
        } else {
            ship_x = rand()%(LCD_X-7)+2;
            ship_y = rand()%LCD_Y-11;
        }

        return;
    }

    if (alien_x[i] < 0 || alien_x[i]+3 > LCD_X
            || alien_y[i] < 8 || alien_y[i]+3 > LCD_Y) {
        // playfield boundary, stop
        //speed_y[i] = 0;
        //speed_x[i] = 0;
    }
}

void update_alien(int t)
{
    for (int i = 0; i < alien_num; i++) {
        if (alien_timer[i] > 0) {
            alien_timer[i] -= t;
        }

        if (alien_timer[i] <= 0) {
            // begin attack
            if (alien_y[i] < ship_y && alien_y[i] < LCD_Y-4) {
                alien_y[i] += speed_y[i]; /* 4 pixel per second */
            } else if (alien_y[i] > ship_y && alien_y[i] > 8){
                alien_y[i] -= speed_y[i];
            }

            if (alien_x[i] < ship_x && alien_x[i] < LCD_X-3) {
                alien_x[i] += speed_x[i];
            } else if (alien_x[i] > ship_x && alien_x[i] > 1) {
                alien_x[i] -= speed_x[i];
            }

            check_collision(i);
        }
    }
}

void update_missle(void)
{
    for (int i = 0; i < MCNT; i++) {
        if (missles[i].valid == 1) {
            missles[i].pos_y -= missles[i].speed;
            if (missles[i].pos_y < 8) {
                // outside
                missles[i].valid = 0;
                mis_count--;
            } else {
                for (int j = 0; j < alien_num; j++) {
                    if (missles[i].pos_x >= alien_x[j] 
                            && missles[i].pos_x <= alien_x[j]+3
                            && missles[i].pos_y >= alien_y[j]
                            && missles[i].pos_y <= alien_y[j]+3) {
                        // hit alien
                        gameState.score += 1;

                        alien_x[i] = LCD_X + 10;
                        missles[i].valid = 0;
                        break;
                    }
                }
            }

            if (missles[i].valid == 1) {
                draw_missle(i);
            }
        }
    }
}

void get_inputs(void)
{ 
    if (((PINF>>5) & 0b1)) {
        // shot missle, down
        if (mis_count < MCNT) {
            for (int i = 0; i < MCNT; i++) {
                if (missles[i].valid == 0) {
                    missles[i].valid = 1;
                    missles[i].speed = 4;
                    missles[i].pos_x = ship_x+2;
                    missles[i].pos_y = ship_y+7;
                    mis_count++;
                    break;
                }
            }
        }
    } else {
        if (((PINB>>7) & 0b1) && (ship_y < LCD_Y-18)) {
            ship_y += 5;
        } else if  (((PIND>>1) & 0b1) && (ship_y > 7)){ // up
            ship_y -= 5;
        } else if (((PINB>>1) & 0b1) && (ship_x > 6)) { // left
            ship_x -= 5;
        } else if (((PIND>>0) & 0b1) && (ship_x < LCD_X-10)) { //right
            ship_x += 5;
        }
    }
}

void draw_missle(int i)
{
    set_pixel(missles[i].pos_x, missles[i].pos_y, 1);
    set_pixel(missles[i].pos_x, missles[i].pos_y+1, 1);
    /*
    draw_string(missles[i].pos_x, missles[i].pos_y, "**");
    draw_string(missles[i].pos_x, missles[i].pos_y+1, "**");
    */
}

void check_presses_debounced(void) {
    // A button press should only be registered ONCE every time that the
    // button is RELEASED. Left decreases the count by 1, right increases the
    // count by 1.
    // TODO
    if((PINF >> PIN5) & 1) {
        _delay_ms(50);
        press_count++;
    }
    while((PINF << PF5) & 1);

    if((PINF >> PIN6) & 1) {
        _delay_ms(50);
        press_count--;
    }
    while((PINF << PF6) & 1);
}
