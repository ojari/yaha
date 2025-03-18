param(
    [Parameter(Mandatory=$true)]
    [ValidateSet(
      "Build", "BuildArm", 
      "Vcpkg", "VcpkgArm", "VcpkgWin", 
      "Clean", "Wc", "Run", "Test",
      "Export", "ExportAll", "VsExport")]
    [string]$Do,
    [bool]$Arm = $false
)

$GENERATOR="Ninja Multi-Config"
$VCPKG="/home/jari/vcpkg/vcpkg"
#$VCPKG="C:/usr/vcpkg/vcpkg.exe"

if ($Arm) {
  Write-Host "ARM"
  $bld_dir = "_build_arm"
} else {
  Write-Host "x64"
  $bld_dir = "_build"
}

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
      [string]$bld_dir,
      [string]$bld_type
  )
  write-host cmake --fresh -S"." -B"$bld_dir" `
        --toolchain "etc/$toolchain" `
        -G "$GENERATOR" `
        -DCMAKE_BUILD_TYPE="$bld_type" `
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
        --log-level=NOTICE

  cmake --fresh -S"." -B"$bld_dir" `
        --toolchain "etc/$toolchain" `
        -G "$GENERATOR" `
        -DCMAKE_BUILD_TYPE="$bld_type" `
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
        --log-level=NOTICE
  if ($LASTEXITCODE -ne 0) {
      throw "cmake config exit code: $LASTEXITCODE"
  }

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

function doExport {
  $target = $env:TARGET_HOST
  write-host "Export to $target"
  Set-Location "etc"
  ./make_cfg.ps1
  Set-Location ".."

  scp _build_arm/mqtt/Debug/yaha pi@${target}:
  scp etc/gen_devices.json pi@${target}:devices.json
  scp etc/gen_automation.json pi@${target}:automation.json
}


switch ($Do) {
  "Clean" {
    doClean $bld_dir
  }
  "Build" {
    doBuild "toolchain-x64.cmake" "_build" "Debug"
  }
  "BuildArm" {
    doBuild "toolchain-arm64.cmake" "_build_arm" "Release"
  }
  "Vcpkg" {
    &"$VCPKG" install --triplet x64-linux-release
  }
  "VcpkgArm" {
    &"$VCPKG" install --triplet arm64-linux-release
  }
  "VcpkgWin" {
    &"$VCPKG" install --triplet x64-windows
  }
  "Wc" {
    cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot
  }
  "Run" {
    ./_build/mqtt/Debug/yaha
  }
  "Test" {
    doBuild "toolchain-x64.cmake" "_build" "Debug"
    set-location "_build"
    ctest -T test -C Debug
    ctest -T coverage
    set-location ".."
  }
  "Export" {
    doExport
  }
  "ExportAll" {
    &"$VCPKG" install --triplet arm64-linux-release
    doBuild "toolchain-arm64.cmake" "_build_arm" "Release"
    doExport
  }
  "VsExport" {
    doVsExport "toolchain-win.cmake" "vs"
  }
  default {
    throw "Invalid option: $Do"
  }
}
