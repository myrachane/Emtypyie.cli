<#
.SYNOPSIS
  Chocolatey uninstall script for emtypyie-cli.
.DESCRIPTION
  Removes the emtypyie shim (bin file) on uninstall.
  Note: does not clean up downloaded files from .emtypyie/.
#>

$ErrorActionPreference = 'Stop'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

Uninstall-BinFile -Name "emtypyie" -Path "$toolsDir\emtypyie.exe"
