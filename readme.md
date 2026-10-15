<div align="center">

# 🌀 Console Raycaster

**A Wolfenstein-3D-style raycasting engine, rendered entirely in the Windows console.**

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6.svg)
![Renderer](https://img.shields.io/badge/renderer-ASCII-lightgrey.svg)
![License](https://img.shields.io/badge/license-unset-red.svg)

</div>

---

## 📖 Table of Contents

- [Features](#-features)
- [Requirements](#-requirements)
- [Project Structure](#-project-structure)
- [Building](#-building)
- [Constants](#-constants)
- [Controls](#-controls)
- [License](#-license)

---

## ✨ Features

- **DDA-based raycasting** for wall detection — perpendicular distance avoids fisheye distortion
- **Distance-based shading** — `#`, `%`, `+`, `.` glyphs lighten with distance, and dim on Y-side walls
- **Flicker-free rendering** — a fixed ASCII framebuffer redrawn in place via ANSI cursor-home escapes
- **Interactive gameplay tiles** via `try_move()` (keys, locked doors, exit)
- **HUD + minimap overlay** for live status and navigation
- **Native Windows Console API** input/terminal helpers

<details>
<summary>📸 Preview (swap in your own screenshot or GIF)</summary>
<br>

```
################################################################################
################################################################################
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
................................................................................
................................................................................
```

</details>

---

## ⚙️ Requirements

|  |  |
|---|---|
| **OS** | Windows only — uses `<windows.h>` Console API directly |
| **Compiler** | MinGW-w64 (`gcc`) or MSVC |

---

## 📂 Project Structure

```
.
├── helper.h      # Constants, types, and function declarations
├── helper.c      # Rendering, input helpers, and collision logic
├── main.c        # Game loop, map data, and controls
├── .gitignore
└── README.md
```

---

## 🛠️ Building

```bash
gcc main.c helper.c -o game.exe -lm
```

Run it:

```bash
game.exe
```

---

## 🔢 Constants

Defined in `helper.h`:

| Constant | Value | Meaning |
|---|---|---|
| `MAP_WIDTH`, `MAP_HEIGHT` | `16` | Size of the map grid |
| `SCREEN_WIDTH`, `SCREEN_HEIGHT` | `80`, `22` | 3D viewport size (`+1` HUD line in `present`) |
| `SPEED` | `0.15` | Movement speed per step |
| `ROT_SPEED` | `0.10` | Rotation speed per step |

---

## 🎮 Controls

Movement/rotation and key handling live in `main.c`, using `try_move()` for collision checks and tile interactions.

<details>
<summary>Suggested default bindings (edit to match your <code>main.c</code>)</summary>
<br>

| Key | Action |
|---|---|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Rotate left |
| `D` | Rotate right |
| `Esc` | Quit |

</details>

---

## 📄 License

No license is set yet — add one (e.g. MIT) if you plan to share this publicly.

<div align="center">

*Made with C and too many ANSI escape codes.*

</div>