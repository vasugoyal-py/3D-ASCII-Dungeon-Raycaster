# TERMINAL 3D RAYCASTER DUNGEON CRAWLER
#### Video Demo: https://www.youtube.com/watch?v=MB3LoFUeH0E

### Project Overview
The Terminal 3D Raycaster Dungeon Crawler is an entirely C language made first person dungeon crawler that simulates fake 3D maze like explorations and is made to run natively on Windows. This is made by using the thought of retro games notably using *Wolfenstein 3D (1992)*.
This project is meant to render a 3D world from a 2D grid of map using Digital Differential Analysis (DDA) raycasting.

Here rather than using any type of high level graphical libraries or APIs like OpenGL, DirectX, or external game libraries like SDL or Raylib. This project is meant to draw every pixel of texture itself from scratch. It overlays many different textures on top of another to give this feel and it's done purely with ASCII and now Unicode characters (for walls). The current game has multiple playable levels, namely:
```
1). Starter Dungeon (16 * 16)
2). Wide Rectangle (20 * 8)
3). L-Shaped Room (16 * 12)
4). Grand Hall (32 * 32)
5). Labyrinth (31 * 21)
```
Of which the Grand Hall is definitely my favourite to work with; it's the most open map (except the L-Shaped Room) and definitely the most popular among my friends.
This was definitely the hardest project I had ever worked on in my entire life since understanding the math was the toughest battle. It took all I had out of me with the amount of sleepless nights it took. I would say this roughly took 2 to 3 weeks out of me as I had started working on it long before I had finished the Harvard course and gotten to the last week.
The hardest part in this was definitely implementing the first version of the render function when I made it. When I had first made it, I had made it in only a 2D world; it was a basic player solving a 2D maze like the minimap up top, but then I had to use the render function to its full capabilities and it still haunts me with how difficult it was to implement.
Right now this game runs at a smooth 30ish fps but keeping the A or D keys pressed down can lead to some glitches or frame cuts mainly along the middle of the screen since that is where from the render function builds the world up.

##### Controls

* **W / S:** Move forward or backward along the direction the player is looking.

* **A / D:** Turn the camera left or right. This updates the direction the player sees.

* **Q:** Quit the game immediately. Bring back the cursor in the terminal.

#### Mechanics & Objective

The player walks through corridors looking for Brass Keys. Keys show up on the mini‑map and 3D screen as `*`. When the player steps on a key the key count goes up. The key tile disappears. Locked doors appear as `|` in 3D and as `[` `]` on the radar. Doors block the path until the player has at least one key. Using a key opens the door. Removes the key from the inventory. The final goal is to find the Exit Portal, marked by `@`. Stepping on the portal shows the victory screen.

---

### The Raycasting Engine & Mathematics
The main rendering technique is to transform a 2D tile array to a 3D perspective image by casting an invisible ray to it for each column in the screen
(Here `SCREEN_WIDTH = 130`).

1. **Camera Plane and Ray Direction.**
    The player's view is marked by 2 vectors, one vector is
    1. direction vectors {dirX} and {dirY},
    2. Camera vectors {planeX} and {planeY}

    The camera plane is always perpendicular to the directional vectors, and for each column the camera plane coordinates are marked between [-1, 1].

    $$\text{cameraX} = \frac{2 \cdot x}{\text{SCREEN\_WIDTH}} - 1.0$$
    $$\text{rayDirX} = \text{dirX} + \text{planeX} \cdot \text{cameraX}$$
    $$\text{rayDirY} = \text{dirY} + \text{planeY} \cdot \text{cameraX}$$

2. **Digital Differential Analysis (DDA):**
    Rather than checking for the collision point by point each time (as such a process will get very expensive computationally and slowly keep up the cost and is likely to miss the walls by going into or through them),
    the algorithm steps forward through the map one step at a time along either the X or Y axis to find the nearest object depending on which grid line is the closest to it.

