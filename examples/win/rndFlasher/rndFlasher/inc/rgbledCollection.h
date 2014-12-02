/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef RGBLEDCOLLECTION_H_
#define RGBLEDCOLLECTION_H_

#include <vector>
#include <rgbled.h>

class rgbledCollection
{
public:
	rgbledCollection();
	~rgbledCollection();
	void clear();
	void add(rgbled*);
	void remove(uint32_t);
	void erase(uint32_t);
	rgbled* get(uint32_t);
	uint32_t size();
	bool has(rgbled*);
	void print(uint32_t);

private:
	std::vector<rgbled*> rgbLeds;
};

#endif /* RGBLEDCOLLECTION_H_ */
