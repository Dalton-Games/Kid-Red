@echo off
tasklist /FI "WINDOWTITLE eq Docs server" 2>NUL | find /I /N "caddy.exe">NUL
set found="%ERRORLEVEL%"
if NOT %found%=="0" (
	start /MIN "KR Docs server" ".github\caddy\caddy.exe" run --config .github/caddy/Caddyfile
)
start http://localhost:2005
if NOT %found%=="0" (
	color 06
	echo Docs started. Press any key to exit...
	pause >> nul
	taskkill /F /FI "WINDOWTITLE eq KR Docs server" >nul 2>&1
)
