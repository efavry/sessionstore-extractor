# sessionstore-extractor

This little app aim at reading the "sessionstore.js" and backup made by firefox. And extract from them the url. I did not use QJsonClass because of the size in some big session (mine is like 59 mio).

The file is usally found in ~/.mozilla/firefox/something.default/ or ~/.mozilla/firefox/something.default/sessionstore-backups

(windows : C:/Users/Qsm-Cqfd/AppData/Roaming/Mozilla/Firefox/Profiles/something.default)


Compiled with GCC 4.8.2 and QT 4.8.6. Binary can be find in the bin folder
