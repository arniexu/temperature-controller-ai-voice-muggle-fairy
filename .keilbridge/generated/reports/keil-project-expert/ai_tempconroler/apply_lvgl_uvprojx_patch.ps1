$ErrorActionPreference = 'Stop'

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..\..')
$projPath = Join-Path $repoRoot 'MDK-ARM\ai_tempconroler.uvprojx'

[xml]$xml = Get-Content -Raw $projPath
$target = $xml.Project.Targets.Target | Where-Object { $_.TargetName -eq 'ai_tempconroler' } | Select-Object -First 1
if (-not $target) {
    throw 'Target ai_tempconroler not found'
}

$requiredIncludes = @(
    '../Middlewares/Third_Party/lvgl',
    '../Middlewares/Third_Party/lvgl/src',
    '../UI/LVGL',
    '../UI/Core/Inc'
)

function Merge-IncludePath {
    param(
        [string]$IncludePathText,
        [string[]]$RequiredIncludes
    )

    $current = @()
    if ($IncludePathText) {
        $current = $includePathText.Split(';') | Where-Object { $_ -and $_.Trim() -ne '' }
    }
    foreach ($inc in $RequiredIncludes) {
        if ($current -notcontains $inc) {
            $current += $inc
        }
    }
    return ($current -join ';')
}

$target.TargetOption.TargetArmAds.Cads.VariousControls.IncludePath = Merge-IncludePath -IncludePathText ([string]$target.TargetOption.TargetArmAds.Cads.VariousControls.IncludePath) -RequiredIncludes $requiredIncludes
$target.TargetOption.TargetArmAds.Aads.VariousControls.IncludePath = Merge-IncludePath -IncludePathText ([string]$target.TargetOption.TargetArmAds.Aads.VariousControls.IncludePath) -RequiredIncludes $requiredIncludes

$groupsNode = $target.Groups
$lvglGroup = $groupsNode.Group | Where-Object { $_.GroupName -eq 'Middlewares/LVGL' } | Select-Object -First 1
if (-not $lvglGroup) {
    $lvglGroup = $xml.CreateElement('Group')
    $groupName = $xml.CreateElement('GroupName')
    $groupName.InnerText = 'Middlewares/LVGL'
    $lvglGroup.AppendChild($groupName) | Out-Null
    $groupsNode.AppendChild($lvglGroup) | Out-Null
}

foreach ($child in @($lvglGroup.ChildNodes)) {
    if ($child.Name -eq 'Files') {
        $lvglGroup.RemoveChild($child) | Out-Null
    }
}

$filesNode = $xml.CreateElement('Files')
$lvglGroup.AppendChild($filesNode) | Out-Null

while ($filesNode.File) {
    $filesNode.RemoveChild($filesNode.File[0]) | Out-Null
}

$excludeFragments = @(
    '\src\draw\arm2d\',
    '\src\draw\nxp\',
    '\src\draw\renesas\',
    '\src\draw\sdl\',
    '\src\draw\stm32_dma2d\',
    '\src\draw\swm341_dma2d\',
    '\src\gpu\'
)

$allowedRegex = @(
    '/src/core/',
    '/src/draw/',
    '/src/hal/',
    '/src/misc/',
    '/src/widgets/',
    '/src/font/lv_font\.c$',
    '/src/font/lv_font_fmt_txt\.c$',
    '/src/font/lv_font_montserrat_8\.c$',
    '/src/font/lv_font_montserrat_10\.c$',
    '/src/font/lv_font_montserrat_12\.c$',
    '/src/font/lv_font_montserrat_14\.c$',
    '/src/font/lv_font_montserrat_16\.c$',
    '/src/font/lv_font_montserrat_20\.c$',
    '/src/font/lv_font_montserrat_24\.c$',
    '/src/font/lv_font_montserrat_28\.c$',
    '/src/font/lv_font_montserrat_32\.c$',
    '/src/font/lv_font_montserrat_36\.c$',
    '/src/font/lv_font_montserrat_40\.c$',
    '/src/font/lv_font_montserrat_48\.c$',
    '/src/extra/layouts/flex/lv_flex\.c$',
    '/src/extra/themes/default/lv_theme_default\.c$',
    '/src/extra/widgets/imgbtn/lv_imgbtn\.c$',
    '/src/extra/widgets/led/lv_led\.c$',
    '/src/extra/widgets/meter/lv_meter\.c$',
    '/src/extra/widgets/spinner/lv_spinner\.c$'
)

function Test-AllowedLvglSource {
    param([string]$NormPath)

    foreach ($rx in $allowedRegex) {
        if ($NormPath -match $rx) {
            return $true
        }
    }
    return $false
}

$lvglSrcRoot = Join-Path $repoRoot 'Middlewares\Third_Party\lvgl\src'
$srcFiles = Get-ChildItem $lvglSrcRoot -Recurse -Filter '*.c' | Where-Object {
    $path = $_.FullName.ToLowerInvariant()
    foreach ($fragment in $excludeFragments) {
        if ($path.Contains($fragment)) {
            return $false
        }
    }

    $norm = '/' + (($path -replace '\\', '/').TrimStart('/'))
    if (-not (Test-AllowedLvglSource -NormPath $norm)) {
        return $false
    }

    return $true
} | Sort-Object FullName

$repoRootNorm = ($repoRoot.Path -replace '\\', '/').TrimEnd('/') + '/'
$added = 0
foreach ($src in $srcFiles) {
    $srcNorm = $src.FullName -replace '\\', '/'
    if (-not $srcNorm.StartsWith($repoRootNorm, [System.StringComparison]::OrdinalIgnoreCase)) {
        continue
    }

    $rel = $srcNorm.Substring($repoRootNorm.Length)
    $keilPath = '../' + $rel

    $fileNode = $xml.CreateElement('File')

    $nameNode = $xml.CreateElement('FileName')
    $nameNode.InnerText = $src.Name
    $fileNode.AppendChild($nameNode) | Out-Null

    $typeNode = $xml.CreateElement('FileType')
    $typeNode.InnerText = '1'
    $fileNode.AppendChild($typeNode) | Out-Null

    $pathNode = $xml.CreateElement('FilePath')
    $pathNode.InnerText = $keilPath
    $fileNode.AppendChild($pathNode) | Out-Null

    $filesNode.AppendChild($fileNode) | Out-Null
    $added++
}

$settings = New-Object System.Xml.XmlWriterSettings
$settings.Indent = $true
$settings.IndentChars = '  '
$settings.NewLineChars = "`r`n"
$settings.NewLineHandling = 'Replace'
$settings.Encoding = New-Object System.Text.UTF8Encoding($true)

$writer = [System.Xml.XmlWriter]::Create($projPath, $settings)
$xml.Save($writer)
$writer.Close()

Write-Output "LVGL project patch applied. Added C files: $added"
