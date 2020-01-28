function Format-Cpp {
    param([switch]$All)
    # $files = Get-ChildItem -Recurse -Include *.h, *.cpp | ForEach-Object { $_.FullName }
    # clang-format.exe -i --style=file @files

    begin {
        if ($All) {
            $files = Get-ChildItem -Recurse -Include *.h, *.cpp | ForEach-Object { $_.FullName }
        }
        else {
            $files = @()
        }
    }
    process {
        if ($null -ne $_) {
            $files += , $_.FullName # append to the array
        }
    }
    end {
        Format-List -InputObject $files
        if ($files.Length -gt 0) {
            clang-format.exe -i --style=file @files
        }
        else {
            Write-Host "Provide some files names."
        }
    }
}

Get-ChildItem ast,codegen,scanner,util | Get-ChildItem -Recurse -include *.cpp,*.h | Format-Cpp