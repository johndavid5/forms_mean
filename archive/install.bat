REM https://www.npmjs.org/package/nsm 
REM nsm - a service manager for node
REM A service manager for node which makes it easy
REM to install node.js modules as native services
REM (daemons) on Windows, Linux and OS X.
REM npm install nsm

REM https://www.npmjs.org/package/daemon
REM Add-on for creating *nix daemons...
REM npm install daemon

REM https://www.npmjs.org/package/read-file
npm install --save read-file
npm install --save line-by-line
npm install --save mongoose
npm install --save node-fs
npm install --save ftp
npm install --save ftp-get
REM instead of monkeying with trying
REM to get node's ftp to work through
REM a proxy, try using good ol' curl...
REM But first, need libcurl to be installed...
REM https://github.com/jiangmiao/node-curl/issues/19
sudo apt-get install libcurl3-dev
REM and may as well install curl executable while we're at it...
sudo apt-get install curl
REM For Windows, see...
REM https://github.com/dhruvbird/http-sync/issues/25
npm install --save node-curl
