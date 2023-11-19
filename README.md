# MouseMacroCreator
A simple macro / scripting language and interpreter primarily used for simulating mouse and keyboard actions, this is solely being developed to increase my skills in various areas of C++

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

## Sleep
- Creates a delay before executing the next action  
Usage: `Sleep <number>`
> [!NOTE]
> Delay number is in milliseconds

## MouseDown
- Presses down a specified mouse button  
Usage: `MouseDown <button>`
> [!TIP]
> Currently supported mouse buttons:  
> `Left` ( Mouse 1 )  
> `Right` ( Mouse 2 )  
> `Middle` ( Mouse 3 )  
