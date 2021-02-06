Attribute VB_Name = "NewMacros"
Type x
   b As Byte
   g As Byte
   r As Byte
End Type

Private Declare Function GetTickCount Lib "Kernel32.dll" () As Long

Sub myversion()
Attribute myversion.VB_Description = "Macro created 12/21/2005 by ams3"
Attribute myversion.VB_ProcData.VB_Invoke_Func = "Normal.NewMacros.myversion"
   Dim w As Long, h As Long
   Dim c As Byte
   Dim f As Integer
   Dim fl As Long
   Dim ci() As x
   Dim t1 As Long
   
   
   f = FreeFile
   
   Open "c:\112.bmp" For Binary As f
   
   fl = LOF(f)
   
   Get f, &H13, w
   Get f, &H17, h
   Get f, &H1D, c
   
  ' MsgBox w & "x" & h & "x" & c
   
   
   ReDim ci(w - 1)
   
   ActiveDocument.PageSetup.PageHeight = InchesToPoints(22)
   ActiveDocument.PageSetup.PageWidth = InchesToPoints(22)
   Selection.Font.Bold = True
   Selection.Font.Size = 10
   Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter
   Selection.ParagraphFormat.LineSpacingRule = wdLineSpaceExactly
   Selection.ParagraphFormat.LineSpacing = 6
   
   
   t1 = GetTickCount
   
   While h > 0
      DoEvents
      Get f, fl - (w * 3) + 1, ci
       
      For gx = 1 To w
         If RGB(ci(gx - 1).r, ci(gx - 1).g, ci(gx - 1).b) < &HFFFFFF Then
            Selection.Font.Color = RGB(Not ci(gx - 1).r, Not ci(gx - 1).g, Not ci(gx - 1).b)
            Selection.TypeText Text:="x"
         End If
      Next
      
      Selection.TypeText Text:=vbCrLf
      
      fl = fl - (w * 3)
   
   
      h = h - 1
   Wend
   
   MsgBox (GetTickCount - t1) / 1000
   
   
   Close f
   

End Sub


Sub aalineupV()
Attribute aalineupV.VB_Description = "Moves the insertion point up one line"
Attribute aalineupV.VB_ProcData.VB_Invoke_Func = "Normal.NewMacros.lineup"
    Dim s1 As Shape
    Dim s2 As Shape
    Dim slen As Long
    Dim apxCount As Long
    
    Set s1 = Selection.ShapeRange(1)
    
    slen = CentimetersToPoints(Val(InputBox("Hoy", "Title")))
    apxCount = Round(slen / s1.height, 0)
    
    Do While (apxCount > 0)
        DoEvents
        Selection.Copy
        Selection.Paste
        Set s2 = Selection.ShapeRange(1)
        s2.Left = s1.Left
        s2.Top = s1.Top + s1.height
        Set s1 = s2
        apxCount = apxCount - 1
    Loop

End Sub

Sub lineupH()
    Dim s1 As Shape
    Dim s2 As Shape
    Dim slen As Long
    Dim apxCount As Long
    
    Set s1 = Selection.ShapeRange(1)
    
    slen = CentimetersToPoints(Val(InputBox("Hoy", "Title")))
    apxCount = Round(slen / s1.width, 0)
    
    Do While (apxCount > 0)
        DoEvents
        Selection.Copy
        Selection.Paste
        Set s2 = Selection.ShapeRange(1)
        s2.Top = s1.Top
        s2.Left = s1.Left + s1.width
        Set s1 = s2
        apxCount = apxCount - 1
    Loop

End Sub

