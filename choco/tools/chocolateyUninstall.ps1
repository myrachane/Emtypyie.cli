$ErrorActionPreference = 'Stop'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Uninstall-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
