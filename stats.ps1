#
# Copyright 2020 Jannik Bamberger
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

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