3. **Elimination of the "Fisheye" Lens Effect:**
    If we were to continue like this on a flat surface or a continuous wall from left to right, the walls to the side would appear more behind and the walls to the front would appear closer, creating a fisheye effect like from fisheye cameras. To fix this we also calculate the perpendicular wall distance like
    $$\text{perpWallDist} = \text{sideDist} - \text{deltaDist}$$
    This projects the collision distance onto the player's camera plane, keeping straight walls flat and perspective accurate.

4. **Column Height & Depth Shading:**
    The projected line height on screen is inversely proportional to perpendicular distance:
    $$\text{lineHeight} = \frac{\text{SCREEN\_HEIGHT}}{\text{perpWallDist}}$$
    Walls are textured and shaded based on distance using four Unicode block density glyphs:
    * Close $(< 2.0\text{ tiles})$: Full block (`█`)
    * Mid-range $(< 4.0\text{ tiles})$: Dark shade (`▓`)
    * Far $(< 8.0\text{ tiles})$: Medium shade (`▒`)
    * Distant $(\ge 8.0\text{ tiles})$: Light shade (`░`)

---

### Project File Architecture

* **`main.c`:** This is the main starting file and the file that ties up and executes the functions and maps defined in all the other files.

* **`helper.h`:** Defines main constants such as `SCREEN_WIDTH`, `SCREEN_HEIGHT`, speeds, a `Color` enum for ANSI colors and a `GameState` struct. This is the most important structure as it holds the values of the player such as its position, direction, camera plane, key, key found, log, map, etc.

* **`helper.c`:** Contains the core functions:

    * `enable_escape_char()`: Sets Windows console flags (`ENABLE_VIRTUAL_TERMINAL_PROCESSING` and `CP_UTF8`) to support ANSI formatting and UTF‑8 box characters.

    * `render()`: Performs DDA ray traversal for all 130 columns and fills a 2‑D character and color matrix.

    * `overlay_minimap()`: Draws a 5×5 radar window in the top‑right corner showing walls, door states, items and the player's facing arrow (`^` `>` `v` `<`).

    * `present()`: Uses buffering to draw the screen.

    * `try_move()`: Handles collision detection and movement logic.

* **`map.h`:** Defines the `Map` struct. It also created the functions `map_count()` and `get_map()`.

* **`map.c`:** Stores 1‑D arrays that translate each level and the static list of `Map` definitions.

---

### Key Design Decisions and Challenges Overcome

1. **Eliminating Screen Tearing and Flicker:**

    Any and all attempts using `system("cls")` to clear the screen each frame failed miserably as they caused this flicker and wasted CPU. The solution was to replace that with buffering. An internal buffer (`static char frame[128 * 1024]`) is filled with `sprintf`. The cursor jumps to top left (starting position) with `\033[H`. The whole screen is printed at once with `fputs()`. This removes flicker and consumes less CPU power.

2. **Optimized ANSI Color State Switching:**

    Sending a color code for every cell would enlarge the output buffer. Slow the console. The loop in `present()` keeps track of the color (`ccolor`). A new ANSI code is written when the color actually changes. This keeps the buffer small and the frame rate high.

3. **Input. The `_getch()` Extended Scan Code Mystery:**

    A significant bug was found during testing. The bug made the engine freeze or misinterpret strokes during keyboard input. Investigation showed that `_getch()` returns an int of a standard char. When arrow keys or extended function keys were pressed, two bytes were returned. That caused signed 8‑bit char variables to wrap around into values (-32). Storing input as a character and strictly evaluating character constants fixed the bug forever.

---

### **USE of AI**

Though I am not very proud to admit it, yes, I have certainly used AI in this project. First of all, every line of code in the final file was handwritten by me. Nothing was copy pasted and I understood every line I used, but to understand everything I had used AI to learn and understand how to even build such a massive project.

AI was also used to format and in writing of this md document. But barely, I would say, as I had to type much of it.

---

### Execution

This project is written in C, for Windows environments and can be compiled using GCC (MinGW / MSYS2 / Clang):

```bash
gcc -O3 -s main.c helper.c map.c -o game.exe -lm

./game.exe
```

Note: the `-s` can be removed; it's to make the final size of the exe smaller by removing all the debugging code, but it can be removed nonetheless.