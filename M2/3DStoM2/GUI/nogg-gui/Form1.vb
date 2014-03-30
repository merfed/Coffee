Public Class Form1

    Private Sub PictureBox1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PictureBox1.Click
        Close()
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Timer1.Enabled = False
        Button1.Text = "Convert Now!!!"
        Convert.StartInfo.Arguments = """" + TxtSrc.Text + """" + " " + """" + TxtDest.Text + """" + " " + TxtScale.Text
        Convert.Start()
        MsgBox("Model converted successfully")
    End Sub

    Private Sub Button1_MouseEnter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.MouseEnter
        Timer1.Enabled = True
    End Sub

    Private Sub Button1_MouseLeave(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.MouseLeave
        Timer1.Enabled = False
        Button1.Text = "Convert Now!!!"
    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        '[Declaration: must be static]
        Static NoText As Boolean
        Static Colour1 As Boolean
        '[/Declaration: must be static]

        '[If the boolean is true]
        If NoText = True Then
            '[/If the boolean is true]

            '[Erase the text]
            Button1.Text = ""
            '[/Erase the text]

            '[Make the boolean false]
            NoText = False
            '[/Make the boolean false]

            '[If the boolean is false]
        ElseIf NoText = False Then
            '[/If the boolean is false]

            '[Show the text in red]
            If Colour1 = True Then
                Button1.Text = "Convert Now!!!"
                Button1.ForeColor = Color.Red
                Colour1 = False
            ElseIf Colour1 = False Then
                Button1.Text = "Convert Now!!!"
                Button1.ForeColor = Color.Blue
                Colour1 = True
            End If
            '[/Show the text in red]

            '[Make the boolean true]
            NoText = True
            '[/Make the boolean true]

        End If
    End Sub

End Class
