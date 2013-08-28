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
#include <rgbledctrl.h>

class rgbledCollection
{
public:
	rgbledCollection();
	~rgbledCollection();
	void clear();
	void add(rgbledctrl*);
	void remove(uint);
	rgbledctrl* get(uint);
	uint size();
	bool has(rgbledctrl*);
	void print(uint);

private:
	std::vector<rgbledctrl*> rgbLeds;
};

#endif /* RGBLEDCOLLECTION_H_ */
