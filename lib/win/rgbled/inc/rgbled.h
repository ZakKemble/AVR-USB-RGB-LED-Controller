/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef RGBLEDCTRL_H_
#define RGBLEDCTRL_H_

#ifdef _WIN32
#include <lusb0_usb.h>
#else
#include <usb.h>
#endif
#include <stdbool.h>
#include <stdint.h>

/**
*Disable LED fade when no USB activity
*/
#define RGBLED_IDLETIME_DISABLE	0

typedef unsigned char byte; /**< Typedef for unsigned char */
typedef unsigned int uint; /**< Typedef for unsigned int */
typedef uint16_t eepAddr_t; /**< Unsigned 16bit type for EEPROM addresses */

/** @struct s_rgbVal
* @brief Structure for RGB values
*/
typedef struct{
	byte red; /**< Red */
	byte green; /**< Green */
	byte blue; /**< Blue */
}s_rgbVal;

/** @struct s_rgbled_deviceSettings
* @brief General device settings
*/
typedef struct{
	byte idleTime; /**< Current idle timeout */
}s_rgbled_deviceSettings;

/** @struct s_rgbled_device
* @brief RGB LED Device
* @todo Add usb_device* attribute?
*/
typedef struct{
	usb_dev_handle* handle; /**< USB Handle */
	byte version[2]; /**< Firmware version */
	eepAddr_t eepromSize; /**< EEPROM Size */
	s_rgbVal rgb; /**< Current RGB value */
	s_rgbled_deviceSettings settings; /**< Other settings */
}s_rgbled_device;

