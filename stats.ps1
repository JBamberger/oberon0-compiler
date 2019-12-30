$no_gen = Get-ChildItem -Exclude out,3rdparty,build,.vs,.idea,cmake-build-*;
$sources = $no_gen | Get-ChildItem -Recurse -Include *.cc,*.cpp,*.h,*.hpp,*.c;


$res = $sources | ForEach-Object {
    $lines = ($_ | Get-Content | Measure-Object -Line).Lines;
    return [PSCustomObject]@{path=$_.FullName; lines=$lines};
};
$table = $res | Sort-Object -Property lines;
$stats = $res | Measure-Object -Sum -Average -Maximum -Minimum -Property lines;
$summary = "Statistics: Total: {0}, Average: {1}, Min: {2}, Max: {3}";
$summary = $summary -f $stats.Sum, $stats.Average, $stats.Minimum, $stats.Maximum;


$table | Format-Table;
Out-Host -InputObject $summary;