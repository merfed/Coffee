using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using System.Text;


	/// <summary>
	/// Modified by 
	///		Taylor Mouse
	///		11.08.2005
	///		
	/// </summary>

namespace wmoconverter
{
	/// <summary>
	/// Summary description for WinForm.
	/// </summary>
	public class WinForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.OpenFileDialog FLoadDLG;
		private System.Windows.Forms.TextBox txtFileName;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblStatus;
		private System.Windows.Forms.SaveFileDialog FSaveDLG;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;

		public WinForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose (bool disposing)
		{
			if (disposing)
			{
				if (components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(WinForm));
			this.btnLoad = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.FLoadDLG = new System.Windows.Forms.OpenFileDialog();
			this.FSaveDLG = new System.Windows.Forms.SaveFileDialog();
			this.txtFileName = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.lblStatus = new System.Windows.Forms.Label();
			this.button2 = new System.Windows.Forms.Button();
			this.button3 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btnLoad
			// 
			this.btnLoad.Cursor = System.Windows.Forms.Cursors.Hand;
			this.btnLoad.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btnLoad.Location = new System.Drawing.Point(608, 32);
			this.btnLoad.Name = "btnLoad";
			this.btnLoad.Size = new System.Drawing.Size(24, 24);
			this.btnLoad.TabIndex = 0;
			this.btnLoad.Text = "...";
			this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
			// 
			// button1
			// 
			this.button1.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button1.Location = new System.Drawing.Point(8, 64);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(72, 24);
			this.button1.TabIndex = 1;
			this.button1.Text = "Export";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// FLoadDLG
			// 
			this.FLoadDLG.Filter = "WMO Files|*.wmo|All Files|*.*";
			this.FLoadDLG.Title = "Load a WMO File";
			// 
			// FSaveDLG
			// 
			this.FSaveDLG.Filter = "X Files|*.x";
			this.FSaveDLG.FileOk += new System.ComponentModel.CancelEventHandler(this.FSaveDLG_FileOk);
			// 
			// txtFileName
			// 
			this.txtFileName.Location = new System.Drawing.Point(8, 32);
			this.txtFileName.Name = "txtFileName";
			this.txtFileName.Size = new System.Drawing.Size(592, 20);
			this.txtFileName.TabIndex = 2;
			this.txtFileName.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(288, 16);
			this.label1.TabIndex = 3;
			this.label1.Text = "Select a WMO file to convert to X file format.";
			// 
			// lblStatus
			// 
			this.lblStatus.Location = new System.Drawing.Point(280, 8);
			this.lblStatus.Name = "lblStatus";
			this.lblStatus.Size = new System.Drawing.Size(440, 16);
			this.lblStatus.TabIndex = 5;
			this.lblStatus.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// button2
			// 
			this.button2.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button2.Location = new System.Drawing.Point(88, 64);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(72, 24);
			this.button2.TabIndex = 6;
			this.button2.Text = "About";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button3
			// 
			this.button3.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button3.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button3.Location = new System.Drawing.Point(168, 64);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(72, 24);
			this.button3.TabIndex = 7;
			this.button3.Text = "Exit";
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// WinForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(642, 96);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.lblStatus);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.txtFileName);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.btnLoad);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "WinForm";
			this.Text = "WMO Converter";
			this.ResumeLayout(false);
		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new WinForm());
		}
		
