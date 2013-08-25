/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Threading;
using System.Windows.Forms;
using LibRgbLedCtrl;

namespace rgbled
{
    public partial class Form1 : Form
    {
        private const double GAMMA_CORRECTION = 2.2;
        private bool ignoreEvent = false;

        public Form1()
        {
            InitializeComponent();

            var assembly = System.Reflection.Assembly.GetExecutingAssembly();
            Version version = assembly.GetName().Version;

            Icon = Icon.ExtractAssociatedIcon(assembly.Location);
            Text = String.Format("RGB LED Control v{0}.{1}", version.Major, version.Minor);

            setTrackBarStuff(tbRed);
            setTrackBarStuff(tbGreen);
            setTrackBarStuff(tbBlue);
        }

        private void setTrackBarStuff(TrackBar tb)
        {
            tb.Maximum = 255;
            tb.TickFrequency = 25;
            tb.SmallChange = 5;
            tb.LargeChange = 25;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Must be called before any other Rgbledctrl stuff!
            Rgbledctrl.init();

            // 
            Thread t = new Thread(new ThreadStart(tCapScreen));
            t.IsBackground = true;
            t.Start();
        }

        private void writeToLog(string str)
        {
            txtLog.AppendText(str + Environment.NewLine);
        }

        // Open
        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (Rgbledctrl.open())
                writeToLog("Device opened");
            else
                writeToLog("Failed to open device");
            writeToLog("------");
        }

        // Get info
        private void btnInfo_Click(object sender, EventArgs e)
        {
            if (!Rgbledctrl.poke(false))
            {
                writeToLog("Device not available");
                writeToLog("------");
                return;
            }

            Rgbledctrl.s_rgbled_device device = new Rgbledctrl.s_rgbled_device();
            Rgbledctrl.getDevice(ref device);

            writeToLog(String.Format("Firmware version: {0}.{1}", device.version[0], device.version[1]));
            writeToLog(String.Format("User EEPROM size: {0}", device.eepromSize));
            writeToLog(String.Format("Red: {0}", device.rgb.red));
            writeToLog(String.Format("Green: {0}", device.rgb.green));
            writeToLog(String.Format("Blue: {0}", device.rgb.blue));
            writeToLog(String.Format("Idle time: {0}", device.settings.idleTime));
            writeToLog("------");
        }

        // Close
        private void btnClose_Click(object sender, EventArgs e)
        {
            Rgbledctrl.close();
            writeToLog("Device closed");
            writeToLog("------");
        }

        // Reset device
        private void btnReset_Click(object sender, EventArgs e)
        {
            Rgbledctrl.reset();
            writeToLog("Device reset");
            writeToLog("------");
        }

        // Set idle time
        private void btnIdleTime_Click(object sender, EventArgs e)
        {
            if(Rgbledctrl.setIdleTime((byte)nudIdleTime.Value))
                writeToLog("Idle time set");
            else
                writeToLog("Device not available");
            writeToLog("------");
        }

        // Trackbar change
        private void tb_Scroll(object sender, EventArgs e)
        {
            if (ignoreEvent)
                return;

            byte red = (byte)tbRed.Value;
            byte green = (byte)tbGreen.Value;
            byte blue = (byte)tbBlue.Value;

            ignoreEvent = true;
            nudRed.Value = red;
            nudGreen.Value = green;
            nudBlue.Value = blue;
            ignoreEvent = false;

            updateColourStuff(red, green, blue);
        }

        // Number change
        private void nud_ValueChanged(object sender, EventArgs e)
        {
            if (ignoreEvent)
                return;

            byte red = (byte)nudRed.Value;
            byte green = (byte)nudGreen.Value;
            byte blue = (byte)nudBlue.Value;

            ignoreEvent = true;
            tbRed.Value = red;
            tbGreen.Value = green;
            tbBlue.Value = blue;
            ignoreEvent = false;

            updateColourStuff(red, green, blue);
        }

        // Send new colour
        private void updateColourStuff(byte red, byte green, byte blue)
        {
            pbColour.BackColor = Color.FromArgb(red, green, blue);

            // Gamma correction
            if (cbGamma.Checked)
            {
                red = (byte)(Math.Pow(red / 255.0, GAMMA_CORRECTION) * 255);
                green = (byte)(Math.Pow(green / 255.0, GAMMA_CORRECTION) * 255);
                blue = (byte)(Math.Pow(blue / 255.0, GAMMA_CORRECTION) * 255);
            }

            Rgbledctrl.s_rgbVal colour = new Rgbledctrl.s_rgbVal();
            colour.red = red;
            colour.green = green;
            colour.blue = blue;

            if(Rgbledctrl.setRGB(ref colour))
                writeToLog(String.Format("Set RGB {0} {1} {2}", red, green, blue));
            else
                writeToLog("Device not available");
            writeToLog("------");
        }

