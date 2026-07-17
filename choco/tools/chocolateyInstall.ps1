$ErrorActionPreference = 'Stop'

$packageName = 'emtypyie-cli'
$url = 'https://github.com/myrachane/emtypyie-cli/releases/download/v2.2.0/emtypyie-cli-windows-x64-2.2.0.zip'
$checksum = '7DA18D2FBA36415DE0385B5739D2570660C3C421871D8E42EEF2E3465F83C3CB'
$checksumType = 'sha256'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Install-ChocolateyZipPackage -PackageName "$packageName" -Url "$url" -UnzipLocation "$toolsDir" -Checksum "$checksum" -ChecksumType "$checksumType"

Install-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
