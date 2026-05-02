# Generate 64-bit import libraries for libvlc / libvlccore from the
# system-installed VLC. The bundled .lib in vlc/ is 32-bit and cannot be
# linked against Qt 6.10.3 / msvc2022_64.

$ErrorActionPreference = 'Stop'

$VlcDir  = 'C:\Program Files\VideoLAN\VLC'
$OutDir  = Join-Path $PSScriptRoot '..\hb-cut-player\vlc\x64'
$VcVars  = 'C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat'

if (-not (Test-Path $VcVars))         { throw "vcvars64.bat not found: $VcVars" }
if (-not (Test-Path "$VlcDir\libvlc.dll")) { throw "VLC not found: $VlcDir" }

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$OutDir = (Resolve-Path $OutDir).Path

# Bring vcvars64 into the current process so dumpbin / lib are on PATH.
$vcEnv = cmd /c "`"$VcVars`" >nul && set"
foreach ($line in $vcEnv) {
    if ($line -match '^([^=]+)=(.*)$') {
        Set-Item -Path "Env:$($Matches[1])" -Value $Matches[2]
    }
}

foreach ($name in @('libvlc', 'libvlccore')) {
    Write-Host "=== Generating $name.lib ==="

    $dll = Join-Path $VlcDir "$name.dll"
    $def = Join-Path $OutDir "$name.def"
    $lib = Join-Path $OutDir "$name.lib"

    $exports = & dumpbin /EXPORTS $dll
    if ($LASTEXITCODE -ne 0) { throw "dumpbin failed for $dll" }

    $names = @()
    $inTable = $false
    foreach ($line in $exports) {
        if ($line -match '^\s*ordinal\s+hint\s+RVA\s+name') { $inTable = $true; continue }
        if (-not $inTable) { continue }
        if ($line -match '^\s*Summary') { break }
        $cols = $line.Trim() -split '\s+'
        if ($cols.Length -ge 4 -and $cols[0] -match '^\d+$') {
            $names += $cols[3]
        }
    }

    if ($names.Count -eq 0) { throw "no exports parsed from $dll" }

    Set-Content -Path $def -Encoding ASCII -Value (@("LIBRARY $name", 'EXPORTS') + $names)

    & lib /def:$def /machine:X64 /out:$lib
    if ($LASTEXITCODE -ne 0) { throw "lib failed for $name" }
}

Write-Host "OK -> $OutDir"
