pushd .\Demo1
dotnet publish --nologo -r linux-arm64 /p:ShowLinkerSizeComparison=true  --configuration Debug
pushd .\bin\Debug\net8.0\linux-arm64\publish
scp  -r .\* root@10.2.2.10:/home/root/demo
popd
popd