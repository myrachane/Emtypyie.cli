$ErrorActionPreference = 'Stop'

$packageName = 'emtypyie-cli'
$url = 'https://github.com/myrachane/emtypyie-cli/releases/download/v2.1.3/emtypyie-cli-windows-x64-2.1.3.zip'
$checksum = 'F0CE6FEF2103A26DFD8EF62E056C0392DEEAEE52613908E7149F684BEE5EF4F0'
$checksumType = 'sha256'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Install-ChocolateyZipPackage -PackageName "$packageName" -Url "$url" -UnzipLocation "$toolsDir" -Checksum "$checksum" -ChecksumType "$checksumType"

Install-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
