/*
Grūdų sandėlis (ADT: stekas, eilė). Įmonė aktyviai prekiauja grūdais. Kiekvieną dieną įmonė perka ir sandėliuoja grūdus
bei juos parduoda (atsitiktinai nuo 0 iki 100% sandėlyje turimų grūdų).  Yra nustatyta per dieną superkamų grūdų norma 
(pvz., 100 t) ir supirkimo kaina (pvz., 200 LT/t), bet natūralu, kad ne visomis dienomis vienodai sekasi, todėl ekspertiškai 
žinomas superkamų grūdų kiekio maksimalus nuokrypis (pvz., 15%, tai reiškia, kad kiekvieną dieną įmonė superka 85-115 t grūdų) 
ir maksimalus grūdų supirkimo kainos nuokrypis (pvz., 5%, tai reiškia, kad įmonė superka grūdus po 190-210 LT/t). Parduodama grūdus 
įmonė ima nustatytą antkainį (pvz., 10%) nuo APSKAITINĖS KAINOS:
@ 1) FIFO supirkimo: pirmiausia parduodami seniausiai įsigyti grūdai, APSKAITINĖ KAINA LYGI SUPIRKIMO;
@ 2) LIFO svertinių vidurkių: pirmiausia parduodami šviežiausi grūdai; apskaitinė gautų grūdų kaina nustatoma kaip tuo metu sandėlyje 
turimų grūdų kainų svertinis vidurkis (pvz., jei sandėlyje turėjome 200 t grūdų, kurių apskaitinė kaina 95 LT/t ir įsigijome 
dar 100 t po 110 LT/t, tai įsigytų grūdų apskaitinė kaina yra 100 LT/t = (200 * 95 + 100 * 110) / (200 + 100)). Tikslas: patyrinėti, 
kurį grūdų apskaitos metodą naudodama įmonė gauna didesnį pelną.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "stack.h"

#define BUY_AMOUNT 100
#define BUY_PRICE 200
#define AMOUNT_FLUCTUATION 15
#define PRICE_FLUCTUATION 5
#define SELL_SURCHARGE (double)10

#define PERIOD 31


int main()
{
	FILE *log_file;
    log_file = fopen("TestLog.txt", "w");
    if(log_file == NULL)
    {
        printf("Failet to create Log file. Shutting down the program... \n");
        exit(1);
    }
	
	double average_sell_percent = 0;
    double all_stock_FIFO = 0, profit_FIFO = 0, all_stock_LIFO = 0, profit_LIFO = 0;
	double balance_after_selling_FIFO = 0, balance_after_selling_LIFO = 0, balance_price_FIFO = 0, balance_price_LIFO = 0;
	double price = 0, amount = 0;
	
    Queue stock_FIFO, price_FIFO;
	initializeQ(&stock_FIFO);
	initializeQ(&price_FIFO);
	
	Stack stock_LIFO, price_LIFO;
	initializeS(&stock_LIFO);
	initializeS(&price_LIFO);
	
    srand(time(0));
	
	fprintf(log_file, "Base buy amount = %d ---- Base buy price = %d\nAmount fluctuation % = %d ---- Price fluctuation % = %d ---- Sell surcharge % = %lf\n\n", BUY_AMOUNT, BUY_PRICE, AMOUNT_FLUCTUATION, PRICE_FLUCTUATION, SELL_SURCHARGE);

    for(int i = 0; i < PERIOD; ++i)
    {
		fprintf(log_file, "\n=======Day %d=======\n", i+1);
		fprintf(log_file, "== stockFIFO = %f ---- stockLIFO = %f ==\n", all_stock_FIFO, all_stock_LIFO);
        //-------------------- Daily buying amount in tones -------------------------
        double buy_amount, amount_fluctuation;
        amount_fluctuation = rand() % (AMOUNT_FLUCTUATION * 2 + 1) - AMOUNT_FLUCTUATION;
        buy_amount = (BUY_AMOUNT * (1 + (amount_fluctuation / 100)));

        fprintf(log_file, "buy amount = %f ==== ", buy_amount);

        //-------------------------- Daily buying price -----------------------------
        double buy_price, price_fluctuation;
        price_fluctuation = rand() % (PRICE_FLUCTUATION * 2 + 1) - PRICE_FLUCTUATION;
        buy_price = (BUY_PRICE * (1 + (price_fluctuation / 100)));

        fprintf(log_file, "buy price = %f", buy_price);

        //-------------------------- Daily sell amount -----------------------------
		double sell_percent, sell_amount_FIFO, sell_amount_LIFO;
		if(i == PERIOD - 1)
			sell_percent = 100;
		else
			sell_percent = rand() % (100 + 1);
		
		average_sell_percent += sell_percent;
		
		sell_amount_FIFO = all_stock_FIFO * (sell_percent / 100);
		sell_amount_LIFO = all_stock_LIFO * (sell_percent / 100);

        fprintf(log_file, " ==== sell percent = %f\n", sell_percent);

        //-------------------------- Process buying day ------------------------------
        enqueueQ(&stock_FIFO, buy_amount);
        pushS(&stock_LIFO, buy_amount);
        all_stock_FIFO += buy_amount;
        all_stock_LIFO += buy_amount;
        enqueueQ(&price_FIFO, buy_price);
        pushS(&price_LIFO, buy_price);
		profit_FIFO -= buy_amount * buy_price;
        profit_LIFO -= buy_amount * buy_price;
		
		fprintf(log_file, "----------------PROCESS FIFO--------------------\n");
		fprintf(log_file, "== FIFO day sell amount = %lf ==\n", sell_amount_FIFO);
		if(balance_after_selling_FIFO > 0)
		{
			fprintf(log_file, "== FIFO sell balance = %lf ---- sell price = %lf ==\n", balance_after_selling_FIFO, balance_price_FIFO);
			sell_amount_FIFO -= balance_after_selling_FIFO;
			all_stock_FIFO -= balance_after_selling_FIFO;
			profit_FIFO += balance_after_selling_FIFO * (balance_price_FIFO * (1 + SELL_SURCHARGE / 100));
			
			if(sell_amount_FIFO < 0)
			{
				balance_after_selling_FIFO = abs(sell_amount_FIFO);
				fprintf(log_file, "== FIFO balance oversold amount = %lf ---- sell price = %lf ==\n", balance_after_selling_FIFO, balance_price_FIFO);
				profit_FIFO -= balance_after_selling_FIFO * (balance_price_FIFO * (1 + SELL_SURCHARGE / 100));
				all_stock_FIFO += balance_after_selling_FIFO;
			}
			else
			{
				balance_after_selling_FIFO = 0;
			}
		}
		
		while(sell_amount_FIFO > 0 && (isEmptyQ(&stock_FIFO) == 0 && isEmptyQ(&price_FIFO) == 0))
		{
			amount = dequeueQ(&stock_FIFO);
			price = dequeueQ(&price_FIFO);
			fprintf(log_file, "== FIFO sell amount = %lf ---- sell price = %lf ==\n", amount, price);
			profit_FIFO += amount * (price * (1 + SELL_SURCHARGE / 100));							///counts profit including overflow of product
			sell_amount_FIFO -= amount;
			all_stock_FIFO -= amount;																///subtracts sold product including overflow
			
			if(sell_amount_FIFO < 0)
			{
				balance_after_selling_FIFO = abs(sell_amount_FIFO);
				fprintf(log_file, "== FIFO oversold amount = %lf ---- sell price = %lf ==\n", balance_after_selling_FIFO, price);
				balance_price_FIFO = price;
				profit_FIFO -= balance_after_selling_FIFO * (balance_price_FIFO * (1 + SELL_SURCHARGE / 100));	///subtracs overflow price 
				all_stock_FIFO += balance_after_selling_FIFO;													///adds overflow product back
			}		
		}
		
		fprintf(log_file, "----------------PROCESS LIFO--------------------\n");
		fprintf(log_file, "== LIFO day sell amount = %lf ==\n", sell_amount_LIFO);
		double stock, price, sum = 0, sum_stock = 0, price_average_LIFO;
		Stack temp_stock, temp_price;
		initializeS(&temp_stock); initializeS(&temp_price);
		
		int size = getSizeS(&price_LIFO);
		for(int i = 0; i < size; ++i)						// count variables for average price equation
		{
			stock = popS(&stock_LIFO);
			price = popS(&price_LIFO);
			pushS(&temp_stock, stock);
			pushS(&temp_price, price);
			
			sum += stock * price;
			sum_stock += stock;
		}
		sum += balance_after_selling_LIFO * balance_price_LIFO;	// | include balance stock and its price
		sum_stock += balance_after_selling_LIFO;				// \
		
		price_average_LIFO = sum / sum_stock;				// count average price
		
		for(int i = 0; i < size; ++i)						// reset stock and price stacks
		{
			pushS(&stock_LIFO, popS(&temp_stock));
			pushS(&price_LIFO, popS(&temp_price));
		}
		destroyS(&temp_stock); destroyS(&temp_price);
		
		if(balance_after_selling_LIFO > 0)
		{
			fprintf(log_file, "== LIFO sell balance = %lf ---- sell price = %lf ==\n", balance_after_selling_LIFO, price_average_LIFO);
			sell_amount_LIFO -= balance_after_selling_LIFO;
			all_stock_LIFO -= balance_after_selling_LIFO;
			profit_LIFO += balance_after_selling_LIFO * (price_average_LIFO * (1 + SELL_SURCHARGE / 100));
			
			if(sell_amount_LIFO < 0)
			{
				balance_after_selling_LIFO = abs(sell_amount_LIFO);
				fprintf(log_file, "== LIFO balance oversold amount = %lf ---- sell price = %lf ==\n", balance_after_selling_LIFO, price_average_LIFO);
				profit_LIFO -= balance_after_selling_LIFO * (price_average_LIFO * (1 + SELL_SURCHARGE / 100));
				all_stock_LIFO += balance_after_selling_LIFO;
			}
			else
			{
				balance_after_selling_LIFO = 0;
			}
		}
		
		while(sell_amount_LIFO > 0 && (isEmptyS(&stock_LIFO) == 0 && isEmptyS(&price_LIFO) == 0))
		{
			amount = popS(&stock_LIFO);
			price = popS(&price_LIFO);
			fprintf(log_file, "== LIFO sell amount = %lf ---- sell price = %lf ==\n", amount, price_average_LIFO);
			profit_LIFO += amount * (price_average_LIFO * (1 + SELL_SURCHARGE / 100));				///counts profit including overflow of product
			sell_amount_LIFO -= amount;
			all_stock_LIFO -= amount;																///subtracts sold product including overflow
			
			if(sell_amount_LIFO < 0)
			{
				balance_after_selling_LIFO = abs(sell_amount_LIFO);
				fprintf(log_file, "== LIFO oversold amount = %lf ---- sell price = %lf ==\n", balance_after_selling_LIFO, price_average_LIFO);
				balance_price_LIFO = price;
				profit_LIFO -= balance_after_selling_LIFO * (price_average_LIFO * (1 + SELL_SURCHARGE / 100));	///subtracs overflow price 
				all_stock_LIFO += balance_after_selling_LIFO;													///adds overflow product back
			}		
		}
		
		fprintf(log_file, "\n\nFIFO_profit = %lf ---- LIFO_profit = %lf\n\n", profit_FIFO, profit_LIFO);
	
	}
	
	fprintf(log_file, "avg sell percent = %lf\n", (average_sell_percent/PERIOD));
	
	destroyQ(&stock_FIFO);
	destroyQ(&price_FIFO);
	
	destroyS(&stock_LIFO);
	destroyS(&price_LIFO);

	fclose(log_file);

	printf("Success\n");
    return 0;
}
