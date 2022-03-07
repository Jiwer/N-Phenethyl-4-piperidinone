# N-Phenethyl-4-piperidinone
EU4 Ironman &amp; Multiplayer Console Enabler For Linux. Uses https://github.com/kubo/funchook

![alt text](https://cdn.discordapp.com/attachments/845038412926550107/950242380135661628/download_1.jpeg)

# Usage
(first time use only)
1. Place all funchook files inside /usr/lib/ (sudo mv filename /usr/lib).
2. Place N-Phenethyl-4-piperidinone.so and loadfile inside the same directory.
3. Enable execution on loafile (chmod +x loadfile).

(non first time use only)
4. Load up EU4 into the main menu.
5. Run loadfile using using sudo (sudo ./loadfile). This will inject N-Phenethyl-4-piperidinone into EU4.
6. Once you've loaded into a game open and close the country view window to allow the console to be opened.
7. Press `/~ to open the console and execute commands as normal.

# Important Notes
A lot of commands will desync the game if you're not the host. It's best to stick to firing events as your main means of making things in the game happen. https://eu4cheats.com/events

It's best to wait for the first month tick before executing any commands.

# Compiling
g++ -fPIC -c main.cpp

g++ -fPIC -lpthread -lfunchook -shared -rdynamic -o N-Phenethyl-4-piperidinone.so main.o -ldl
