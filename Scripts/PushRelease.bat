pushd %~dp0

cd ..
.\PBSync.exe --autoversion hotfix
.\PBSync.exe --build package
git commit -am "update bins"
.\PBSync.exe --build release
git push

popd