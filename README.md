# NBA Fantasy Draft Tool
## Introduction
NBA player optimal selection for online fantasy game based on dynamic programming.   
Support data format from [draftking](http://draftkings.com) and [Tiantian NBA (chinese)](http://ttnba.nbahero.com) now.  

## Electron GUI for Tiantian NBA
Automatic login and get rooms lists and corresponding players lists from server.    
Reserve players and exclude players based on your preference.    
Adjust and customize weights to calculate projected points used for prediction.   
Friendly web GUI calling C++ API and display result.  

## Usage
```bash
git clone https://github.com/aaaeeeo/NBA-Fantasy-Selection.git
cd NBA-Fantasy-Selection
```
### C++ CLI API
```bash
make api
# ./draft-api [salary] [nPG] [nSG] [nSF] [nPF] [nC] [nG] [nF] [nAny] < [input Data file]
./draft-api 200 1 1 1 1 1 1 1 1 < DKSalaries.csv
```

### Electron GUI
Make sure node.js is installed.    
If it is not, [install](https://nodejs.org/en/download/package-manager/) it first.   
```bash
make debug  # run in debug mode
make package # build release package (in ./builds)
make release # run in release mode
```