# N-Phenethyl-4-piperidinone
EU4 Ironman &amp; Multiplayer Console Enabler For Linux. Uses https://github.com/kubo/funchook

![alt text](https://cdn.discordapp.com/attachments/845038412926550107/950242380135661628/download_1.jpeg)

# Usage
(first time use only)
1. Download the latest release (https://github.com/Jiwer/N-Phenethyl-4-piperidinone/releases/tag/linux).
2. Place all funchook files inside /usr/lib/ (sudo mv filename /usr/lib).
3. Place N-Phenethyl-4-piperidinone.so and loadfile inside the same directory.
4. Enable execution on loadfile (chmod +x loadfile).

(non first time use only)
1. Load up EU4 into the main menu.
2. Run loadfile using using sudo (sudo ./loadfile). This will inject N-Phenethyl-4-piperidinone.so into EU4.
3. Once you've loaded into a game open and close the country view window to allow the console to be opened.
4. Press `/~ to open the console and execute commands as normal.

# Important Notes
A lot of commands are not networked. I have removed the ability for the game to desync, but it's best to stick to firing events as your main means of making things in the game happen. https://eu4cheats.com/events

It's best to wait for the first month tick before executing any commands.

# Compiling
g++ -fPIC -c main.cpp

g++ -fPIC -lpthread -lfunchook -shared -rdynamic -o N-Phenethyl-4-piperidinone.so main.o -ldl
