#include<stdio.h>
#include<string.h>                                                                 
#include<stdbool.h>
#include<data.h>                                                               
#include<main.h>

extern int *no_of_notes_user;
extern item_list_e item[];

/****************************************************************************************
*                                                                                       *
* Param : void                                                                          *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Initialize the data if required.                                        *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void init_data()
{
	// initialize no of notes of each currency
	init_no_of_notes();
}

/****************************************************************************************
*                                                                                       *
* Param : items[] - array of items purchased                                            *
*         no_of_items - no of items purchased                                           *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Displays the list of items purchsed by user.                            *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void disp_purchased_items(int items[], int no_of_items)
{
	item_list_e *item_handle = NULL;
	int i = 0;

	printf("\nList of items you have purchased : \n\n");
	for(; i < no_of_items; i++)
	{
		item_handle = get_item_handle(items[i]);
		printf("%s\n", get_item_name(item_handle));
	}
}

/****************************************************************************************
*                                                                                       *
* Param : remaining_balance - the balance remaining to be returned to user.             *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Calculates how many notes of particular denomination to be returned to  *
*               user and accordingly update the notes in the database.                  *
*                                                                                       *
****************************************************************************************/
void return_remaining_balance(int remaining_balance)
{
	int note = DENOMINATION_1000;
	int no_of_notes = 0;
	int no_of_return_notes = 0;
	int return_notes[DENOMINATION_MAX] = {0};

	printf("\nPlease take back your remaining balance.\n");
	for(; note > -1 ; note--)
	{
		if(!remaining_balance)
			break;

		// Count the number of notes of "note" currency to be returned to user.
		no_of_notes = remaining_balance/map_enum_to_note(note);
		if(!no_of_notes)
			continue;

		no_of_return_notes = no_of_notes;

		if(no_of_notes <= no_of_notes_user[note])	// If user entered same or more no of notes then add remaining notes to database.
		{
			no_of_notes = update_notes_in_database(note, no_of_notes_user[note] - no_of_notes, true);
		}
		else	// If user entered less notes then check database and return from the database notes.
		{
			no_of_notes -= no_of_notes_user[note];
			no_of_notes = update_notes_in_database(note, no_of_notes, false);
		}

		remaining_balance -= ((no_of_return_notes - no_of_notes) * map_enum_to_note(note));
		return_notes[note] = no_of_return_notes - no_of_notes;
	}

	if(remaining_balance)
	{
		printf("\nSorry not enough notes available in system\n");
		for(note = DENOMINATION_1000; note > -1 ; note--)
		{
			if(no_of_notes_user[note])
				update_notes_in_database(note, return_notes[note], false);
		}
	}
	else
	{
		for(note = DENOMINATION_1000; note > -1; note--)
		{
			if(return_notes[note])
				printf("\nNo of notes = %d of denomination = %d\n", return_notes[note], map_enum_to_note(note));

			if(no_of_notes_user[note])
				update_notes_in_database(note, no_of_notes_user[note] - return_notes[note], true);
		}
	}
}

/****************************************************************************************
*                                                                                       *
* Param : void                                                                          *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Displays the list of items available.                                   *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void disp_items()
{
	int i = 0;
	printf("\n----------------------------------------------------------------\n");
	printf("\n Item Name  Item Code  Item Price Item quantity\n");
	for(; i < MAX_ITEMS; i++)
	{
		if(item[i].quantity)
			printf("\n %s  %d  %d  %d \n", item[i].name, item[i].code, item[i].price, item[i].quantity);
	}
	printf("\n----------------------------------------------------------------\n");
}

/****************************************************************************************
*                                                                                       *
* Param : total - Total amount entered by user.                                         *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : User selects the items that he wants to buy.                            *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int disp_item_menu(int *remaining_balance, int user_items[])
{
	int user_input = 0;
	int i = 0;
	item_list_e *item_handle = NULL;

	printf("\nStart entering the item code of the item you want to buy.\n");
	printf("\nPress 0 anytime in between to finish your purchase.\n");

	while(true)
	{
		disp_items();

		printf("\nEnter item code : ");
		if(scanf("%d",&user_input) != 1)
		{
			PRINT(ERR, "Non numeric value encountered");
			while(getchar() != '\n');
			continue;
		}
		else if(user_input >= ITEM_CODE_MAX)
		{
			PRINT(ERR, "Wrong item code.");
			continue;
		}

		if(user_input == 0)
			break;

		item_handle = get_item_handle(user_input);
		if(!item_handle)
		{
			PRINT(ERR, "Something went wrong..... Please try again");
			continue;
		}

		if(!check_item_availability(item_handle))
		{
			PRINT(ERR,"Selected item is out of stock.");
			continue;
		}

		if(get_item_price(item_handle) > *remaining_balance)
		{
			PRINT(ERR,"Selected item price is more than the remaining balance. Please select other item within remaining balance");
			continue;
		}

		*remaining_balance -= get_item_price(item_handle);
		user_items[i] = user_input;
		update_item_quantity(&item_handle);
		i++;

		printf("\nBalnce remaining is = %d\n", *remaining_balance);
	}

	return i;
}

/****************************************************************************************
*                                                                                       *
* Param : void                                                                          *
*                                                                                       *
* Return : Total amount entered by user.                                                *
*                                                                                       *
* Description : Takes valid denomination notes as input from user.                      *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int display_curr_menu()
{                                                                                  
	int user_input = 0;
	note_type_e input_note = DENOMINATION_MAX;
	int total_amount = 0;

	printf("\nStart entering notes in denomination of :\n");
	printf("\n Rs 10 \n Rs 20 \n Rs 50 \n Rs 100 \n Rs 500 \n Rs 1000 \n");
	printf("\nYou can stop anytime by entering '0'\n");

	while(true)
	{
		printf("\nEnter currency notes : ");
		if(scanf("%d",&user_input) != 1)
		{
			PRINT(ERR, "Non numeric value encountered");
			while(getchar() != '\n');
			continue;
		}

		if(user_input == 0)
			break;
		
		input_note = map_user_input_to_enum(user_input);

		if(input_note == DENOMINATION_MAX)
		{
			PRINT(ERR, "Denomination type is note correct");
		}
		else
		{
			total_amount += user_input;
			no_of_notes_user[input_note]++;
			printf("\nAmount Entered = %d . Total Amount = %d\n", user_input, total_amount);
		}

	}

	printf("\n\n\nTotal amount entered by you = %d\n", total_amount);
	return total_amount;
}

void start_app()
{
	int total;
	int user_items[50];
	int no_of_items_bought = 0;
	int remaining_balance = 0;

	total = display_curr_menu();
	remaining_balance = total;

	no_of_items_bought = disp_item_menu(&remaining_balance, user_items);

	disp_purchased_items(user_items, no_of_items_bought);
	printf("\nBalnce remaining after purchasing is = %d\n", remaining_balance);

	return_remaining_balance(remaining_balance);
}

/****************************************************************************************
*                                                                                       *
* Param : void                                                                          *
*                                                                                       *
* Return : status                                                                       *
*                                                                                       *
* Description : Driver function.                                                        *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int main()                                                                      
{
	printf("\n **************************************************** \n");
	printf("\n *                                                  * \n");
	printf("\n *        Welcome to the vending mechine            * \n");
	printf("\n *                                                  * \n");
	printf("\n **************************************************** \n");
	
	// initialize all the data first
	init_data();

	// start the application.
	start_app();

	getchar();
	return 0;
}
