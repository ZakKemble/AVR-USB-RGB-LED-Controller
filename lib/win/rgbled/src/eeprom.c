/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */
/*
#include <time.h>
#include "eeprom.h"
#include "rgbledctrl.h"

#define CACHETIME 4

typedef struct node{
	struct node* left;
	struct node* right;
	eepAddr_t address;
	byte data;
	time_t modified;
}s_node;

static s_node root;

static void visitNode(s_node*);

void eeprom_write(byte data, eepAddr_t address)
{
	rgbledctrl_eeprom_write(data, address);

	root.address = address;
	root.data = data;
	root.modified = time(NULL);
}

byte eeprom_read(eepAddr_t address)
{
	byte data;
	data = rgbledctrl_eeprom_read(address);

	return data;
}

// Commit any EEPROM data that hasn't been touched for a while
void eeprom_commit()
{
	visitNode(&root);
}

static void visitNode(s_node* node)
{
	if(node->left != NULL)
		visitNode(node->left);

	if(node->right != NULL)
		visitNode(node->right);

	if(node->left == NULL && node->right == NULL)
	{
		time_t now = time(NULL);
		if(difftime(now, node->modified) >= CACHETIME)
		{

		}
	}
}

void eeprom_clear()
{

}
*/
