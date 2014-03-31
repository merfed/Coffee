using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;

namespace M2Converter
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
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.SaveFileDialog FSaveDLG;
		private System.Windows.Forms.OpenFileDialog FLoadDLG;

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
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.FSaveDLG = new System.Windows.Forms.SaveFileDialog();
			this.FLoadDLG = new System.Windows.Forms.OpenFileDialog();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button1.Location = new System.Drawing.Point(8, 8);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(64, 24);
			this.button1.TabIndex = 0;
			this.button1.Text = "Load";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// button2
			// 
			this.button2.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button2.Location = new System.Drawing.Point(80, 8);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(64, 24);
			this.button2.TabIndex = 1;
			this.button2.Text = "Save";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// FSaveDLG
			// 
			this.FSaveDLG.DefaultExt = "x";
			this.FSaveDLG.Filter = "X Files|*.x|All Files|*.*";
			this.FSaveDLG.RestoreDirectory = true;
			this.FSaveDLG.Title = "Save an x file";
			this.FSaveDLG.FileOk += new System.ComponentModel.CancelEventHandler(this.FSaveDLG_FileOk);
			// 
			// FLoadDLG
			// 
			this.FLoadDLG.Filter = "M2 Files|*.m2|All Files|*.*";
			this.FLoadDLG.RestoreDirectory = true;
			this.FLoadDLG.Title = "Open an M2";
			// 
			// WinForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(152, 38);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "WinForm";
			this.Text = "M2 Converter";
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
		
		private void button1_Click(object sender, System.EventArgs e)
		{
			FLoadDLG.ShowDialog ();
		}
		
		private void button2_Click(object sender, System.EventArgs e)
		{
			FSaveDLG.ShowDialog ();
		}
		
		private void FSaveDLG_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
		{
			FileStream FLoad = new FileStream(FLoadDLG.FileName, FileMode.Open );
			FileStream FSave = new FileStream(FSaveDLG.FileName, FileMode.CreateNew );
			BinaryReader BinRead = new BinaryReader(FLoad);
			StreamWriter StrWrit = new StreamWriter (FSave);
			uint nVertex;
			uint nLODVert;
			uint nFaces;
			float[] VertexX ;
			float[] VertexY ;
			float[] VertexZ ;
			float[] UVU ;
			float[] UVV ;
			uint ofsVertex;
			uint ofsLODVert;
			uint ofsLODVX;
			uint ofsFaces;
			UInt16[] LOD;
			UInt16[] Faces;
			int loopcount = 0;
			//Start Reading the M2
			FLoad.Seek ( 0x044, SeekOrigin.Begin );
			nVertex = BinRead.ReadUInt32();
			ofsVertex = BinRead.ReadUInt32();
			FLoad.Seek (0x050, SeekOrigin.Begin);
			ofsLODVert = BinRead.ReadUInt32();
			FLoad.Seek (ofsVertex, SeekOrigin.Begin);
			VertexX = new float[nVertex ];
			VertexY = new float[nVertex ];
			VertexZ = new float[nVertex ];
			UVU = new float[nVertex];
			UVV = new float[nVertex];
			do
			{
				if (loopcount == nVertex /*- 1*/)
				{
				 break;
				 }
				 else
				 VertexX[loopcount] = BinRead.ReadSingle();
				 VertexY[loopcount] = BinRead.ReadSingle ();
				 VertexZ[loopcount] =BinRead.ReadSingle ();
				 BinRead.ReadBytes(8);
				 BinRead.ReadSingle();
				 BinRead.ReadSingle();
				 BinRead.ReadSingle();
				 UVU[loopcount] = BinRead.ReadSingle();
				 UVV[loopcount] = BinRead.ReadSingle();
				 BinRead.ReadSingle();
				 BinRead.ReadSingle();
				 loopcount = loopcount +1;


			}
				while (loopcount <= nVertex);
			FLoad.Seek (ofsLODVert, SeekOrigin.Begin);
			nLODVert = BinRead.ReadUInt32();
			LOD = new UInt16[nLODVert];
			ofsLODVX = BinRead.ReadUInt32();
			FLoad.Seek (ofsLODVX, SeekOrigin.Begin);
			loopcount = 0;
			do
			{
				if (loopcount == nLODVert)
				{
				break;
				}
				else
				LOD[loopcount] = BinRead.ReadUInt16();
				loopcount = loopcount + 1;
			}
				while (loopcount <= nLODVert);
			//Start readin Face Data
			FLoad.Seek (ofsLODVert, SeekOrigin.Begin);
			FLoad.Seek  (0x08,SeekOrigin.Current);
			nFaces = BinRead.ReadUInt32();
			Faces = new UInt16[nFaces];
			ofsFaces = BinRead.ReadUInt32();
			FLoad.Seek (ofsFaces,SeekOrigin.Begin);
			loopcount = 0;
			do
			{
				if (loopcount == nFaces)
				{
				break;
				}
				else
				Faces[loopcount] = BinRead.ReadUInt16();
				loopcount = loopcount + 1;

			}
				while (loopcount <= nFaces);
			//Finished reading everything ^_^
			//Start writing to the x file
			StrWrit.Write  ("xof 0302txt 0032");
			StrWrit.WriteLine();
			StrWrit.Write ("Mesh MtwoConv {");
			StrWrit.WriteLine ();
			StrWrit.Write (nLODVert);
			StrWrit.Write (";");
			StrWrit.WriteLine ();
			loopcount = 0;
			do
			{
				if (loopcount == nVertex - 1)
				{
				break;
				}
				else
				StrWrit.Write (VertexX[LOD[loopcount]]);
				StrWrit.Write (";");
				StrWrit.Write (VertexZ[LOD[loopcount]] * (-1));
				StrWrit.Write (";");
				StrWrit.Write (VertexY[LOD[loopcount]]);
				StrWrit.Write (";,");
				StrWrit.WriteLine ();
				loopcount = loopcount + 1;

			}
			   while (loopcount <=nVertex);
			StrWrit.Write (VertexX[LOD[loopcount]]);
			StrWrit.Write (";");
			StrWrit.Write (VertexZ[LOD[loopcount]] * (-1));
			StrWrit.Write (";");
			StrWrit.Write (VertexY[LOD[loopcount]]);
			StrWrit.Write (";;");
			StrWrit.WriteLine ();
			StrWrit.Write (nFaces / 3);
			StrWrit.Write (";");
			StrWrit.WriteLine ();
			loopcount = 0;
			//Start to write the face data
			do
			{
				if (loopcount == nFaces  - 3)
				{
					break;
				}
				else
				StrWrit.Write ("3;");
				StrWrit.Write (Faces[loopcount]);
				StrWrit.Write  (",");
				StrWrit.Write (Faces[loopcount+1]);
				StrWrit.Write (",");
				StrWrit.Write (Faces[loopcount+2]);
				StrWrit.Write (";");
				StrWrit.WriteLine ();
				loopcount = loopcount + 3;
			}
				while (loopcount <= nFaces );
            StrWrit.Write ("3;");
			StrWrit.Write (Faces[loopcount]);
			StrWrit.Write  (",");
			StrWrit.Write (Faces[loopcount+1]);
			StrWrit.Write (",");
			StrWrit.Write (Faces[loopcount+2]);
			StrWrit.Write (";;");
			StrWrit.WriteLine ();
			//Start writing the UV.
			StrWrit.Write ("MeshTextureCoords {");
			StrWrit.Write (nLODVert);
			StrWrit.Write (";");
			loopcount = 0;
			do
			{
				 if (loopcount == nLODVert - 1)
				 {
				 break;
				 }
				 else
				 StrWrit.Write (UVU[loopcount]);
				 StrWrit.Write (";");
				 StrWrit.Write (UVV[loopcount]);
				 StrWrit.Write (";");
				 StrWrit.WriteLine ();
				 loopcount = loopcount + 1;
			}
				while (loopcount < nLODVert);
			StrWrit.Write (UVU[loopcount]);
			StrWrit.Write (";");
			StrWrit.Write (UVV[loopcount]);
			StrWrit.Write (";;");
			StrWrit.WriteLine ();
			StrWrit.Write ("}");
			StrWrit.WriteLine ();
			StrWrit.Write ("}");
			StrWrit.Close ();
			}
	}
}
