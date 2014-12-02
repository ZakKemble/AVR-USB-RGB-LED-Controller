Dev branch. Working on converting the libUSB stuff to HID so drivers aren't required on Windows. Using <a href="https://github.com/signal11/hidapi">HIDAPI</a>.  

librgbledctrl renamed to librgbled

Everything seems to work on Windows with just a few minor workarounds for some things. Linux still being worked on.


HIDAPI Issues:

Windows:  
open_device() fails if already open (this is ok since usually only 1 thing uses a device) See: https://github.com/signal11/hidapi/issues/138  
hid_get_feature_report() returns data length + 1 for ID, but data starts at [0] and last byte is untouched (docs say data starts at [1] and [0] is the report ID)  
Having a report type INPUT with hid_read() (or hid_write(), whichever youre supposed to use for INPUT, cant remember) causes bits of data to be lost, high bus activity, generally gets all messed up (workaround = use FEATURE type instead)  


Linux:
HIDRAW backend doesn't see device at all, have to use libusb backend  
open_device() works if already open, but writing/reading fails with "usb 2-2.1: usbfs: process 24733 (rndflasher) did not claim interface 0 before use" (not much of an issue since you'd normally only have a device opened once)  
Enumerate stuff also hangs after the error above, device needs to be reconnected  
If a device is open, calls to hid_enumerate(0, 0) to look for more devices will often mess things up, see above  
hid_get_feature_report() returns data length without + 1 for ID. Data starts at [0] (docs say different)  


Windows is Windows 7 x64  
Linux is Debian 7.1 running on VMware.
