$xml = Get-Content "wuifet.vcxproj"
$xml.Project.PropertyGroup.WindowsPackageType = "None"
$xml.Save("wuifet.vcxproj")