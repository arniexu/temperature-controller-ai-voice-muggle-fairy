# Workspace-local command pack for ai_tempconroler

$script:AiTempcRepo = Split-Path -Parent $PSScriptRoot
$script:AiTempcUv4 = 'C:\Users\xuqianjx\AppData\Local\Keil_v5\UV4\UV4.exe'
$script:AiTempcProj = 'MDK-ARM\ai_tempconroler.uvprojx'
$script:AiTempcTarget = 'ai_tempconroler'
$script:AiTempcCliLog = 'MDK-ARM\build_cli.log'
$script:AiTempcHtmlLog = 'MDK-ARM\ai_tempconroler\ai_tempconroler.build_log.htm'

function Show-AiTempcLog {
    param([int]$Tail = 120)
    Push-Location $script:AiTempcRepo
    try {
        if (Test-Path $script:AiTempcCliLog) {
            Write-Host '===== Keil CLI Log =====' -ForegroundColor Cyan
            Get-Content $script:AiTempcCliLog -Tail $Tail
        } elseif (Test-Path $script:AiTempcHtmlLog) {
            Write-Host '===== Keil HTML Log =====' -ForegroundColor Cyan
            Get-Content $script:AiTempcHtmlLog -Tail $Tail
        } else {
            Write-Host 'No Keil log file found.' -ForegroundColor Yellow
        }
    } finally {
        Pop-Location
    }
}

function Invoke-AiTempcKeil {
    param(
        [Parameter(Mandatory = $true)]
        [ValidateSet('build', 'rebuild', 'flash')]
        [string]$Mode
    )

    if (-not (Test-Path $script:AiTempcUv4)) {
        throw "UV4 not found: $script:AiTempcUv4"
    }

    Push-Location $script:AiTempcRepo
    try {
        $switch = if ($Mode -eq 'build') { '-b' } elseif ($Mode -eq 'rebuild') { '-r' } else { '-f' }
        $args = @($switch, $script:AiTempcProj, '-t', $script:AiTempcTarget, '-j0')

        if ($Mode -ne 'flash') {
            if (Test-Path $script:AiTempcCliLog) { Remove-Item $script:AiTempcCliLog -Force }
            $args += @('-o', $script:AiTempcCliLog)
        }

        Write-Host "Running: UV4 $($args -join ' ')" -ForegroundColor Green
        $p = Start-Process -FilePath $script:AiTempcUv4 -ArgumentList $args -Wait -PassThru
        Show-AiTempcLog

        if ($p.ExitCode -ne 0) {
            throw "UV4 exited with code $($p.ExitCode)"
        }
    } finally {
        Pop-Location
    }
}

function clean_ai_tempc {
    Push-Location $script:AiTempcRepo
    try {
        $outDir = 'MDK-ARM\ai_tempconroler'
        if (-not (Test-Path $outDir)) {
            Write-Host "Output folder not found: $outDir" -ForegroundColor Yellow
            return
        }

        $patterns = @('*.axf','*.hex','*.bin','*.map','*.lst','*.d','*.o','*.crf','*.htm','*.log')
        $deleted = 0
        foreach ($pattern in $patterns) {
            $files = Get-ChildItem -Path $outDir -Filter $pattern -File -ErrorAction SilentlyContinue
            foreach ($f in $files) {
                Remove-Item $f.FullName -Force -ErrorAction SilentlyContinue
                $deleted++
            }
        }
        if (Test-Path $script:AiTempcCliLog) { Remove-Item $script:AiTempcCliLog -Force -ErrorAction SilentlyContinue }
        Write-Host "Clean done. Deleted $deleted files." -ForegroundColor Green
    } finally {
        Pop-Location
    }
}

function build_ai_tempc { Invoke-AiTempcKeil -Mode build }
function rebuild_ai_tempc { Invoke-AiTempcKeil -Mode rebuild }
function flash_ai_tempc { Invoke-AiTempcKeil -Mode flash }

function tempc {
    param(
        [ValidateSet('build','rebuild','clean','flash','log','err','b','r','c','f','l','e')]
        [string]$Action = 'build',
        [int]$Tail = 120
    )

    switch ($Action) {
        'build' { build_ai_tempc; break }
        'b'     { build_ai_tempc; break }
        'rebuild' { rebuild_ai_tempc; break }
        'r'       { rebuild_ai_tempc; break }
        'clean' { clean_ai_tempc; break }
        'c'     { clean_ai_tempc; break }
        'flash' { flash_ai_tempc; break }
        'f'     { flash_ai_tempc; break }
        'log' { Show-AiTempcLog -Tail $Tail; break }
        'l'   { Show-AiTempcLog -Tail $Tail; break }
        'err' {
            Push-Location $script:AiTempcRepo
            try {
                if (Test-Path $script:AiTempcCliLog) {
                    Select-String -Path $script:AiTempcCliLog -Pattern 'error:|Error\(s\)|Target not created' -CaseSensitive:$false
                } elseif (Test-Path $script:AiTempcHtmlLog) {
                    Select-String -Path $script:AiTempcHtmlLog -Pattern 'error:|Error\(s\)|Target not created' -CaseSensitive:$false
                } else {
                    Write-Host 'No Keil log file found.' -ForegroundColor Yellow
                }
            } finally {
                Pop-Location
            }
            break
        }
        'e' {
            tempc err
            break
        }
    }
}

Set-Alias btempc build_ai_tempc
Set-Alias rtempc rebuild_ai_tempc
Set-Alias ctempc clean_ai_tempc
Set-Alias ftempc flash_ai_tempc
Set-Alias tc tempc

Register-ArgumentCompleter -CommandName tempc -ScriptBlock {
    param($commandName, $parameterName, $wordToComplete, $commandAst, $fakeBoundParameters)
    @('build','rebuild','clean','flash','log','err','b','r','c','f','l','e') |
        Where-Object { $_ -like "$wordToComplete*" } |
        ForEach-Object { [System.Management.Automation.CompletionResult]::new($_, $_, 'ParameterValue', $_) }
}
