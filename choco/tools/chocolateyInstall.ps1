$ErrorActionPreference = 'Stop'

$packageName = 'emtypyie-cli'
$url = 'https://github.com/myrachane/emtypyie-cli/releases/download/v2.2.0/emtypyie-cli-native-windows-x64-2.2.0.zip'
$checksum = '4A6C448C90C0F32FFE2FD199E7B89105D090429B743EE0CCC8D717F6B95C9590'
$checksumType = 'sha256'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Install-ChocolateyZipPackage -PackageName "$packageName" -Url "$url" -UnzipLocation "$toolsDir" -Checksum "$checksum" -ChecksumType "$checksumType"

Install-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
