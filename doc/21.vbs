strPath = Wscript.ScriptFullName

Set objFSO  = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.GetFile(strPath)

cPathTmp = objFSO.GetParentFolderName(objFile)
If Right(cPathTmp, 1) = "\" then
	cPath = objFSO.GetParentFolderName(objFile)
Else
	cPath = objFSO.GetParentFolderName(objFile) & "\"
End If

cCfgFilePath = cPath&"11.ini"
strValue = GetProFile(cCfgFilePath, "HOST_CONFIG", "NoRtpTimeout", "20")
MsgBox strValue

Function GetProFile(strFileName, strSectionName, strKeyName, strDefault)
	Set objFSO = CreateObject("Scripting.FileSystemObject")
	
	If Not objFSO.fileExists(strFileName) then
		Exit Function
	End If	
	
	Set eXmlFile = objFSO.OpenTextFile(strFileName, 1)
	Do While not eXmlFile.AtEndOfStream
		eStrLine = eXmlFile.Readline
		If Trim(eStrLine) <> "" Then
			If Left(Trim(eStrLine), 1) = "[" Then
				If Trim(Mid(Trim(eStrLine), 2, Len(Trim(eStrLine))-2)) <> strSectionName Then
					st = False
				Else
					st = True
				End If
			End If
			
			If st = True Then
				eStrKey = split(Trim(eStrLine), "=")
				If Trim(eStrKey(0)) = strKeyName Then
				
					If Trim(eStrKey(1)) = "" Then
						If strDefault <> "" Then
							GetProFile = Trim(strDefault)
						End If
					Else
						GetProFile = Trim(eStrKey(1))
					End If
					
					eXmlFile.Close
					Set eXmlFile = Nothing   
  					Exit Function
				End If
			End If
		End If
	Loop
	
	eXmlFile.Close   
    Set eXmlFile = Nothing
    wscript.quit   
    Exit Function
End Function

Function SetProFile(strFileName, strSectionName, strKeyName, strString)
	Set objFSO = CreateObject("Scripting.FileSystemObject")
	
	If Not objFSO.fileExists(strFileName) then
		Exit Function
	End If	
	
	Set eXmlFile = objFSO.OpenTextFile(strFileName, 1)
	Set eWriteFile = objFSO.OpenTextFile(".\bak.ini", 2, True)
	
	Do While not eXmlFile.AtEndOfStream
		eStrLine = eXmlFile.Readline
		If Trim(eStrLine) <> "" Then
			If Left(Trim(eStrLine), 1) = "[" Then
				If Trim(Mid(Trim(eStrLine), 2, Len(Trim(eStrLine))-2)) <> strSectionName Then
					st = False
				Else
					st = True
				End If
			End If
			
			If st = True Then
				eStrKey = split(Trim(eStrLine), "=")
				If Trim(eStrKey(0)) = strKeyName Then
					
					If strString <> "" Then
						'eStrLine = strKeyName + "=" + strString;
						'eWriteFile.WriteLine(eStrLine)	
					Else
						eWriteFile.WriteLine(eStrLine)	
					End If
				Else
					eWriteFile.WriteLine(eStrLine)	
				End If
			Else
				eWriteFile.WriteLine(eStrLine)
			End If
			
		End If	
	Loop
	
	'objFSO.GetFile(strFileName).Delete
	
	eXmlFile.Close
	eWriteFile.Close
	
    Set eXmlFile = Nothing
	Set eWriteFile = Nothing
	
    wscript.quit   
    Exit Function
End Function