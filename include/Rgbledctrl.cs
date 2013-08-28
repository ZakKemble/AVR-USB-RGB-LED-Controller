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

namespace LibRgbLedCtrl
{
    /// <summary>
    /// C# .NET Wrapper class for C library
    /// </summary>
    class Rgbledctrl : IDisposable
    {
        /// <summary>
        /// Disable LED fade when no USB activity.
        /// </summary>
        public const byte RGBLED_IDLETIME_DISABLE = 0;

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct s_rgbVal
        {
            public byte red;
            public byte green;
            public byte blue;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct s_rgbled_deviceSettings
        {
            public byte idleTime;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        private struct s_rgbled_device
        {
            public IntPtr handle; // libUSB handle
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)] // version is always 2 bytes
            public byte[] version;
            public ushort eepromSize;
            public s_rgbVal rgb;
            public s_rgbled_deviceSettings settings;
        }

        private IntPtr devicePtr;

        /// <summary>
        /// Controller firmware version
        /// </summary>
        public byte[] version
        {
            get { return getStruct().version; }
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
                s_rgbVal rgb = getStruct().rgb;
                return Color.FromArgb(0, rgb.red, rgb.green, rgb.blue);
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

        private s_rgbled_device getStruct()
        {
            s_rgbled_device dev;
            if (devicePtr != IntPtr.Zero)
                dev = (s_rgbled_device)Marshal.PtrToStructure(devicePtr, typeof(s_rgbled_device));
            else
            {
                dev = new s_rgbled_device();
                dev.version = new byte[2];
            }
            return dev;
        }

        /// <summary>
        /// Initialise stuff
        /// </summary>
        public Rgbledctrl()
        {
            devicePtr = IntPtr.Zero;
        }

        /// <summary>
        /// Deal with unmanaged memory
        /// </summary>
        ~Rgbledctrl()
        {
            Dispose();
        }

        /// <summary>
        /// Deal with unmanaged memory
        /// </summary>
        public void Dispose()
        {
            close();
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Initialises libraries. Should be called before any other stuff.
        /// </summary>
        public static void init()
        {
            rgbledctrl_init();
        }

        /// <summary>
        /// Updates devices list. Should be called before opening a controller.
        /// </summary>
        /// <returns>Number of RGB LED controllers found.</returns>
        public static uint find()
        {
            return rgbledctrl_find();
        }

        /// <summary>
        /// Checks to see if two handles are handles for the same device
        /// </summary>
        /// <param name="handle1">Device handle 1</param>
        /// <param name="handle2">Device handle 2</param>
        /// <returns>true if they are handles for the same device, otherwise false</returns>
        public static bool sameDevice(IntPtr handle1, IntPtr handle2)
        {
            return rgbledctrl_sameDevice(handle1, handle2);
        }

        /// <summary>
        /// Open whatever device is found first
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool open()
        {
            devicePtr = rgbledctrl_open();
            return devicePtr != IntPtr.Zero;
        }

        /// <summary>
        /// Open device using its position/index in the list of devices found by find()
        /// </summary>
        /// <param name="idx">Index</param>
        /// <returns>true on success, false on failure</returns>
        public bool open(uint idx)
        {
            devicePtr = rgbledctrl_open_byIndex(idx);
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
            devicePtr = rgbledctrl_open_byEEPROM(value, address);
            return devicePtr != IntPtr.Zero;
        }

        /// <summary>
        /// Close RGB LED controller.
        /// </summary>
        public void close()
        {
            rgbledctrl_close(devicePtr);
            devicePtr = IntPtr.Zero;
        }

        /// <summary>
        /// Check if the device is still connected
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool poke()
        {
            return rgbledctrl_poke(devicePtr);
        }

        /// <summary>
        /// Reset device (makes the watchdog timer timeout)
        /// </summary>
        /// <returns>true on success, false on failure</returns>
        public bool reset()
        {
            return rgbledctrl_reset(devicePtr);
        }

        /// <summary>
        /// Set idle time. Idle time is how long the controller waits (in seconds) before fading out the LEDs when there is no USB activity. RGBLED_IDLETIME_DISABLE = Disabled.
        /// </summary>
        /// <param name="time"></param>
        /// <returns>true on success, false on failure</returns>
        public bool setIdleTime(byte time)
        {
            return rgbledctrl_setIdleTime(devicePtr, time);
        }

        /// <summary>
        /// Set red, green and blue brightnesses in one go
        /// </summary>
        /// <param name="colour">The colour to set to</param>
        /// <returns>true on success, false on failure</returns>
        public bool setRGB(Color colour)
        {
            s_rgbVal rgb;
            rgb.red = colour.R;
            rgb.green = colour.G;
            rgb.blue = colour.B;
            return rgbledctrl_setRGB(devicePtr, ref rgb);
        }

        /// <summary>
        /// Set red LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setR(byte value)
        {
            return rgbledctrl_setR(devicePtr, value);
        }

        /// <summary>
        /// Set green LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setG(byte value)
        {
            return rgbledctrl_setG(devicePtr, value);
        }

        /// <summary>
        /// Set blue LED brightness
        /// </summary>
        /// <param name="value">Brightness</param>
        /// <returns>true on success, false on failure</returns>
        public bool setB(byte value)
        {
            return rgbledctrl_setB(devicePtr, value);
        }

        /// <summary>
        /// Write a byte to device EEPROM
        /// </summary>
        /// <param name="data">Value to write</param>
        /// <param name="address">EEPROM Address</param>
        /// <returns>true on success, false on failure</returns>
        public bool eeprom_write(byte data, ushort address)
        {
            return rgbledctrl_eeprom_write(devicePtr, data, address);
        }

        /// <summary>
        /// Read a byte from device EEPROM
        /// </summary>
        /// <param name="data"></param>
        /// <param name="address">EEPROM Address</param>
        /// <returns>true on success, false on failure</returns>
        public bool eeprom_read(ref byte data, ushort address)
        {
            return rgbledctrl_eeprom_read(devicePtr, ref data, address);
        }


        //
        // Library import stuff
        //

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_init")]
        private static extern void rgbledctrl_init();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_find")]
        private static extern uint rgbledctrl_find();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_sameDevice")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_sameDevice(IntPtr handle1, IntPtr handle2);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_open")]
        private static extern IntPtr rgbledctrl_open();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_open_byIndex")]
        private static extern IntPtr rgbledctrl_open_byIndex(uint index);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_open_byEEPROM")]
        private static extern IntPtr rgbledctrl_open_byEEPROM(byte value, ushort address);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_close")]
        private static extern void rgbledctrl_close(IntPtr device);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_poke")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_poke(IntPtr device);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_reset")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_reset(IntPtr device);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setIdleTime")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_setIdleTime(IntPtr device, byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setRGB")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_setRGB(IntPtr device, ref s_rgbVal rgbVal);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setR")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_setR(IntPtr device, byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setG")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_setG(IntPtr device, byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setB")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_setB(IntPtr device, byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_eeprom_write")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_eeprom_write(IntPtr device, byte value, ushort address);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_eeprom_read")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool rgbledctrl_eeprom_read(IntPtr device, ref byte value, ushort address);
    }
}
