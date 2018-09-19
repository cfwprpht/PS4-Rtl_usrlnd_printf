     ______ _____ _       _   _          _               _  ______     _       _    __ 
     | ___ \_   _| |     | | | |        | |             | | | ___ \   (_)     | |  / _|
     | |_/ / | | | |     | | | |___ _ __| |    _ __   __| | | |_/ / __ _ _ __ | |_| |_ 
     |    /  | | | |     | | | / __| '__| |   | '_ \ / _` | |  __/ '__| | '_ \| __|  _|
     | |\ \  | | | |____ | |_| \__ \ |  | |___| | | | (_| | | |  | |  | | | | | |_| |  
     \_| \_| \_/ \_____/  \___/|___/_|  \_____/_| |_|\__,_| \_|  |_|  |_|_| |_|\__|_| 
     
## What's this ?
     A driver for debugging reasons. Will use syscall 11 to run a
     tiny kernel payload to wrapp the non working "printf" casts
     on a retail console to the kernels "printf".
     
## Does it need any special drivers to be loaded ?
     No. Only the system drivers which are automaticly loaded.
     
## How to use ?
     You need to add the PRX project to yours app one
     and set a depense from your project to the dll.
     Done. Use "_printf" to write to the System UART.
          
     
     -Have Fun- cfwprpht