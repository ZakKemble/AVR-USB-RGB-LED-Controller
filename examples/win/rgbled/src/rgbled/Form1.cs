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
		private const int SCREENCAP_INTERVAL = 50;
        private bool ignoreEvent = false;
        private Rgbledctrl rgbLed;

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
            // Must be called before any other stuff!
            Rgbledctrl.init();

            // Create RGB LED object
            rgbLed = new Rgbledctrl();

			// Start poke timer
            tmrPoke.Enabled = true;

            // Thread for getting average screen colour
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
            if (cmbDevices.SelectedItem == null || cmbDevices.SelectedItem.ToString() == "")
                writeToLog("No device selected");
            else
            {
                uint device;
                uint.TryParse(cmbDevices.SelectedItem.ToString(), out device);

                writeToLog(String.Format("Opening device #{0}", device));

                Rgbledctrl newRgbLed = new Rgbledctrl();

                if (newRgbLed.open(device)) // Open
                {
                    // See if the new handle and old handle are handles to the same device
                    if (!Rgbledctrl.sameDevice(rgbLed.handle, newRgbLed.handle))
                    {
                        writeToLog("Device opened");
                        rgbLed.close(); // Close old handle
                        rgbLed = newRgbLed;
                    }
                    else
                    {
                        writeToLog("Device is already open");
                        newRgbLed.close(); // Close handle
                    }
                }
                else // Failed to open
                    writeToLog("Failed to open device");
            }

            writeToLog("------");
        }

        // Get info
        private void btnInfo_Click(object sender, EventArgs e)
        {
            // See if device is available
            if (!rgbLed.poke())
            {
                writeToLog("Device not available");
                writeToLog("------");
                return;
            }

            // Print info
            writeToLog(String.Format("Firmware version: {0}.{1}", rgbLed.version[0], rgbLed.version[1]));
            writeToLog(String.Format("User EEPROM size: {0}", rgbLed.eepromSize));
            writeToLog(String.Format("Red: {0}", rgbLed.colour.R));
            writeToLog(String.Format("Green: {0}", rgbLed.colour.G));
            writeToLog(String.Format("Blue: {0}", rgbLed.colour.B));
            writeToLog(String.Format("Idle time: {0}", rgbLed.idleTime));
            writeToLog("------");
        }

        // Close device
        private void btnClose_Click(object sender, EventArgs e)
        {
            rgbLed.close();
            writeToLog("Device closed");
            writeToLog("------");
        }

        // Reset device
        private void btnReset_Click(object sender, EventArgs e)
        {
            writeToLog("Device reset");
            writeToLog("------");
        }

        // Set idle time
        private void btnIdleTime_Click(object sender, EventArgs e)
        {
            if(rgbLed.setIdleTime((byte)nudIdleTime.Value))
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

            // Get trackbar values
            byte red = (byte)tbRed.Value;
            byte green = (byte)tbGreen.Value;
            byte blue = (byte)tbBlue.Value;

            // Set numeric box values
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

            // Get numeric box values
            byte red = (byte)nudRed.Value;
            byte green = (byte)nudGreen.Value;
            byte blue = (byte)nudBlue.Value;

            // Set trackbar values
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

            Color colour = Color.FromArgb(red, green, blue);

            if(rgbLed.setRGB(colour))
                writeToLog(String.Format("Set RGB {0} {1} {2}", red, green, blue));
            else
                writeToLog("Device not available");
            writeToLog("------");
        }

        // Read EEPROM
        private void btnEEPROMRead_Click(object sender, EventArgs e)
        {
            // See if device is available
            if (!rgbLed.poke())
            {
                writeToLog("Device not available");
                writeToLog("------");
                return;
            }

            byte value = new byte();
            lvEEPROM.Items.Clear();
			
			// Read all EEPROM locations
            for (ushort i = 0; i < rgbLed.eepromSize; i++)
            {
                rgbLed.eeprom_read(ref value, i);
                lvEEPROM.Items.Add(new ListViewItem(new string[] { i.ToString(), value.ToString() }));
            }
        }

        // Write to EEPROM
        private void btnEEPROMWrite_Click(object sender, EventArgs e)
        {
            ushort location = (ushort)nudEEPROMLocation.Value;
            byte value = (byte)nudEEPROMValue.Value;

            if (rgbLed.eeprom_write(value, location))
                writeToLog(String.Format("Wrote {0} to location {1}", value, location));
            else
                writeToLog("Device not available");
            writeToLog("------");
        }

        // ComboBox drop down opened
        private void cmbDevices_DropDown(object sender, EventArgs e)
        {
			// Find devices
            uint count = Rgbledctrl.find();

            writeToLog(String.Format("Devices found: {0}", count));
            writeToLog("------");

            cmbDevices.Items.Clear();

			// Show devices
            for (uint i = 0; i < count; i++)
                cmbDevices.Items.Add(i);
        }

        // Poke timer
        private void tmrPoke_Tick(object sender, EventArgs e)
        {
            if (rgbLed.poke())
                pbUsbOk.BackColor = Color.FromArgb(0, 255, 0);
            else
                pbUsbOk.BackColor = Color.FromArgb(255, 0, 0);
        }

        // Get average colour of screen
        private void screenCap()
        {
            // Create bitmap
            Bitmap bmpScreenshot = new Bitmap(
                Screen.PrimaryScreen.Bounds.Width,
                Screen.PrimaryScreen.Bounds.Height,
                PixelFormat.Format32bppRgb
                );

            Graphics gfxScreenshot = Graphics.FromImage(bmpScreenshot);
            gfxScreenshot.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
            gfxScreenshot.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Low;
            gfxScreenshot.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;

            // Get screenshot
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
                // Oops, something went wrong
                BeginInvoke(new MethodInvoker(() =>
                {
                    writeToLog("Error getting screen image: " + ex.Message);
                    writeToLog("------");
                }));
                gfxScreenshot.Dispose();
                bmpScreenshot.Dispose();
                return;
            }
            
            // Get average colour
            uint avgRed = 0;
            uint avgGreen = 0;
            uint avgBlue = 0;

            BitmapData data = bmpScreenshot.LockBits(
                new Rectangle(Point.Empty, bmpScreenshot.Size),
                ImageLockMode.ReadOnly,
                PixelFormat.Format32bppRgb
                );

            int pixelCount = Screen.PrimaryScreen.Bounds.Height * Screen.PrimaryScreen.Bounds.Width;

            //Stopwatch stopWatch = new Stopwatch();
            //stopWatch.Start();

            // Unsafe code as its much faster than safe code (getPixel() is super slow)
            unsafe
            {
                byte* ptr = (byte*)data.Scan0;
                for (int i = 0; i < pixelCount; i++)
                {
                    avgRed += *ptr++;
                    avgGreen += *ptr++;
                    avgBlue += *ptr++; // 2 for alpha
                    ptr++;
                }
            }

            //stopWatch.Stop();
            //TimeSpan ts = stopWatch.Elapsed;

            bmpScreenshot.UnlockBits(data);

            // Dispose now. These images use up a lot of memory
            gfxScreenshot.Dispose();
            bmpScreenshot.Dispose();

            byte red = (byte)(avgRed / pixelCount);
            byte green = (byte)(avgGreen / pixelCount);
            byte blue = (byte)(avgBlue / pixelCount);

            // Set colour
            BeginInvoke(new MethodInvoker(() =>
            {
                //writeToLog(String.Format(ts.ToString()));
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

        // Screen capture thread
        private void tCapScreen()
        {
            int interval = SCREENCAP_INTERVAL;
            while (true)
            {
                Thread.Sleep(interval);

                // Start timing
                Stopwatch stopWatch = new Stopwatch();
                stopWatch.Start();

                if (cbCapScreen.Checked)
                    screenCap();

                stopWatch.Stop();

                // WOrkout how long to wait for next screen cap
                TimeSpan ts = stopWatch.Elapsed;
                if (ts.Milliseconds < SCREENCAP_INTERVAL)
                    interval = SCREENCAP_INTERVAL - ts.Milliseconds;
                else
                    interval = 0;
            }
        }
    }
}
