/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

using System;
using System.Runtime.InteropServices;

namespace LibRgbLedCtrl
{
    static class Rgbledctrl
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct s_rgbVal
        {
            public byte red;
            public byte green;
            public byte blue;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct s_rgbled_deviceSettings
        {
            public byte idleTime;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct s_rgbled_device
        {
            public IntPtr handle; // libUSB handle
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public byte[] version;
            public ushort eepromSize;
            public s_rgbVal rgb;
            public s_rgbled_deviceSettings settings;
        }

        private static IntPtr devicePtr;

        public static void init()
        {
            rgbledctrl_init();
            devicePtr = getDevicePtr();
        }

        public static void getDevice(ref Rgbledctrl.s_rgbled_device dev)
        {
            dev.version = new byte[2];
            dev = (Rgbledctrl.s_rgbled_device)Marshal.PtrToStructure(devicePtr, typeof(Rgbledctrl.s_rgbled_device));
        }

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_init")]
        private static extern void rgbledctrl_init();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_open")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool open();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_close")]
        public static extern void close();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_valid")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool valid();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_poke")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool poke(bool autoReconnect);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_reset")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool reset();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setIdleTime")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setIdleTime(byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setRGB")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setRGB(ref s_rgbVal rgbVal);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setR")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setR(byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setG")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setG(byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_setB")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setB(byte value);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_getDevice")]
        private static extern IntPtr getDevicePtr();

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_eeprom_write")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool eeprom_write(byte value, ushort address);

        [DllImport("librgbledctrl", CallingConvention = CallingConvention.Cdecl, EntryPoint = "rgbledctrl_eeprom_read")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool eeprom_read(ref byte value, ushort address);
    }
}
