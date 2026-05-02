#
#  scripts/set-up-env-vars.ps1
#  Resolves nmake.exe and updates environment variables.
#  Copyright (c) 2026 Carlos Camacho. All rights reserved.
#
#  Usage:
#    set-up-env-vars.ps1
#

# Resolve nmake.exe: honour NMAKE_PATH if already set, otherwise use vswhere.
if (-not $env:NMAKE_PATH) {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) {
        Write-Error "vswhere.exe not found at '$vswhere'. Install Visual Studio 2022."
        exit 1
    }
    $env:NMAKE_PATH = & $vswhere -latest -find "VC/Tools/MSVC/*/bin/HostX64/x64/nmake.exe" |
        Select-Object -First 1

    # Persist for future sessions (CMake Tools reads this at startup).
    [System.Environment]::SetEnvironmentVariable("NMAKE_PATH", $env:NMAKE_PATH, "User")
    Write-Host "NMAKE_PATH persisted as user environment variable. Restart VS Code for CMake Tools to pick it up."
}

if (-not $env:NMAKE_PATH) {
    Write-Error "Could not locate nmake.exe. Set NMAKE_PATH or install Visual Studio 2022."
    exit 1
}

Write-Host "Using nmake: $env:NMAKE_PATH"

exit $LASTEXITCODE
