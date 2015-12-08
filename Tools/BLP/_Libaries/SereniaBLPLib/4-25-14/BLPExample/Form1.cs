using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;

using System.IO;
using SereniaBLPLib;

namespace BLPExample
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Bitmap bmp;

        private void OpenClick(object sender, EventArgs e)
        {
            try
            {
                var dlg = openFileDialog.ShowDialog();
                if (dlg != DialogResult.OK)
                    return;

                var file = openFileDialog.OpenFile();

                using (var blp = new BlpFile(file))
                {
                    bmp = blp.GetBitmap(0);
                }

                var graphics = panel1.CreateGraphics();
                graphics.Clear(panel1.BackColor);
                graphics.DrawImage(bmp, 0, 0);

                button3.Enabled = true;                
            }
            catch (FileNotFoundException fe)
            {
                MessageBox.Show("The 'example.blp' was not found!");
            }
        }

        private void SaveAsClick(object sender, EventArgs e)
        {
            var dlg = saveFileDialog.ShowDialog();
            if (dlg != DialogResult.OK)
                return;

            var format = ExtensionToImageFormat(saveFileDialog.FileName);
            bmp.Save(saveFileDialog.FileName, format);
        }

        private static ImageFormat ExtensionToImageFormat(string fileName)
        {
            switch (Path.GetExtension(fileName))
            {
                case "jpg":
                case "jpeg":
                    return ImageFormat.Jpeg;
                case "png":
                    return ImageFormat.Png;
                case "bmp":
                    return ImageFormat.Bmp;
            }
            return ImageFormat.Jpeg;
        }

        private void PanelPaint(object sender, PaintEventArgs e)
        {
            if (bmp != null) e.Graphics.DrawImage(bmp, 0, 0);
        }
    }
}
