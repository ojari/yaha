param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("Build", "BuildArm", "Vcpkg", "VcpkgArm", "VcpkgWin", "Clean", "CleanArm", "Wc", "Run", "Export", "VsExport")]
    [string]$Do
)

$CMAKE_GENERATOR="Ninja Multi-Config"
$VCPKG="/home/jari/vcpkg/vcpkg"
#$VCPKG="C:/usr/vcpkg/vcpkg.exe"

function doClean {
  param (
      [string]$bld_dir
  )
  Write-Host "clean"
  Remove-Item -Recurse -Force $bld_dir
  mkdir $bld_dir
}

function doBuild {
  param (
      [string]$toolchain,
      [string]$bld_dir
  )
  cmake --fresh -S"." -B"$bld_dir" --toolchain "etc/$toolchain" -G $CMAKE_GENERATOR -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON    
  if ($LASTEXITCODE -ne 0) {
      throw "cmake config exit code: $LASTEXITCODE"
  }

  Write-Host "cmake --build $bld_dir -j 10"
  cmake --build "$bld_dir" -j 10
  if ($LASTEXITCODE -ne 0) {
      throw "cmake build exit code: $LASTEXITCODE"
  }
}


function doVsExport {
  param (
      [string]$toolchain,
      [string]$bld_dir
  )
  Write-Host "Export Visual Studio project"
  cmake --fresh -S"." -B"$bld_dir" --toolchain "etc/$toolchain" -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE="Debug"    
  if ($LASTEXITCODE -ne 0) {
      throw "cmake config exit code: $LASTEXITCODE"
  }
}


if ($Do -eq "Clean") {
  doClean "_build"
}
if ($Do -eq "CleanArm") {
  doClean "_build_arm"
}
if ($Do -eq "Build") {
  doBuild "toolchain-x64.cmake" "_build"
}
if ($Do -eq "BuildArm") {
  doBuild "toolchain-arm64.cmake" "_build_arm"
}
if ($Do -eq "Vcpkg") {
  &"$VCPKG" install --triplet x64-linux-release
}
if ($Do -eq "VcpkgArm") {
  &"$VCPKG" install --triplet arm64-linux-release
}
if ($Do -eq "VcpkgWin") {
  &"$VCPKG" install --triplet x64-windows
}
if ($Do -eq "Wc") {
  cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot
}
if ($Do -eq "Run") {
  ./_build/mqtt/Debug/yaha
}
if ($Do -eq "Export") {
    scp _build_arm/mqtt/yaha pi@${TARGET_HOST}:
    scp *.json pi@${TARGET_HOST}:
}
if ($Do -eq "VsExport") {
  doVsExport "toolchain-win.cmake" "vs"
}
