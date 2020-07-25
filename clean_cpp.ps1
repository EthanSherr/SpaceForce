Remove-Item .\DerivedDataCache -Recurse -ErrorAction Ignore
Remove-Item .\Intermediate -Recurse -ErrorAction Ignore
Remove-Item .\Binaries -Recurse -ErrorAction Ignore
Remove-Item .\*.sln -ErrorAction Ignore
Write-Host "Deleted sln and other intermediates"
#A:\UE4\UE_4.25\UnrealEngine\Engine\Build\BatchFiles\GenerateProjectFiles.bat A:\UE4\projects\SpaceForce\SpaceForce.uproject
 