        // Read EEPROM
        private void btnEEPROMRead_Click(object sender, EventArgs e)
        {
            if (!Rgbledctrl.poke(false))
            {
                writeToLog("Device not available");
                writeToLog("------");
                return;
            }

            Rgbledctrl.s_rgbled_device device = new Rgbledctrl.s_rgbled_device();
            Rgbledctrl.getDevice(ref device);

            byte value = new byte();
            lvEEPROM.Items.Clear();
            for (ushort i = 0; i < device.eepromSize; i++)
            {
                Rgbledctrl.eeprom_read(ref value, i);
                lvEEPROM.Items.Add(new ListViewItem(new string[] { i.ToString(), value.ToString() }));
            }
        }

        // Write to EEPROM
        private void btnEEPROMWrite_Click(object sender, EventArgs e)
        {
            ushort location = (ushort)nudEEPROMLocation.Value;
            byte value = (byte)nudEEPROMValue.Value;

            if(Rgbledctrl.eeprom_write(value, location))
                writeToLog(String.Format("Wrote {0} to location {1}", value, location));
            else
                writeToLog("Device not available");
            writeToLog("------");
        }

        private void tmrPoke_Tick(object sender, EventArgs e)
        {
            if (Rgbledctrl.poke(false))
                pbUsbOk.BackColor = Color.FromArgb(0, 255, 0);
            else
                pbUsbOk.BackColor = Color.FromArgb(255, 0, 0);
        }

        private void screenCap()
        {
            Bitmap bmpScreenshot = new Bitmap(
                Screen.PrimaryScreen.Bounds.Width,
                Screen.PrimaryScreen.Bounds.Height,
                PixelFormat.Format32bppRgb
                );

            Graphics gfxScreenshot = Graphics.FromImage(bmpScreenshot);
            gfxScreenshot.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;

            try
            {
                gfxScreenshot.CopyFromScreen(
                    Screen.PrimaryScreen.Bounds.X,
                    Screen.PrimaryScreen.Bounds.Y,
                    0,
                    0,
                    Screen.PrimaryScreen.Bounds.Size,
                    CopyPixelOperation.SourceCopy
                    );
            }
            catch (Exception ex)
            {
                BeginInvoke(new MethodInvoker(() =>
                {
                    writeToLog("Error getting screen image: " + ex.Message);
                    writeToLog("------");
                }));
            }
            
            uint avgRed = 0;
            uint avgGreen = 0;
            uint avgBlue = 0;

            BitmapData data = bmpScreenshot.LockBits(
                new Rectangle(Point.Empty, bmpScreenshot.Size),
                ImageLockMode.ReadOnly,
                PixelFormat.Format32bppRgb
                );

            int pixelCount = Screen.PrimaryScreen.Bounds.Height * Screen.PrimaryScreen.Bounds.Width;

            unsafe
            {
                int* ptr = (int*)data.Scan0;

                for (int i = 0; i < pixelCount; i++)
                {
                    int pixel = *ptr++;

                    avgRed += (byte)(pixel >> 16);
                    avgGreen += (byte)(pixel >> 8);
                    avgBlue += (byte)pixel;
                }
            }

            bmpScreenshot.UnlockBits(data);

            gfxScreenshot.Dispose();
            bmpScreenshot.Dispose();

            byte red = (byte)(avgRed / pixelCount);
            byte green = (byte)(avgGreen / pixelCount);
            byte blue = (byte)(avgBlue / pixelCount);

            // 
            BeginInvoke(new MethodInvoker(() =>
            {
                ignoreEvent = true;
                tbRed.Value = red;
                tbGreen.Value = green;
                tbBlue.Value = blue;
                nudRed.Value = red;
                nudGreen.Value = green;
                nudBlue.Value = blue;
                ignoreEvent = false;
                updateColourStuff(red, green, blue);
            }));
        }

        private void tCapScreen()
        {
            int interval = 50;
            while (true)
            {
                Thread.Sleep(interval);

                //
                Stopwatch stopWatch = new Stopwatch();
                stopWatch.Start();

                if (cbCapScreen.Checked)
                    screenCap();

                stopWatch.Stop();

                // 
                TimeSpan ts = stopWatch.Elapsed;
                if (ts.Milliseconds < 50)
                    interval = 50 - ts.Milliseconds;
                else
                    interval = 0;
            }
        }
    }
}
