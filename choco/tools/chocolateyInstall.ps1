<#
.SYNOPSIS
  Chocolatey install script for emtypyie-cli.
.DESCRIPTION
  Downloads the native Windows binary ZIP from GitHub Releases,
  extracts it, and registers the emtypyie command via shim.
.NOTES
  The URL and checksum must match the C native build ZIP from
  the release workflow (emtypyie-cli-native-windows-x64-*.zip).
#>

$ErrorActionPreference = 'Stop'

$packageName = 'emtypyie-cli'
$url = 'https://github.com/myrachane/emtypyie-cli/releases/download/v2.5.1/emtypyie-cli-native-windows-x64-2.5.1.zip'
$checksum = '9548F80E95DAB147EF5734938F5CCC2ABD9762AD9B4C84A9A4E7CC3246906424'
$checksumType = 'sha256'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Install-ChocolateyZipPackage -PackageName "$packageName" -Url "$url" -UnzipLocation "$toolsDir" -Checksum "$checksum" -ChecksumType "$checksumType"

Install-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
