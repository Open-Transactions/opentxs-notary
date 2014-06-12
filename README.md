opentxs-server
==============

Open Transactions Server

### Contributing

If you are planning to contribute please contact the devs in #opentransactions @ freenode.net IRC chat.

All development goes in develop branch - please don't submit pull requests to master.

Please do *NOT* use an editor that automatically reformats.

The source code is verified with cppcheck (https://github.com/danmar/cppcheck/)
in our continuous integration system. For convenience please enable the git pre-push
hook which will trigger cppcheck each time you are pushing. To do so type in the
repo directory:  
cd .git/hooks  
ln -s ../../scripts/git_hooks/pre-push pre-push  
To check your code without pushing the following command can be used:  
git push -n

### Dependencies
Open Transactions library (otcore, otextensions):
https://github.com/Open-Transactions/libopentxs
