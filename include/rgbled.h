/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef RGBLED_H_
#define RGBLED_H_

#include <stdbool.h>
#include <stdint.h>

/**
*Disable LED fade when no USB activity
*/
#define RGBLED_IDLETIME_DISABLE	0

typedef uint16_t eepAddr_t; /**< Unsigned 16bit type for EEPROM addresses */

/** @struct colour_t
* @brief Structure for RGB values
*/
typedef struct{
	uint8_t r; /**< Red */
	uint8_t g; /**< Green */
	uint8_t b; /**< Blue */
}colour_t;

/** @struct rgbled_settings_t
* @brief General device settings
*/
typedef struct{
	uint8_t idleTime; /**< Current idle timeout */
}rgbled_settings_t;

/** @struct rgbled_version_t
* @brief blah
*/
typedef struct{
	uint8_t major; /**< Major */
	uint8_t minor; /**< Minor */
}rgbled_version_t;

/** @struct rgbled_t
* @brief RGB LED Device
*/
typedef struct{
	void*				handle;		/**< Device handle */
	char*				path;		/**< Device path, used to identify the physical device */
	rgbled_version_t	version;	/**< Firmware version */
	eepAddr_t			eepromSize;	/**< EEPROM Size */
	colour_t			colour;		/**< Current RGB value */
	rgbled_settings_t	settings;	/**< Other settings */
}rgbled_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
* Initialise, must be called before anything else!
*
* @return (none)
*/
void rgbled_init(void);

/**
* Shutdown library
*
* @return (none)
*/
void rgbled_exit(void);

/**
* Update list of RGB LED controllers, should be called before opening devices.
*
* @return Number of RGB LED controllers found.
*/
uint32_t rgbled_find(void);

/**
* Checks to see if two RGB LED devices are of the same physical device.
*
* @param [dev1] RGB LED Device 1
* @param [dev2] RGB LED Device 2
* @return true if they are the same physical device, otherwise false
*/
bool rgbled_sameDevice(rgbled_t* dev1, rgbled_t* dev2);

/**
* Open whatever device is found first.
*
* @return rgbled_t pointer if successful, NULL pointer if failed
*/
rgbled_t* rgbled_open(void);

/**
* Open device using its position/index in the list of devices found by rgbledctrl_find().
*
* @param [idx] Index
* @return rgbled_t pointer if successful, NULL pointer if failed
*/
rgbled_t* rgbled_open_byIndex(uint32_t idx);

/**
* Open device that has the specified value at the specified EEPROM address.
*
* @param [value] Value to check for
* @param [address] EEPROM address to look at
* @return rgbled_t pointer if successful, NULL pointer if failed
*/
rgbled_t* rgbled_open_byEEPROM(uint8_t value, eepAddr_t address);

/**
* Close device.
*
* @param [device] Device to operate on
* @return (none)
*/
void rgbled_close(rgbled_t* device);

/**
* Check if the device is still connected.
*
* @param [device] Device to operate on
* @return true if connected, false if not
* @todo Optional auto-reconnect
*/
bool rgbled_poke(rgbled_t* device);

/**
* Reset device (makes the watchdog timer timeout).
*
* @param [device] Device to operate on
* @return true on success, false on failure
*/
bool rgbled_reset(rgbled_t* device);

/**
* Set idle time. Idle time is how long the controller waits (in seconds) before fading out the LEDs when there is no USB activity. RGBLED_IDLETIME_DISABLE = Disabled.
*
* @param [device] Device to operate on
* @param [time] Timeout
* @return true on success, false on failure
*/
bool rgbled_setIdleTime(rgbled_t* device, uint8_t time);

/**
* Set red, green and blue brightnesses in one go
*
* @param [device] Device to operate on
* @param [colour] Pointer to colour_t structure
* @return true on success, false on failure
*/
bool rgbled_setRGB(rgbled_t* device, colour_t* colour);

/**
* Set red brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbled_setR(rgbled_t* device, uint8_t value);

/**
* Set green brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbled_setG(rgbled_t* device, uint8_t value);

/**
* Set blue brightness
*
* @param [device] Device to operate on
* @param [value] Brightness
* @return true on success, false on failure
*/
bool rgbled_setB(rgbled_t* device, uint8_t value);

/**
* Write to device EEPROM
*
* @param [device] Device to operate on
* @param [data] Data to write
* @param [address] EEPROM address to write to
* @return true on success, false on failure
*/
bool rgbled_eeprom_write(rgbled_t* device, uint8_t data, eepAddr_t address);

/**
* Read device EEPROM
*
* @param [device] Device to operate on
* @param [data] Pointer to buffer to write data to
* @param [address] EEPROM address to read
* @return true on success, false on failure
*/
bool rgbled_eeprom_read(rgbled_t* device, uint8_t* data, eepAddr_t address);

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus) || defined(DOXYGEN) // __DOXYGEN__ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/** @class rgbledctrl
* @brief C++ Wrapper class for C library
*/
class rgbled
{
public:
/**
* Initialise, must be called before anything else!
*
* @return (none)
*/
	static void init();

/**
* Shutdown library
*
* @return (none)
*/
	static void exit();

/**
* Update list of RGB LED controllers, should be called before opening devices.
*
* @return Number of RGB LED controllers found.
*/
	static uint32_t find();

/**
* Checks to see if two handles are handles for the same device.
*
* @param [dev1] RGB LED Device 1
* @param [dev2] RGB LED Device 2
* @return true if they are the same physical device, otherwise false
*/
	static bool sameDevice(rgbled* dev1, rgbled* dev2);

/**
* 
*/
	rgbled();

/**
* Closes any handles.
*/
	~rgbled();

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
	bool open(uint32_t idx);

/**
* Open device that has the specified value at the specified EEPROM address.
*
* @param [value] Value to check for
* @param [address] EEPROM address to look at
* @return true on success, false on failure
*/
	bool open(uint8_t value, eepAddr_t address);

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
	bool setIdleTime(uint8_t time);

/**
* Set red, green and blue brightnesses in one go
*
* @param [colour] colour_t structure
* @return true on success, false on failure
*/
	bool setRGB(colour_t& colour);

/**
* Set red brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setR(uint8_t value);

/**
* Set green brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setG(uint8_t value);

/**
* Set blue brightness
*
* @param [value] Brightness
* @return true on success, false on failure
*/
	bool setB(uint8_t value);

/**
* Write to device EEPROM
*
* @param [data] Data to write
* @param [address] EEPROM address to write to
* @return true on success, false on failure
*/
	bool eeprom_write(uint8_t data, eepAddr_t address);

/**
* Read device EEPROM
*
* @param [data] Pointer to buffer to write data to
* @param [address] EEPROM address to read
* @return true on success, false on failure
*/
	bool eeprom_read(uint8_t& data, eepAddr_t address);

/**
*Controller firmware version
*
* @param [version] rgbled_version_t struct to load data to
*/
	void getVersion(rgbled_version_t& version);

/**
*Available EEPROM space
*/
	eepAddr_t getEepromSize();

/**
*Current colour
*
* @param [colour] colour_t structure to load colour to
*/
	void getColour(colour_t& colour);

/**
*Idle timeout (seconds)
*/
	uint8_t getIdleTime();

/**
*Device handle
*/
	void* getHandle();

/**
*blah
*/
	rgbled_t* getDevice();

private:
	rgbled_t* device;
};
#endif

#endif /* RGBLED_H_ */
