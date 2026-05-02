# Render hb-cut-player/Resources/icons/app.svg into PNG previews using Qt's
# QSvgRenderer (via a small inline helper). Useful for README screenshots.

$ErrorActionPreference = 'Stop'
$Repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$Svg  = Join-Path $Repo 'hb-cut-player/Resources/icons/app.svg'
$Out  = Join-Path $Repo 'imgs/app-icon-256.png'

Add-Type -AssemblyName System.Drawing

# Use System.Xml + GDI+ is not enough for SVG; instead invoke chrome / edge
# headless to rasterize. msedge is preinstalled on Windows 11.
$edge = (Get-ChildItem 'C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe',
                       'C:\Program Files\Microsoft\Edge\Application\msedge.exe' -ErrorAction SilentlyContinue |
         Select-Object -First 1).FullName
if (-not $edge) { throw 'msedge.exe not found' }

$tmpHtml = Join-Path $env:TEMP 'hb_icon.html'
@"
<!doctype html><html><head><meta charset='utf-8'><style>
body{margin:0;background:transparent}
img{display:block}
</style></head><body><img id='i' src='file:///$($Svg.Replace('\','/'))' width='256' height='256'></body></html>
"@ | Set-Content -Path $tmpHtml -Encoding UTF8

& $edge --headless=new --disable-gpu --hide-scrollbars `
        --window-size=256,256 `
        --default-background-color=00000000 `
        --screenshot="$Out" "file:///$($tmpHtml.Replace('\','/'))" 2>&1 | Out-Null
Write-Host "saved: $Out"