#if defined(__cplusplus)
extern "C" {
#endif

/**
* Initialise, must be called before anything else!
*
* @return (none)
*/
void rgbledctrl_init(void);

/**
* Update list of RGB LED controllers, should be called before opening devices.
*
* @return Number of RGB LED controllers found.
*/
uint rgbledctrl_find(void);

/**
* Checks to see if two handles are handles for the same device.
*
* @param [handle1] Handle 1
* @param [handle2] Handle 2
* @return true if they are handles for the same device, otherwise false
*/
bool rgbledctrl_sameDevice(usb_dev_handle* handle1, usb_dev_handle* handle2);

/**
* Open whatever device is found first.
*
* @return s_rgbled_device pointer if successful, NULL pointer if failed
*/
s_rgbled_device* rgbledctrl_open(void);

/**
* Open device using its position/index in the list of devices found by rgbledctrl_find().
*
* @param [idx] Index
* @return s_rgbled_device pointer if successful, NULL pointer if failed
*/
s_rgbled_device* rgbledctrl_open_byIndex(uint idx);

/**
* Open device that has the specified value at the specified EEPROM address.
*
* @param [value] Value to check for
* @param [address] EEPROM address to look at
* @return s_rgbled_device pointer if successful, NULL pointer if failed
*/
s_rgbled_device* rgbledctrl_open_byEEPROM(byte value, eepAddr_t address);

/**
* Close device.
*
* @param [device] Device to operate on
* @return (none)
*/
void rgbledctrl_close(s_rgbled_device* device);

/**
* Check if the device is still connected.
*
* @param [device] Device to operate on
* @return true if connected, false if not
* @todo Optional auto-reconnect
*/
bool rgbledctrl_poke(s_rgbled_device* device);

/**
* Reset device (makes the watchdog timer timeout).
*
* @param [device] Device to operate on
* @return true on success, false on failure
*/
bool rgbledctrl_reset(s_rgbled_device* device);

/**
* Set idle time. Idle time is how long the controller waits (in seconds) before fading out the LEDs when there is no USB activity. RGBLED_IDLETIME_DISABLE = Disabled.
*
* @param [device] Device to operate on
* @param [time] Timeout
* @return true on success, false on failure
*/
bool rgbledctrl_setIdleTime(s_rgbled_device* device, byte time);

/**
* Set red, green and blue brightnesses in one go
*
* @param [device] Device to operate on
* @param [colour] Pointer to s_rgbVal structure
* @return true on success, false on failure
*/
bool rgbledctrl_setRGB(s_rgbled_device* device, s_rgbVal* colour);

/**
* Set red brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbledctrl_setR(s_rgbled_device* device, byte value);

/**
* Set green brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbledctrl_setG(s_rgbled_device* device, byte value);

/**
* Set blue brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbledctrl_setB(s_rgbled_device* device, byte value);

/**
* Write to device EEPROM
*
* @param [device] Device to operate on
* @param [data] Data to write
* @param [address] EEPROM address to write to
* @return true on success, false on failure
*/
bool rgbledctrl_eeprom_write(s_rgbled_device* device, byte data, eepAddr_t address);

/**
* Read device EEPROM
*
* @param [device] Device to operate on
* @param [data] Pointer to buffer to write data to
* @param [address] EEPROM address to read
* @return true on success, false on failure
*/
bool rgbledctrl_eeprom_read(s_rgbled_device* device, byte* data, eepAddr_t address);

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus) || defined(DOXYGEN)
/** @class rgbledctrl
* @brief C++ Wrapper class for C library
*/
class rgbledctrl
{
public:
/**
* Initialise, must be called before anything else!
*
* @return (none)
*/
	static void init();

/**
* Update list of RGB LED controllers, should be called before opening devices.
*
* @return Number of RGB LED controllers found.
*/
	static uint find();

/**
* Checks to see if two handles are handles for the same device.
*
* @param [handle1] Handle 1
* @param [handle2] Handle 2
* @return true if they are handles for the same device, otherwise false
*/
	static bool sameDevice(usb_dev_handle* handle1, usb_dev_handle* handle2);

/**
* 
*/
	rgbledctrl();

/**
* Closes any handles.
*/
	~rgbledctrl();

/**
* Open whatever device is found first.
*
* @return true on success, false on failure
*/
	bool open();

/**
* Open device using its position/index in the list of devices found by find().
*
* @param [idx] Index
* @return true on success, false on failure
*/
	bool open(uint idx);

/**
* Open device that has the specified value at the specified EEPROM address.
*
* @param [value] Value to check for
* @param [address] EEPROM address to look at
* @return true on success, false on failure
*/
	bool open(byte value, eepAddr_t address);

/**
* Close device.
*
* @return (none)
*/
	void close();

/**
* Check if the device is still connected.
*
* @return true if connected, false if not
*/
	bool poke();

/**
* Reset device (makes the watchdog timer timeout).
*
* @return true on success, false on failure
*/
	bool reset();

/**
* Set idle time. Idle time is how long the controller waits (in seconds) before fading out the LEDs when there is no USB activity. RGBLED_IDLETIME_DISABLE = Disabled.
*
* @param [time] Timeout
* @return true on success, false on failure
*/
	bool setIdleTime(byte time);

/**
* Set red, green and blue brightnesses in one go
*
* @param [colour] s_rgbVal structure
* @return true on success, false on failure
*/
	bool setRGB(s_rgbVal& colour);

/**
* Set red brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setR(byte value);

/**
* Set green brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setG(byte value);

/**
* Set blue brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setB(byte value);

/**
* Write to device EEPROM
*
* @param [data] Data to write
* @param [address] EEPROM address to write to
* @return true on success, false on failure
*/
	bool eeprom_write(byte data, eepAddr_t address);

/**
* Read device EEPROM
*
* @param [data] Pointer to buffer to write data to
* @param [address] EEPROM address to read
* @return true on success, false on failure
*/
	bool eeprom_read(byte& data, eepAddr_t address);

/**
*Controller firmware version
*
* @param [version] Byte[2] array to load data to
*/
	void getVersion(byte (&version)[2]);

/**
*Available EEPROM space
*/
	eepAddr_t getEepromSize();

/**
*Current colour
*
* @param [colour] s_rgbVal structure to load colour to
*/
	void getColour(s_rgbVal& colour);

/**
*Idle timeout (seconds)
*/
	byte getIdleTime();

/**
*LibUSB device handle
*/
	usb_dev_handle* getHandle();

private:
	s_rgbled_device* device;
};
#endif

#endif /* RGBLEDCTRL_H_ */
