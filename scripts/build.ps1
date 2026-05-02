# Configure and build the project. Uses Qt 6.10.3 / msvc2022_64.

$ErrorActionPreference = 'Stop'

$Repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$Build = Join-Path $Repo 'build'

cmake -S $Repo -B $Build -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_PREFIX_PATH="C:/Qt/6.10.3/msvc2022_64"
if ($LASTEXITCODE -ne 0) { throw "configure failed" }

cmake --build $Build --config Release --parallel
if ($LASTEXITCODE -ne 0) { throw "build failed" }

Write-Host "Build OK -> $Build\Release"