		private void FSaveDLG_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
		{
			FileStream FLoad = new FileStream (FLoadDLG.FileName,FileMode.Open);
			FileStream FSave = new FileStream (FSaveDLG.FileName,FileMode.Create);
			BinaryReader BinRead = new BinaryReader (FLoad);
			StreamWriter StrWrit = new StreamWriter (FSave);
			//Declare most of used variables
			uint nUVS;
			uint nVertex;
			uint CHNKLNGHT;
			float[] UVData;
			char[] fourcc = new char[5];
			string[] CHNKID = new String[3];
			uint nFace;
			Int16[] FaceData ;
			FLoad.Seek (0x58,0);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			nVertex = CHNKLNGHT / 12;
			float[] VertexData = new float[nVertex *3 + 1];
			StrWrit.Write ("xof 0302txt 0032");
			StrWrit.WriteLine();
			StrWrit.Write ("Mesh WMOCNV{");
			StrWrit.WriteLine ();
			StrWrit.Write (nVertex.ToString() + ";");
			StrWrit.WriteLine();
			long Position;
			Position = FLoad.Position;
			int firstvar = 0;
			int secondvar = 1;
			int thirdvar = 0;
			int fourthvar ; //this is sick allready!!!
			//Start reading vertex data
			do
			{
					VertexData[firstvar] = BinRead.ReadSingle ();
					firstvar= firstvar+1;
									
			}
				while (firstvar < nVertex * 3);
			VertexData[firstvar] = BinRead.ReadSingle ();
			fourthvar = firstvar;
			firstvar = 0;
			do
			{
				if (secondvar == 1)
				{
					StrWrit.Write (VertexData[firstvar]);
					StrWrit.Write (";");
					secondvar = 2;
					firstvar = firstvar + 2;
					
				}
				else if (secondvar == 2)
				{
					StrWrit.Write (VertexData[firstvar]*(-1));
					StrWrit.Write (";");
					secondvar = 3;
					firstvar = firstvar - 1;
				}
				else if (secondvar == 3)
				{
					if (firstvar == nVertex * 3 - 2)
					{
						break;
					}
					else
					StrWrit.Write (VertexData[firstvar].ToString());
					StrWrit.Write (";,");
					StrWrit.WriteLine ();
					secondvar = 1;
					firstvar = firstvar + 2;
					thirdvar = thirdvar + 1;
				}
				else 
					break;
				
			}

				while (thirdvar *3 < fourthvar );
			StrWrit.Write (VertexData[firstvar]);
			StrWrit.Write (";;");
			StrWrit.WriteLine ();
			FLoad.Position = 0;
			FLoad.Seek (0x58,0);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			nFace = CHNKLNGHT / 6;
			StrWrit.Write (nFace);
			StrWrit.Write (";");
			StrWrit.WriteLine();
			firstvar = 0;
			FaceData = new Int16[nFace * 3 + 1];
			//Start reading the face data
            do 
			{
				FaceData[firstvar] = BinRead.ReadInt16();
				firstvar = firstvar + 1;
				if (firstvar == nFace * 3)
				{
					break;
				}

			}
				while (firstvar < nFace * 3);
			FaceData[firstvar]= BinRead.ReadInt16();
			firstvar = 0;
			secondvar = 1;
			//Start writing the face data
			do
			{
				if (secondvar == 1)
				{
					StrWrit.Write ("3;");
					StrWrit.Write (FaceData[firstvar]);
					StrWrit.Write (",");
					firstvar = firstvar + 1;
					secondvar = 2;
				}
				else if (secondvar == 2)
				{
					StrWrit.Write (FaceData[firstvar]);
					StrWrit.Write (",");
					firstvar = firstvar + 1;
					secondvar = 3;
				}
				else if (secondvar == 3)
				{
					if (firstvar == nFace * 3 - 1)
					{	
                        break;					
					}
					else
					StrWrit.Write (FaceData[firstvar]);
					StrWrit.Write (";");
					StrWrit.WriteLine ();
					secondvar = 1;
					firstvar = firstvar + 1;
				}
			}
				while(firstvar < nFace * 3 - 1);
			StrWrit.Write (FaceData[firstvar]);
			StrWrit.Write (";;");
			StrWrit.WriteLine();
			FLoad.Position = 0;
			FLoad.Seek (0x58,0);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			FLoad.Position = (FLoad.Position + CHNKLNGHT);
			BinRead.ReadChars (4);
			CHNKLNGHT = BinRead.ReadUInt32();
			firstvar = 0;
			nUVS = CHNKLNGHT / 8;
			UVData = new float[CHNKLNGHT/4];
			//Start reading the UV Data;
			do
			{
				/*if (firstvar == CHNKLNGHT / 4 -1)
				{
					break;
				}
				else
				*/
				UVData[firstvar] = BinRead.ReadSingle();
				firstvar = firstvar + 1;
				
			}	
				while (firstvar < CHNKLNGHT / 4);
			UVData[firstvar-1] = BinRead.ReadSingle() ;
			BinRead.Close ();
			StrWrit.Write ("MeshTextureCoords {");
			StrWrit.WriteLine() ;
			StrWrit.Write (nUVS);
			StrWrit.Write (";");
			StrWrit.WriteLine ();
			secondvar = 1;
			firstvar = 0;
			do 
				if (secondvar == 1)
				{
					StrWrit.Write (UVData[firstvar]);
					StrWrit.Write (";");
					firstvar=firstvar + 1;
					secondvar = 2;
				}
				else if (secondvar == 2)
				{
					if (firstvar == CHNKLNGHT / 4 -1)
					{
						break;
					}
					else
					StrWrit.Write (UVData[firstvar]);
					StrWrit.Write (";");
					StrWrit.WriteLine();
					firstvar = firstvar + 1;
					secondvar = 1;
			}
				while (firstvar < CHNKLNGHT /4);
			StrWrit.Write (UVData[firstvar]);
			StrWrit.Write (";;");
			StrWrit.WriteLine();
			StrWrit.Write ("}");
			StrWrit.WriteLine();
			StrWrit.Write ("}");
			StrWrit.Close();
		}
		
		private void btnLoad_Click(object sender, System.EventArgs e)
		{
			FLoadDLG.ShowDialog();
			txtFileName.Text = FLoadDLG.FileName;
		}
		
		private void button1_Click(object sender, System.EventArgs e)
		{
			// Manipulate so u don't need  to type the name of the x file.
			string tmp;

			tmp = txtFileName.Text;
			
			FSaveDLG.FileName = tmp;
			
			
			FSaveDLG.ShowDialog();
		}
		
		private void button2_Click(object sender, System.EventArgs e)
		{
			MessageBox.Show ("Convertor code by mapper, interface and extras by Taylor_Mouse, go to http://www.freewebs.com/wmotoolcsharp/index.htm.");
		}
		
		private void button3_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}
	}
}
