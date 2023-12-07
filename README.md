# MouseMacroCreator
A simple macro / scripting language and interpreter primarily used for simulating mouse and keyboard actions, this is solely being developed to increase my skills in various areas of C++

# Getting Started
Head over to the releases page and download the latest interpreter binary.  
You will need to extract the interpreter from the `.7z` file.  
To execute a created macro, simply drag and drop the macro file onto the interpreter to open it.  

> [!NOTE]
> An installer / file extension association tool is in development, there will be more info here once its ready.   

> [!IMPORTANT]
> Macros must have the file extension '.mmc'  

# Documentation 📖

> [!TIP]
> A Sublime Text syntax format is available [here](https://cdn.discordapp.com/attachments/1109714013773910026/1181598748967710782/mmc.sublime-syntax?ex=6581a496&is=656f2f96&hm=fd430b047acc54a1f632893d5648c153e96b4ca7386d477fa4f7a26a79c3e9e9&)  

## Info

> [!IMPORTANT]
> All commands are to be seperated and written on new lines

> [!TIP]
> Empty lines are supported for code cleaning.  
> Comments may be added by writing `;` before any other character on the line  

## Keywords

## SetToggleType
- Sets the method used to toggle the macro on or off  
Usage: `SetToggleType <type>`  
> [!IMPORTANT]
> All toggle types include:  
> `Toggle`: Toggles the macro on/off with a single keypress  
> `RunOnce`: Toggles the macro on, runs the script once, then turns off again  
> `HoldOn`: Toggles the macro on while the toggle key is held down  
> `HoldOff`: Toggles the macro on while the toggle key is **NOT** held down  

## SetToggleKey
- Sets the key used to toggle the macro on and off  
Usage: `SetToggleKey <key>`  
> [!NOTE]
> Running a script without this keyword will set the default toggle key to F8  

## SetTerminateKey
- Sets the key used to terminate/quit the macro 
Usage: `SetTerminateKey <key>`  
> [!NOTE]
> Running a script without this keyword will set the default toggle key to F9  


## SetMousePos
- Sets the mouse's position to a desired location on the screen  
Usage: `SetMousePos <x> <y>`

## SetMousePosInterpolated
- Sets the mouse's position to a desired location on the screen at a determined speed  
Usage: `SetMousePosInterpolated <x> <y> <delay>`  
> [!NOTE]
> The lower the number entered for delay, the faster the mouse will reach the desired position.  

## Sleep
- Creates a delay before executing the next action  
Usage: `Sleep <number>`
> [!NOTE]
> Delay number is in milliseconds

## Log
- Logs content to the console  
Usage: `Log <message>`  

## MouseDown
#mousedown
- Presses down a specified mouse button  
Usage: `MouseDown <button>`
> [!TIP]
> Currently supported mouse buttons:  
> `Left` ( Mouse 1 )  
> `Right` ( Mouse 2 )  
> `Middle` ( Mouse 3 )

## MouseUp
- Releases a mouse button previously pressed by `MouseDown`  
Usage: `MouseUp <button>`  
> [!TIP]
> For list of supported mouse buttons, look to `MouseDown`

## MouseClick
- Performs both a MouseDown AND MouseUp action  
Usage: `MouseClick <button>`

## KeyDown
- Presses down a specified key on the keyboard  
Usage: `KeyDown <key>`  
> [!TIP]
> All keys supported, if you find one that ISNT, report it so it can be fixed  

## KeyUp
- Releases a key previously pressed by `KeyDown`  
Usage: `KeyUp <key>`  
> [!TIP]
> For list of unsupported keys, look to `KeyDown`  

## KeyPress
- Performs both a KeyDown AND KeyUp Action  
Usage: `KeyPress <key>`
> [!NOTE]
> Keyboard input DOES detect key case  
> Example: KeyDown A would simulate a Shift KeyDown along with A  
> Example: KeyDown a would NOT simulate a Shift Keydown along with a  
