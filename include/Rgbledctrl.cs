/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace LibRgbLed
{
    /// <summary>
    /// C# .NET Wrapper class for C library
    /// </summary>
    class Rgbled : IDisposable
    {
        /// <summary>
        /// Disable LED fade when no USB activity.
        /// </summary>
        public const byte RGBLED_IDLETIME_DISABLE = 0;

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct colour_t
        {
            public byte r;
            public byte g;
            public byte b;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct rgbled_settings_t
        {
            public byte idleTime;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct rgbled_version_t
        {
            public byte major;
			public byte minor;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        private struct rgbled_device_t
        {
            public IntPtr handle; // libUSB handle
            public IntPtr path;
            public rgbled_version_t version;
            public ushort eepromSize;
            public colour_t colour;
            public rgbled_settings_t settings;
        }

        private IntPtr devicePtr;

        /// <summary>
        /// Controller firmware version
        /// </summary>
        public Version version
        {
            get
            {
                rgbled_device_t dev = getStruct();
                Version version = new Version(dev.version.major, dev.version.minor);
                return version;
            }
        }

        /// <summary>
        /// Available EEPROM space
        /// </summary>
        public ushort eepromSize
        {
            get { return getStruct().eepromSize; }
        }

        /// <summary>
        /// Current colour
        /// </summary>
        public Color colour
        {
            get
            {
                colour_t colour = getStruct().colour;
                return Color.FromArgb(0, colour.r, colour.g, colour.b);
            }
        }

        /// <summary>
        /// Idle timeout (seconds)
        /// </summary>
        public byte idleTime
        {
            get { return getStruct().settings.idleTime; }
        }

        /// <summary>
        /// LibUSB device handle
        /// </summary>
        public IntPtr handle
        {
            get { return getStruct().handle; }
        }

        private rgbled_device_t getStruct()
        {
            rgbled_device_t dev;
            if (devicePtr != IntPtr.Zero)
                dev = (rgbled_device_t)Marshal.PtrToStructure(devicePtr, typeof(rgbled_device_t));
            else
            {
                dev = new rgbled_device_t();
                dev.version = new rgbled_version_t();
            }
            return dev;
        }

        /// <summary>
        /// Initialise stuff
        /// </summary>
        public Rgbled()
        {
            devicePtr = IntPtr.Zero;
        }

        /// <summary>
        /// Deal with unmanaged memory
        /// </summary>
        ~Rgbled()
        {
            Dispose();
        }

        /// <summary>
        /// Deal with unmanaged memory
        /// </summary>
        public void Dispose()
        {
            // http://stackoverflow.com/questions/538060/proper-use-of-the-idisposable-interface
            //Win32.DestroyHandle(devicePtr);

            close();
			exit();
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Initialises libraries. Should be called before any other stuff.
        /// </summary>
        public static void init()
        {
            rgbled_init();
        }

        /// <summary>
        /// blah
        /// </summary>
        public static void exit()
        {
            rgbled_exit();
        }

        /// <summary>
        /// Updates devices list. Should be called before opening a controller.
        /// </summary>
        /// <returns>Number of RGB LED controllers found.</returns>
        public static uint find()
        {
            return rgbled_find();
        }

        /// <summary>
        /// Checks to see if two devices are of the same physical device
        /// </summary>
        /// <param name="handle1">Device 1</param>
        /// <param name="handle2">Device 2</param>
        /// <returns>true if they are the same physical device, otherwise false</returns>
        public static bool sameDevice(Rgbled dev1, Rgbled dev2)
        {
            return rgbled_sameDevice(dev1.handle, dev2.handle);
        }

        /// <summary>
        /// Open whatever device is found first
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool open()
        {
            devicePtr = rgbled_open();
            return devicePtr != IntPtr.Zero;
        }

        /// <summary>
        /// Open device using its position/index in the list of devices found by find()
        /// </summary>
        /// <param name="idx">Index</param>
        /// <returns>true on success, false on failure</returns>
        public bool open(uint idx)
        {
            devicePtr = rgbled_open_byIndex(idx);
            return devicePtr != IntPtr.Zero;
        }

        /// <summary>
        /// Open device that has the specified value at the specified EEPROM address.
        /// </summary>
        /// <param name="value">Value to check for</param>
        /// <param name="address">EEPROM address to look at</param>
        /// <returns>true on success, false on failure</returns>
        public bool open(byte value, ushort address)
        {
            devicePtr = rgbled_open_byEEPROM(value, address);
            return devicePtr != IntPtr.Zero;
        }

        /// <summary>
        /// Close RGB LED controller.
        /// </summary>
        public void close()
        {
            rgbled_close(devicePtr);
            devicePtr = IntPtr.Zero;
        }

        /// <summary>
        /// Check if the device is still connected
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool poke()
        {
            return rgbled_poke(devicePtr);
        }

        /// <summary>
        /// Reset device (makes the watchdog timer timeout)
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool reset()
        {
            return rgbled_reset(devicePtr);
        }

        /// <summary>
        /// Set idle time. Idle time is how long the controller waits (in seconds) before fading out the LEDs when there is no USB activity. RGBLED_IDLETIME_DISABLE = Disabled.
        /// </summary>
        /// <param name="time"></param>
        /// <returns>true on success, false on failure</returns>
        public bool setIdleTime(byte time)
        {
            return rgbled_setIdleTime(devicePtr, time);
        }

        /// <summary>
        /// Set red, green and blue brightnesses in one go
        /// </summary>
        /// <param name="colour">The colour to set to</param>
        /// <returns>true on success, false on failure</returns>
        public bool setRGB(Color colour)
        {
            colour_t _colour;
            _colour.r = colour.R;
            _colour.g = colour.G;
            _colour.b = colour.B;
            return rgbled_setRGB(devicePtr, ref _colour);
        }

        /// <summary>
        /// Set red LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setR(byte value)
        {
            return rgbled_setR(devicePtr, value);
        }

        /// <summary>
        /// Set green LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setG(byte value)
        {
            return rgbled_setG(devicePtr, value);
        }

        /// <summary>
        /// Set blue LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setB(byte value)
        {
            return rgbled_setB(devicePtr, value);
        }

        /// <summary>
        /// Write a byte to device EEPROM
        /// </summary>
        /// <param name="data">Value to write</param>
        /// <param name="address">EEPROM Address</param>
        /// <returns>true on success, false on failure</returns>
        public bool eeprom_write(byte data, ushort address)
        {
            return rgbled_eeprom_write(devicePtr, data, address);
        }

        /// <summary>
        /// Read a byte from device EEPROM
        /// </summary>
        /// <param name="data"></param>
        /// <param name="address">EEPROM Address</param>
        /// <returns>true on success, false on failure</returns>
        public bool eeprom_read(ref byte data, ushort address)
        {
            return rgbled_eeprom_read(devicePtr, ref data, address);
        }


        //
        // Library import stuff
        //

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_init")]
        private static extern void rgbled_init();

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_exit")]
        private static extern void rgbled_exit();

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_find")]
        private static extern uint rgbled_find();

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_sameDevice")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_sameDevice(IntPtr handle1, IntPtr handle2);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_open")]
        private static extern IntPtr rgbled_open();

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_open_byIndex")]
        private static extern IntPtr rgbled_open_byIndex(uint index);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_open_byEEPROM")]
        private static extern IntPtr rgbled_open_byEEPROM(byte value, ushort address);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_close")]
        private static extern void rgbled_close(IntPtr device);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_poke")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_poke(IntPtr device);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_reset")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_reset(IntPtr device);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_setIdleTime")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_setIdleTime(IntPtr device, byte value);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_setRGB")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_setRGB(IntPtr device, ref colour_t colour);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_setR")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_setR(IntPtr device, byte value);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_setG")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_setG(IntPtr device, byte value);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_setB")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_setB(IntPtr device, byte value);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_eeprom_write")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_eeprom_write(IntPtr device, byte value, ushort address);

        [DllImport("librgbled", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbled_eeprom_read")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbled_eeprom_read(IntPtr device, ref byte value, ushort address);
    }
}
