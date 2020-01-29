function Invoke-Environment {
    param
    (
        # Any cmd shell command, normally a configuration batch file.
        [Parameter(Mandatory=$true)]
        [string] $Command
    )
    
    $Command = "`"" + $Command + "`""
    cmd /c "$Command > nul 2>&1 && set" | . { process {
        if ($_ -match '^([^=]+)=(.*)') {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
        }
    }}
}
    

$vs = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
Invoke-Environment $vs


$name = "Test.asm"

$file = Get-Item $name;
if ($file.Extension -ne ".asm") {
    Write-Error "Not an asm file."
    return;
}

nasm -f win64 $file.FullName
if (!$?) {
    Write-Error "Could not assemble."
    return;
}

$obj = Get-Item "$($file.BaseName).obj"
if (!$obj) {
    Write-Error "Could not assemble."
    return;
}

link /machine:x64 $obj msvcrt.lib legacy_stdio_definitions.lib

if (!$?) {
    Write-Error "Could not link."
    return;
}

.\Test.exe