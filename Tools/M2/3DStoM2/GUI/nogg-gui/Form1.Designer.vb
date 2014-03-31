<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.PictureBox1 = New System.Windows.Forms.PictureBox
        Me.Button1 = New System.Windows.Forms.Button
        Me.Convert = New System.Diagnostics.Process
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.TxtSrc = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.TxtDest = New System.Windows.Forms.Label
        Me.TxtScale = New System.Windows.Forms.Label
        Me.TextBox2 = New System.Windows.Forms.TextBox
        Me.TextBox3 = New System.Windows.Forms.TextBox
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = CType(resources.GetObject("PictureBox1.Image"), System.Drawing.Image)
        Me.PictureBox1.Location = New System.Drawing.Point(755, 9)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(18, 11)
        Me.PictureBox1.TabIndex = 0
        Me.PictureBox1.TabStop = False
        '
        'Button1
        '
        Me.Button1.Font = New System.Drawing.Font("Microsoft Sans Serif", 15.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Button1.ForeColor = System.Drawing.Color.Red
        Me.Button1.Location = New System.Drawing.Point(354, 292)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(244, 60)
        Me.Button1.TabIndex = 1
        Me.Button1.Text = "Convert Now!!!"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Convert
        '
        Me.Convert.StartInfo.CreateNoWindow = True
        Me.Convert.StartInfo.Domain = ""
        Me.Convert.StartInfo.FileName = "3dstom2.exe"
        Me.Convert.StartInfo.LoadUserProfile = False
        Me.Convert.StartInfo.Password = Nothing
        Me.Convert.StartInfo.StandardErrorEncoding = Nothing
        Me.Convert.StartInfo.StandardOutputEncoding = Nothing
        Me.Convert.StartInfo.UserName = ""
        Me.Convert.SynchronizingObject = Me
        '
        'Timer1
        '
        Me.Timer1.Interval = 200
        '
        'TxtSrc
        '
        Me.TxtSrc.BackColor = System.Drawing.Color.SkyBlue
        Me.TxtSrc.Location = New System.Drawing.Point(358, 67)
        Me.TxtSrc.Name = "TxtSrc"
        Me.TxtSrc.Size = New System.Drawing.Size(378, 20)
        Me.TxtSrc.TabIndex = 2
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.BackColor = System.Drawing.Color.Transparent
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.Color.White
        Me.Label1.Location = New System.Drawing.Point(172, 62)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(180, 25)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "Input file (*.3ds)"
        '
        'TxtDest
        '
        Me.TxtDest.AutoSize = True
        Me.TxtDest.BackColor = System.Drawing.Color.Transparent
        Me.TxtDest.Font = New System.Drawing.Font("Microsoft Sans Serif", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtDest.ForeColor = System.Drawing.Color.White
        Me.TxtDest.Location = New System.Drawing.Point(152, 122)
        Me.TxtDest.Name = "TxtDest"
        Me.TxtDest.Size = New System.Drawing.Size(191, 25)
        Me.TxtDest.TabIndex = 4
        Me.TxtDest.Text = "Output file (*.m2)"
        '
        'TxtScale
        '
        Me.TxtScale.AutoSize = True
        Me.TxtScale.BackColor = System.Drawing.Color.Transparent
        Me.TxtScale.Font = New System.Drawing.Font("Microsoft Sans Serif", 15.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtScale.ForeColor = System.Drawing.Color.White
        Me.TxtScale.Location = New System.Drawing.Point(132, 182)
        Me.TxtScale.Name = "TxtScale"
        Me.TxtScale.Size = New System.Drawing.Size(209, 25)
        Me.TxtScale.TabIndex = 5
        Me.TxtScale.Text = "Scale (Default = 1)"
        '
        'TextBox2
        '
        Me.TextBox2.BackColor = System.Drawing.Color.SkyBlue
        Me.TextBox2.Location = New System.Drawing.Point(358, 127)
        Me.TextBox2.Name = "TextBox2"
        Me.TextBox2.Size = New System.Drawing.Size(378, 20)
        Me.TextBox2.TabIndex = 6
        '
        'TextBox3
        '
        Me.TextBox3.BackColor = System.Drawing.Color.SkyBlue
        Me.TextBox3.Location = New System.Drawing.Point(358, 187)
        Me.TextBox3.Name = "TextBox3"
        Me.TextBox3.Size = New System.Drawing.Size(40, 20)
        Me.TextBox3.TabIndex = 7
        Me.TextBox3.Text = "1"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackgroundImage = CType(resources.GetObject("$this.BackgroundImage"), System.Drawing.Image)
        Me.ClientSize = New System.Drawing.Size(952, 432)
        Me.Controls.Add(Me.TextBox3)
        Me.Controls.Add(Me.TextBox2)
        Me.Controls.Add(Me.TxtScale)
        Me.Controls.Add(Me.TxtDest)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.TxtSrc)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.PictureBox1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "Form1"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Form1"
        Me.TransparencyKey = System.Drawing.Color.Fuchsia
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents Convert As System.Diagnostics.Process
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents TxtScale As System.Windows.Forms.Label
    Friend WithEvents TxtDest As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents TxtSrc As System.Windows.Forms.TextBox
    Friend WithEvents TextBox2 As System.Windows.Forms.TextBox
    Friend WithEvents TextBox3 As System.Windows.Forms.TextBox

End Class
