#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx.h"
#include "lcd.h"
#include "bitmap.h"
#define TOP GPIO_PIN_10
#define BOTTOM GPIO_PIN_11
#define RIGHT GPIO_PIN_12
#define LEFT GPIO_PIN_15


int main(void){
	SystemCoreClock=8000000;
	HAL_Init();
	//Peripherals initialization
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7; // SCK, MOSI
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_6; // MISO
	gpio.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = LCD_DC|LCD_CE|LCD_RST;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);
	HAL_GPIO_WritePin(GPIOC, LCD_CE|LCD_RST, GPIO_PIN_SET);

	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = GPIO_PIN_6;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &gpio);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET); // backlight

	gpio.Pin = TOP|BOTTOM|RIGHT|LEFT; // input buttons
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpio);

	spi.Instance = SPI1;
	spi.Init.Mode = SPI_MODE_MASTER;
	spi.Init.NSS = SPI_NSS_SOFT;
	spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // 1MHz
	spi.Init.Direction = SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi.Init.DataSize = SPI_DATASIZE_8BIT;
	spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi.Init.TIMode = SPI_TIMODE_DISABLE;
	spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&spi);
	__HAL_SPI_ENABLE(&spi);


	lcd_setup();
	draw_start_screen();
	char move;
	uint8_t x=40,y=20;
	uint8_t fruitX=50,fruitY=10;
	uint8_t fruitXtab[16]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};
	uint8_t fruitYtab[10]={0,5,10,15,20,25,30,35,40};
	uint8_t snakeTail=0;
	uint8_t tailX[100],tailY[100];
	uint8_t prevX=x,prevY=y;
	uint counter=0, time=0;
	bool gameOver=false;
	bool startgame=false;
	uint32_t lastTime=HAL_GetTick();
	uint32_t seed;

	//main program loop
	while(1){
		//waiting for the start of the game
		while(startgame==false){
			if(HAL_GetTick()-lastTime>1000){
				lastTime=HAL_GetTick();
			}
			if(HAL_GPIO_ReadPin(GPIOA,RIGHT)==0){
				HAL_Delay(5);
				seed=HAL_GetTick()-lastTime;
				srand(seed);
				startgame=true;
				while(!HAL_GPIO_ReadPin(GPIOA,RIGHT));
			}
		}

		//main game loop
		while(gameOver==false){
				lcd_clear();
				draw_border_line();

				if((HAL_GPIO_ReadPin(GPIOA,BOTTOM)==0 && move!='t') || (HAL_GPIO_ReadPin(GPIOA,BOTTOM)==0 && snakeTail==0)){
					HAL_Delay(5);
					move='b';
					while(!HAL_GPIO_ReadPin(GPIOA,BOTTOM));
				}
				if((HAL_GPIO_ReadPin(GPIOA,TOP)==0 && move!='b') || (HAL_GPIO_ReadPin(GPIOA,TOP)==0 && snakeTail==0)){
					HAL_Delay(5);
					move='t';
					while(!HAL_GPIO_ReadPin(GPIOA,TOP));
				}
				if((HAL_GPIO_ReadPin(GPIOA,RIGHT)==0 && move!='l') || (HAL_GPIO_ReadPin(GPIOA,RIGHT)==0 && snakeTail==0)){
					HAL_Delay(5);
					move='r';
					while(!HAL_GPIO_ReadPin(GPIOA,RIGHT));
				}
				if((HAL_GPIO_ReadPin(GPIOA,LEFT)==0 && move!='r') || (HAL_GPIO_ReadPin(GPIOA,LEFT)==0 && snakeTail==0)){
					HAL_Delay(5);
					move='l';;
					while(!HAL_GPIO_ReadPin(GPIOA,LEFT));
				}

				if(prevX!=x || prevY!=y){
					tailX[counter]=prevX;
					tailY[counter]=prevY;
					counter++;
					if(counter==100){
						for(int i=0;i<snakeTail+1;i++){
							tailX[snakeTail-i]=tailX[99-i];
							tailY[snakeTail-i]=tailY[99-i];
							counter=snakeTail+1;
						}
					}
				}else{
					for(int i=counter-snakeTail;i<counter;i++){
						lcd_rectangle(1+tailX[i],1+tailY[i],5+tailX[i],5+tailY[i]);
							if(x==tailX[i] && y==tailY[i]){
								gameOver=true;
							}
					}
				}

				lcd_rectangle_black(1+x,1+y,5+x,5+y);
				lcd_draw_circle(fruitX+3,fruitY+3,2);

				if(x==fruitX && y==fruitY){
					bool onSnake=true;
					while(onSnake){
						onSnake=false;
						fruitX=fruitXtab[rand()%16];
						fruitY=fruitYtab[rand()%9];
						for(int i=counter-snakeTail;i<counter;i++){
							if(tailX[i]==fruitX && tailY[i]==fruitY){
								onSnake=true;
							}
						}
					}
					snakeTail++;
				}

				prevX=x;
				prevY=y;
				if(time>10){
					switch(move){
						case 'b':
							 y+=5;
							break;
						case 't':
							 y-=5;
							break;
						case 'l':
							 x-=5;
							break;
						case 'r':
							 x+=5;
							break;
						default:
							break;
					}
					time=0;
				}
				if(gameOver==false){
					gameOver=exceed(x,y);
				}
			lcd_copy();
			HAL_Delay(50);
			time++;
		}

		lcd_clear();
		lcd_draw_text(1, 20 , "Game Over!");
		char score[10]="";
		itoa(snakeTail,score,10);
		lcd_draw_text(2,5,"Your score:");
		lcd_draw_text(2,70,score);
		lcd_copy();
		HAL_Delay(2500);
		//start initialization
		draw_start_screen();
		x=40,y=20;
		fruitX=50,fruitY=10;
		prevX=x,prevY=y;
		snakeTail=0;
		counter=0;
		time=0;
		move='x';
		gameOver=false;
		startgame=false;
		for(int i=0;i<100;i++){
			tailX[i]=0;tailY[i]=0;
		}

	 }

}
