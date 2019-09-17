#include<data.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

// Array to store no of notes of each denomination
int *no_of_notes;

// Array to store no of notes of each denomination entered by user
int *no_of_notes_user;

// Basic items available
item_list_e item[] =
{
	{ .name = "Item1" , .code = ITEM_CODE_1 , .price = 10 , .quantity = 2 },
	{ .name = "Item2" , .code = ITEM_CODE_2 , .price = 20 , .quantity = 3 },
	{ .name = "Item3" , .code = ITEM_CODE_3 , .price = 30 , .quantity = 4 },
	{ .name = "Item4" , .code = ITEM_CODE_4 , .price = 70 , .quantity = 2 },
	{ .name = "Item5" , .code = ITEM_CODE_5 , .price = 150 , .quantity = 7 },
	{ .name = "Item6" , .code = ITEM_CODE_6 , .price = 200 , .quantity = 1 }
};

/****************************************************************************************
*                                                                                       *
* Param : void                                                                          *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Initialize no of notes in system with default value.                    *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void init_no_of_notes()
{
	int index = 0;

	no_of_notes = malloc(sizeof(int) * DENOMINATION_MAX);
	if(!no_of_notes)
	{
		printf("\nMemory allocation failed.");
		return;
	}
	
	no_of_notes_user = malloc(sizeof(int) * DENOMINATION_MAX);
	if(!no_of_notes_user)
	{
		printf("\nMemory allocation failed.");
		return;
	}

	for(; index < DENOMINATION_MAX; index++)
		no_of_notes[index] = INITIAL_NOTES;
}

/****************************************************************************************
*                                                                                       *
* Param : note - denomination type.                                                     *
*                                                                                       *
* Return : number of notes of note type denomination.                                   *
*                                                                                       *
* Description : Returns the no of notes of note type denomination.                      *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int get_no_of_notes(note_type_e note)
{
	return no_of_notes[note];
}

/****************************************************************************************
*                                                                                       *
* Param : note - denomination type.                                                     *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Update the no of notes of denomination type note in the system.         *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void set_no_of_notes(note_type_e note)
{
	no_of_notes[note]++;
}

/****************************************************************************************
*                                                                                       *
* Param : note - Denomination type.                                                     *
*                                                                                       *
* Return : valid currency or not.                                                       *
*                                                                                       *
* Description : Checks if the note entered by user is valid or not.                     *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
bool is_valid_note_type(note_type_e note)
{
	switch(note)
	{
		case DENOMINATION_10:
		case DENOMINATION_20:
		case DENOMINATION_50:
		case DENOMINATION_100:
		case DENOMINATION_500:
		case DENOMINATION_1000:
			 return true;
		default :
			return false;
	}
}

/****************************************************************************************
*                                                                                       *
* Param : note - Denomination type.                                                     *
*                                                                                       *
* Return : Note value.                                                                  *
*                                                                                       *
* Description : Returns the value of the corresponding note.                            *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int map_enum_to_note(note_type_e note)
{
	switch(note)
	{
		case DENOMINATION_10: return 10;
		case DENOMINATION_20: return 20;
		case DENOMINATION_50: return 50;
		case DENOMINATION_100: return 100;
		case DENOMINATION_500: return 500;
		case DENOMINATION_1000: return 1000;
		default :  return 0;
	}
}

/****************************************************************************************
*                                                                                       *
* Param : currency - note value.                                                        *
*                                                                                       *
* Return : Enum value.                                                                  *
*                                                                                       *
* Description : Returns the enum value corresponding to the note value.                 *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
note_type_e map_user_input_to_enum(int currency)
{
	switch(currency)
	{
		case 10 : return DENOMINATION_10;
		case 20 : return DENOMINATION_20;
		case 50 : return DENOMINATION_50;
		case 100 : return DENOMINATION_100;
		case 500 : return DENOMINATION_500;
		case 1000 : return DENOMINATION_1000;
		default : return DENOMINATION_MAX;
	}
}

/****************************************************************************************
*                                                                                       *
* Param : code - item code.                                                             *
*                                                                                       *
* Return : Address of the item array.                                                   *
*                                                                                       *
* Description : Returns the address of the item array corresponding to the item code.   *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
item_list_e* get_item_handle(item_code_e code)
{
	int i = 0;

	for(; i < MAX_ITEMS; i++)
	{
		if(item[i].code == code)
		{
			return &item[i];
		}
	}
	return NULL;
}

/****************************************************************************************
*                                                                                       *
* Param : item - handle of the item.                                                    *
*                                                                                       *
* Return : Item available or not.                                                       *
*                                                                                       *
* Description : Checks availability of the item.                                        *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
bool check_item_availability(item_list_e *item)
{
	if(!item)
	{
		printf("%s : %d : NULL value received", __func__, __LINE__);
		return false;
	}

	if(item->quantity > 0)
		return true;
	else
		return false;
}

/****************************************************************************************
*                                                                                       *
* Param : item - handle of the item.                                                    *
*                                                                                       *
* Return : price of item.                                                               *
*                                                                                       *
* Description : Returns the price of the item.                                          *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int get_item_price(item_list_e *item)
{
	if(!item)
	{
		printf("%s : %d : NULL value received", __func__, __LINE__);
		return FAILURE;
	}

	return item->price;
}

/****************************************************************************************
*                                                                                       *
* Param : item - handle of item.                                                        *
*                                                                                       *
* Return : void                                                                         *
*                                                                                       *
* Description : Update the quantity of the item in system.                              *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
void update_item_quantity(item_list_e **item)
{
	if(!(*item))
	{
		printf("%s : %d : NULL value received", __func__, __LINE__);
		return;
	}

	((*item)->quantity)--;
}

/****************************************************************************************
*                                                                                       *
* Param : item - handle of item.                                                        *
*                                                                                       *
* Return : item name.                                                                   *
*                                                                                       *
* Description : Returns the item name.                                                  *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
char* get_item_name(item_list_e *item)
{
	if(!item)
	{
		printf("%s : %d : NULL value received", __func__, __LINE__);
		return NULL;
	}
	return item->name;
}

/****************************************************************************************
*                                                                                       *
* Param : note_t - Denomination type.                                                   *
*         notes - no of notes.                                                          *
*         is_update - add to or remove from database.                                   *
*                                                                                       *
* Return : no of notes of note_t type not available in system.                          *
*                                                                                       *
* Description : Update the notes in database after returning remaining balance to user. *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
int update_notes_in_database(note_type_e note_t, int notes, bool is_update)
{
	if(is_update)
	{
		no_of_notes[note_t] += notes;
		return 0;
	}
	else
	{
		if(no_of_notes[note_t] >= notes)
		{
			no_of_notes[note_t] -= notes;
			return 0;
		}
		else
		{
			notes -= no_of_notes[note_t];
			return notes;
		}
	}
}
