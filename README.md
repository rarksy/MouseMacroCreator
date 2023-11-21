# MouseMacroCreator
A simple macro / scripting language and interpreter primarily used for simulating mouse and keyboard actions, this is solely being developed to increase my skills in various areas of C++

# Getting Started
Head over to the releases page and download the latest interpreter binary.  
Upon extracting and opening, the interpreter will create a new folder titled `Macros`, this is where you will place any macros you create.  

> [!IMPORTANT]
> Macros must have the file extension '.mmc'  

# Documentation 📖

## Info

> [!IMPORTANT]
> All commands are to be seperated and written on new lines

> [!TIP]
> Empty lines are supported for code cleaning.  
> Comments may be added by writing `;` before any other character on the line  

## Keywords

## SetMousePos
- Sets the mouse's position to a desired location on the screen  
Usage: `SetMousePos <x> <y>`

## SetMousePosInterpolated
- Sets the mouse's position to a desired location on the screen at a determined speed  
Usage: `SetMousePosInterpolated <x> <y> <speed>`  
> [!NOTE]
> The lower the number entered for speed, the faster the mouse will reach the desired position.  

## Sleep
- Creates a delay before executing the next action  
Usage: `Sleep <number>`
> [!NOTE]
> Delay number is in milliseconds


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
