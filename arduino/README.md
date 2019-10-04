Arduino
=======
Arduino code for tuentiengito

Status
======
NOT WORKING
* [!] High current spike when activating motors make the raspberry reboot. 
	* Solutions:
		* Protect with capacitors
		* Reduce voltage? (Remove 12V regulator)
* [i] Higher baud rate is overflowing on Input library (wiljan's, already reported and fix has been PRed)
* [i] Voltage regulator removed to check if 5V were enough to run without the current spike.


TODO
====
* [ ] Add voltage regulator again (12V or less)
* [ ] Add supply wires from USB to H-bridge (removed when extracting voltage regulator)
