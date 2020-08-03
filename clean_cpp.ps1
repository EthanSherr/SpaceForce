Remove-Item .\DerivedDataCache -Recurse -ErrorAction Ignore
Remove-Item .\Intermediate -Recurse -ErrorAction Ignore
Remove-Item .\Binaries -Recurse -ErrorAction Ignore
Remove-Item .\*.sln -ErrorAction Ignore
Write-Host "Deleted sln and other intermediates"
./GenerateProjectFiles.bat SpaceForce.uproject
 