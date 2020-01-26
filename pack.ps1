# This script packs the code into a zip file, then unpacks it and runs cmake on
# the code to evaluate that the code compiles.

$out_dir = ".\.milestones\"
function New-TemporaryDirectory {
    # $parent = [System.IO.Path]::GetTempPath()
    $parent = $out_dir
    [string] $name = [System.Guid]::NewGuid()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}


$dt = Get-Date -Format "yyyyMMdd_HHmmss";
$out_name = "$($out_dir)$($dt)_oberon0c_acker_bamberger.zip";

# create output directory
New-Item -ItemType Directory -Name $out_dir -Force | Out-Null;

# build zip file
$sources = Get-ChildItem -Force -Exclude .idea,.vs,build,cmake-build-*,out,.milestones;
$sources | Compress-Archive -DestinationPath $out_name

# save the current location
Push-Location
$loc = Get-Location

# create a temporary build directory and move there
$build_dir = New-TemporaryDirectory
Set-Location $build_dir

# extract sources to ./src
Expand-Archive -DestinationPath ".\src" -Path "$loc\$out_name";

# create and move to ./build
New-Item -ItemType Directory -Name "build" | Out-Null;
Set-Location ".\build"

# configure cmake
cmake "..\src"
$success = $LastExitCode -eq 0

# build the project
if ($success) {
    cmake --build "."
    $success = $success -and ($LastExitCode -ne 0)
}
if ($success) {
    Write-Error  "Failed To compile"
}

# restore the location
Pop-Location

# remove the temp build directory
Remove-Item -Recurse $build_dir


