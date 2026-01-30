# cub3D - 42 Istanbul
## 👥 Contributors
This project was developed as a team at 42 Istanbul:
- [Mikail]
- [Abdullah Tursun]
  
**cub3D** is a dynamic 3D graphical representation of a maze from a first-person perspective, inspired by the world-famous *Wolfenstein 3D*. The project focuses on exploring **ray-casting** principles to create a dynamic view inside a 3D environment using the **miniLibX** library.

## 🚀 Overview
The goal of this project is to improve skills in window management, event handling, and practical applications of mathematics without relying on modern high-level 3D engines.

### 🛠 Technical Implementation
- **Language:** C.
- **Graphics Library:** miniLibX (for window, color, and event management).
- **Core Algorithm:** **Ray-casting**, used to render a 2D map into a 3D perspective.
- **Input Handling:** Real-time user movement (W, A, S, D) and rotation (Left/Right arrows).
- **Map Parsing:** Strict parsing of `.cub` scene files, including wall textures (North, South, East, West) and custom floor/ceiling colors.

## 📋 Features
- **3D Maze Rendering:** "Realistic" first-person view based on ray-casting.
- **Texture Mapping:** Different textures for walls depending on their orientation.
- **Robust Parsing:** Explicit error messaging for misconfigured maps or scene files.
- **Clean Exit:** Proper handling of window closure via the ESC key or the window's red cross.

## 🕹 Instructions
### Compilation
The project includes a **Makefile** with all required rules.
```bash
make          # Compiles the cub3D executable
make bonus    # Compiles with bonus features (if applicable)
make clean    # Removes object files
