# 🐛 Centipede

A classic **Centipede arcade game** built in C++ using the [SFML](https://www.sfml-dev.org/) library. Shoot down the centipede as it winds its way down the screen, dodge mushrooms and a roaming scorpion, and rack up as high a score as you can!

---

## 📸 Gameplay Overview

| Element | Description |
|---|---|
| **Player** | Controls a shooter at the bottom of the screen |
| **Centipede** | A 12-segment enemy that winds downward; each segment killed spawns a mushroom |
| **Mushrooms** | Obstacles that alter the centipede's path; require 2 hits to destroy |
| **Scorpion** | A fast-moving enemy that traverses the screen sinusoidally and respawns every 10 seconds |

---

## 🎮 Controls

| Key | Action |
|---|---|
| `←` / `→` Arrow Keys | Move player left / right |
| `↑` / `↓` Arrow Keys | Move player up / down (restricted to bottom zone) |
| `Space` | Fire bullet |
| Close Window | Quit game |

---

## 🏆 Scoring

| Event | Points |
|---|---|
| Destroy a mushroom | +1 |
| Hit a centipede segment | +10 |
| Destroy the centipede head (kills all segments) | +20 per segment |
| Destroy the scorpion | +30 |

---

## ⚙️ Requirements

- **C++17** or later
- **SFML 2.x** — Graphics, Audio modules

### Installing SFML

**macOS (Homebrew):**
```bash
brew install sfml
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libsfml-dev
```

**Windows:**  
Download from [sfml-dev.org](https://www.sfml-dev.org/download.php) and link manually or via vcpkg.

---

## 🛠️ Building & Running

### Compile

```bash
g++ Centipede.cpp -o Centipede \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

### Run

```bash
./Centipede
```

> **Important:** The executable **must be run from the project root directory** so it can locate the `Textures/` and `Music/` asset folders.

---

## 📁 Project Structure

```
Munatzir Mehdi _ Centipede/
├── Centipede.cpp          # Main source file (all game logic)
├── Textures/
│   ├── background.png     # Game background
│   ├── player.png         # Player sprite
│   ├── bullet.png         # Bullet sprite
│   ├── centipede.png      # Centipede body segment sprite
│   ├── head.png           # Centipede head sprite
│   ├── mushroom.png       # Mushroom sprite (2-frame damage states)
│   ├── spider.png         # Scorpion/spider sprite
│   ├── scorpion.png       # Scorpion sprite (alternate)
│   ├── explosion.png      # Explosion effect sprite
│   ├── flea.png           # Flea sprite (asset present)
│   ├── death.png          # Death animation sprite
│   └── score.ttf          # Font used for score display
└── Music/
    ├── field_of_hopes.wav # Background music
    ├── fire1.wav          # Bullet fire sound effect
    └── death.wav          # Player death sound effect
```

---

## 🧩 Game Mechanics

- **Grid-based movement** on a 30×30 tile grid (960×960 resolution, 32px tiles).
- The **centipede moves horizontally** and steps down one row each time it hits a wall or mushroom.
- Shooting the **centipede head** destroys all remaining segments at once.
- When all 12 segments are destroyed, the centipede **resets** at the top of the screen.
- The **scorpion** moves in a sinusoidal path across the screen and respawns every 10 seconds after being killed.
- Touching the centipede, a mushroom, or the scorpion **instantly ends the game**.

---

## 📐 Resolution / Window Size

The game renders at **960×960** internally. The displayed window defaults to **640×640** (suitable for 768p displays). To change it, edit the `window.setSize(...)` line in `main()`:

```cpp
window.setSize(sf::Vector2u(640, 640));   // 768p displays
// window.setSize(sf::Vector2u(1280, 1280)); // 1440p displays
// window.setSize(sf::Vector2u(1920, 1920)); // 4K displays
```

---

## 👤 Author

**Muntazir Mehdi**
