param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("build", "rebuild")]
    [string]$Mode
)

$uv4 = "C:\Users\xuqianjx\AppData\Local\Keil_v5\UV4\UV4.exe"
$proj = "MDK-ARM\ai_tempconroler.uvprojx"
$target = "ai_tempconroler"
$log = Join-Path $PSScriptRoot "..\MDK-ARM\build_cli.log"
$htmlLog = Join-Path $PSScriptRoot "..\MDK-ARM\ai_tempconroler\ai_tempconroler.build_log.htm"

if (Test-Path $log) {
    Remove-Item $log -Force
}

$buildSwitch = if ($Mode -eq "rebuild") { "-r" } else { "-b" }
& $uv4 $buildSwitch $proj -t $target -j0 -o $log
$exitCode = $LASTEXITCODE

if (Test-Path $log) {
    Write-Output "===== Keil CLI Log (last 200 lines) ====="
    Get-Content $log -Tail 200
} elseif (Test-Path $htmlLog) {
    Write-Output "===== Keil HTML Log (last 200 lines) ====="
    Get-Content $htmlLog -Tail 200
} else {
    Write-Output "No build log file found."
}

exit $exitCode
