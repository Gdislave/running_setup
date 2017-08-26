# running_setup


to run the minimum setup do:

roscore
./src/adhoc_package/adhoc_communication/scripts/lanchCars.sh

# customize setup
if a new setup is needed on a dedicated machine the following steps are needed for customize:

edit:
adhoc_package/adhoc_communication/src/header.h
all wlp-names needs to fit the device


adhoc_package/adhoc_communication/scripts/launchCars.sh
line 745 needs an edit for the correct wlp device name

