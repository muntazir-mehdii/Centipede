/*
NAME:         Muntazir Mehdi
CENTIPEDE
*/

//******************** PROJECT  ********************

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing
// the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;    // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

//**************** Function declearation area. All functions are decleared here
//****************

void drawPlayer(sf::RenderWindow &window, float player[],
                sf::Sprite &playerSprite);
void moveBullet(float bullet[], sf::Clock &bulletClock);
void drawBullet(sf::RenderWindow &window, float bullet[],
                sf::Sprite &bulletSprite);
void drawmushroom(sf::RenderWindow &window, float mushroom[][3],
                  sf::Sprite mushroomSprite[]);
void bulletmushroomcollision(sf::RenderWindow &window, float bullet[],
                             float mushroom[][3], sf::Sprite mushroomSprite[],
                             int &score);
void drawcentipede(sf::RenderWindow &window, float centipede[][3],
                   sf::Sprite centipedeSprite[], sf::Sprite &headSprite);
void resetbullet(float bullet[], float player[]);
void movecentipede(float centipede[][3], sf::Clock &centipedeClock,
                   float dirction[], float mushroom[][3]);
void centipedebulletcollision(sf::RenderWindow &window, float centipede[][3],
                              float bullet[], float dirction[],
                              sf::Sprite centipedeSprite[],
                              sf::Texture &headTexture, float mushroom[][3],
                              sf::Sprite mushroomSprite[], int &score);
void centipedeplayercollision(float centipede[][3], float player[]);
void playermushroomcollision(float mushroom[][3], float player[]);
void resetCentipede(float centipede[][3], float dirction[]);
void drawscorpion(sf::RenderWindow &window, float scorpion[][3],
                  sf::Sprite scorpionSprite[]);
void scorpioncollision(float bullet[], float scorpion[][3], int &score);
void movescorpion(float scorpion[][3], sf::Clock &scorpionClock);
void resetscorpion(float scorpion[][3], sf::Clock scorpionRespawnClock);
void playerscorpioncollision(float scorpion[][3], float player[]);

int main() {
  srand(time(0));

  // Declaring RenderWindow.
  sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede",
                          sf::Style::Close | sf::Style::Titlebar);

  // Used to resize your window if it's too big or too small. Use according to
  // your needs.
  window.setSize(
      sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
  // window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440
  // (1440p) displays. window.setSize(sf::Vector2u(1920, 1920)); // Recommended
  // for 3840x2160 (4k) displays.

  // Used to position your window on every launch. Use according to your needs.
  window.setPosition(sf::Vector2i(100, 0));

  // Initializing Background Music.
  sf::Music bgMusic;
  bgMusic.openFromFile("Music/field_of_hopes.wav");
  bgMusic.play();
  bgMusic.setVolume(50);

  // Initializing bullet sound
  const float bulletVolume = 20.0f;
  sf::SoundBuffer bulletSoundBuffer;
  sf::Sound bulletSound;
  bulletSoundBuffer.loadFromFile("Music/fire1.wav");
  bulletSound.setBuffer(bulletSoundBuffer);
  bulletSound.setVolume(bulletVolume);

  // Initializing Background.
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
  backgroundTexture.loadFromFile("Textures/background.png");
  backgroundSprite.setTexture(backgroundTexture);
  backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.9));

  // Initializing Player and Player Sprites.
  float player[2] = {};
  player[x] = (gameColumns / 2) * boxPixelsX;
  player[y] = (gameColumns - 4) * boxPixelsY;
  sf::Texture playerTexture;
  sf::Sprite playerSprite;
  playerTexture.loadFromFile("Textures/player.png");
  playerSprite.setTexture(playerTexture);
  playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

  // Initializing Bullet and Bullet Sprites.

  float bullet[3] = {};
  bullet[x] = player[x];
  bullet[y] = player[y] - boxPixelsY;
  bullet[exists] = false;
  sf::Clock bulletClock;
  sf::Texture bulletTexture;
  sf::Sprite bulletSprite;
  bulletTexture.loadFromFile("Textures/bullet.png");
  bulletSprite.setTexture(bulletTexture);
  bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

  // Initializing score.
  int score = 0;
  sf::Font font;
  font.loadFromFile("Textures/score.ttf");
  sf::Text scoreText;
  scoreText.setFont(font);
  scoreText.setCharacterSize(30);
  scoreText.setFillColor(sf::Color::Red);
  scoreText.setPosition(10, 10);

  // Initializing mushroom and mushroom Sprites.
  float mushroom[25][3] = {};
  sf::Texture mushroomTexture;
  sf::Sprite mushroomSprite[25];
  mushroomTexture.loadFromFile("Textures/mushroom.png");

  for (int i = 0; i < 25; i++) {
    mushroom[i][x] = rand() % gameColumns * boxPixelsX;
    mushroom[i][y] = (rand() % (7 - gameRows) + 1) * boxPixelsY;
    mushroom[i][exists] = 1;
    mushroomSprite[i].setTexture(mushroomTexture);
    mushroomSprite[i].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
  }

  // Initializing centipede and centipede Sprites.
  float centipede[12][3] = {};
  float dirction[12] = {}; // direction variable for change in direction of
                           // centipede when it hits the wall or mushroom
  for (int i = 0; i < 12; i++) {
    dirction[i] = -32;
  }
  sf::Clock centipedeClock;
  sf::Texture centipedeTexture;
  sf::Sprite centipedeSprite[20];
  centipedeTexture.loadFromFile("Textures/centipede.png");

  // Initializing centipede head and head sprite
  sf::Texture headTexture;
  sf::Sprite headSprite;
  headTexture.loadFromFile("Textures/head.png");
  headSprite.setTexture(headTexture);
  headSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

  for (int i = 0; i < 12; i++) {
    centipede[i][x] = ((gameColumns - 12) + i) * boxPixelsX;
    centipede[i][y] = (gameColumns / 32) * boxPixelsY;
    centipede[i][exists] = true;
    centipedeSprite[i].setTexture(centipedeTexture);
    centipedeSprite[i].setTextureRect(
        sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
  }

  // Initializing scorpion and scorpion Sprites.
  float scorpion[1][3] = {};
  sf::Texture scorpionTexture;
  sf::Sprite scorpionSprite[25];
  sf::Clock scorpionClock;
  sf::Clock scorpionRespawnClock;
  scorpionTexture.loadFromFile("Textures/spider.png");

  for (int i = 0; i < 1; i++) {
    scorpion[i][x] = rand() % gameColumns * boxPixelsX;
    scorpion[i][y] = rand() % gameRows * boxPixelsY;
    scorpion[i][exists] = 1;
    scorpionSprite[i].setTexture(scorpionTexture);
    scorpionSprite[i].setTextureRect(
        sf::IntRect(96, 0, boxPixelsX, boxPixelsY));
  }

  while (window.isOpen()) {
    //*************** Function calling area. Here all the created functions are
    //called ****************

    window.draw(backgroundSprite);
    drawPlayer(window, player, playerSprite);
    drawmushroom(window, mushroom, mushroomSprite);
    drawcentipede(window, centipede, centipedeSprite, headSprite);
    resetbullet(bullet, player);
    movecentipede(centipede, centipedeClock, dirction, mushroom);
    centipedebulletcollision(window, centipede, bullet, dirction,
                             centipedeSprite, headTexture, mushroom,
                             mushroomSprite, score);
    bulletmushroomcollision(window, bullet, mushroom, mushroomSprite, score);
    centipedeplayercollision(centipede, player);
    playermushroomcollision(mushroom, player);
    drawscorpion(window, scorpion, scorpionSprite);
    movescorpion(scorpion, scorpionClock);
    scorpioncollision(bullet, scorpion, score);
    playerscorpioncollision(scorpion, player);

    sf::Time elapsed =
        scorpionRespawnClock
            .getElapsedTime(); // this is function for repeatitive regeneration
                               // of scorpion
    if (elapsed.asSeconds() >= 10.0) {
      resetscorpion(scorpion, scorpionRespawnClock);
      scorpionRespawnClock.restart();
    }

    scoreText.setString("Score: " + std::to_string(score));
    window.draw(scoreText);

    bool allSegmentsDead =
        true; // this is the function for generating centipede again and again
    for (int i = 0; i < 12; i++) {
      if (centipede[i][exists]) {
        allSegmentsDead = false;
        break;
      }
    }

    if (allSegmentsDead) {
      resetCentipede(centipede, dirction);
    }

    if (bullet[exists] == true) {
      moveBullet(bullet, bulletClock);
      drawBullet(window, bullet, bulletSprite);
    }

    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        return 0;
      } else if (e.type == sf::Event::KeyPressed) // keyprssed functions
      {
        if (e.key.code == sf::Keyboard::Right && player[x] < 928) {
          player[x] += boxPixelsX;
        }
        if (e.key.code == sf::Keyboard::Left && player[x] > 0) {
          player[x] -= boxPixelsX;
        }
        if (e.key.code == sf::Keyboard::Up && player[y] > 800 &&
            player[y] < 960) {
          player[y] -= boxPixelsY;
        }
        if (e.key.code == sf::Keyboard::Down && player[y] < 928) {
          player[y] += boxPixelsY;
        }
        if (e.key.code == sf::Keyboard::Space) {
          bullet[exists] = true;
          bulletSound.play();
        }
      }
    }
    window.display();
    window.clear();
  }
}

// *************** Function writing area. Here are all the functions written
// ****************

void drawPlayer(sf::RenderWindow &window, float player[],
                sf::Sprite &playerSprite) {
  playerSprite.setPosition(player[x], player[y]);
  window.draw(playerSprite);
}

void drawBullet(sf::RenderWindow &window, float bullet[],
                sf::Sprite &bulletSprite) {
  bulletSprite.setPosition(bullet[x], bullet[y]);
  window.draw(bulletSprite);
}

void moveBullet(float bullet[], sf::Clock &bulletClock) {
  if (bulletClock.getElapsedTime().asMilliseconds() < 2)
    return;

  bulletClock.restart();

  bullet[y] -= 10;
  if (bullet[y] < -32)
    bullet[exists] = false;
}

void resetbullet(float bullet[], float player[]) {
  if (bullet[exists] == false) {
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
  }
}

void drawmushroom(sf::RenderWindow &window, float mushroom[][3],
                  sf::Sprite mushroomSprite[]) {
  for (int i = 0; i < 25; i++) {
    if (mushroom[i][exists]) {
      mushroomSprite[i].setPosition(mushroom[i][x], mushroom[i][y]);
      window.draw(mushroomSprite[i]);
    }
  }
}

void bulletmushroomcollision(sf::RenderWindow &window, float bullet[],
                             float mushroom[][3], sf::Sprite mushroomSprite[],
                             int &score) {
  for (int i = 0; i < 25; i++) {
    if (mushroom[i][exists] && bullet[exists] && bullet[x] >= mushroom[i][x] &&
        bullet[x] <= mushroom[i][x] + boxPixelsX &&
        bullet[y] >= mushroom[i][y] &&
        bullet[y] <= mushroom[i][y] + boxPixelsY) {
      bullet[exists] = false;
      mushroom[i][exists]++;
      if (mushroom[i][exists] >= 2) {
        if (mushroom[i][exists] == 2) {
          mushroomSprite[i].setTextureRect(
              sf::IntRect(96, 0, boxPixelsX, boxPixelsY));
          window.draw(mushroomSprite[i]);
        }
      }
      if (mushroom[i][exists] == 3) {
        mushroom[i][exists] = false;
        score += 1;
      }
    }
  }
}

void drawcentipede(sf::RenderWindow &window, float centipede[][3],
                   sf::Sprite centipedeSprite[], sf::Sprite &headSprite) {
  for (int i = 0; i < 12; i++) {
    if (centipede[i][exists]) {
      if (i == 0) {
        headSprite.setPosition(centipede[0][x], centipede[0][y]);
        window.draw(headSprite);
      }
      if (i >= 1) {
        centipedeSprite[i].setPosition(centipede[i][x], centipede[i][y]);
        window.draw(centipedeSprite[i]);
      }
    }
  }
}

void movecentipede(float centipede[][3], sf::Clock &centipedeClock,
                   float dirction[], float mushroom[][3]) {
  if (centipedeClock.getElapsedTime().asMilliseconds() < 80)
    return;
  centipedeClock.restart();
  for (int i = 0; i < 12; i++) {
    centipede[i][x] += dirction[i];
    if (centipede[i][x] < 0 || centipede[i][x] > 928) {
      dirction[i] = -dirction[i];
      centipede[i][y] += 32;
    }
    for (int j = 0; j < 25; j++) {
      if (centipede[i][exists] && mushroom[j][exists] &&
          centipede[i][x] >= mushroom[j][x] &&
          centipede[i][x] <= mushroom[j][x] + boxPixelsX &&
          centipede[i][y] >= mushroom[j][y] &&
          centipede[i][y] <= mushroom[j][y] + boxPixelsY) {
        centipede[i][y] += 32;
        dirction[i] = -dirction[i];
      }
    }
    if ((centipede[i][y] == 960) &&
        (centipede[i][x] < 0 || centipede[i][x] > 928)) {
      centipede[i][y] -= 160;
    }
  }
}

void centipedebulletcollision(sf::RenderWindow &window, float centipede[][3],
                              float bullet[], float dirction[],
                              sf::Sprite centipedeSprite[],
                              sf::Texture &headTexture, float mushroom[][3],
                              sf::Sprite mushroomSprite[], int &score) {
  for (int i = 0; i < 12; i++) {
    if (centipede[i][exists] && bullet[exists] &&
        bullet[x] >= centipede[i][x] &&
        bullet[x] <= centipede[i][x] + boxPixelsX &&
        bullet[y] >= centipede[i][y] &&
        bullet[y] <= centipede[i][y] + boxPixelsY) {
      int mushroomIndex = -1;
      for (int j = 0; j < 25; j++) {
        if (!mushroom[j][exists]) {
          mushroomIndex = j;
          break;
        }
      }

      if (mushroomIndex != -1) {
        mushroom[mushroomIndex][x] = centipede[i][x];
        mushroom[mushroomIndex][y] = centipede[i][y];
        mushroom[mushroomIndex][exists] = 1;
        mushroomSprite[mushroomIndex].setTextureRect(
            sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
      }
      if (centipede[i][y] <= 960 && centipede[i][y] >= 800) {
        if (mushroomIndex != -1) {
          mushroom[mushroomIndex][x] = centipede[i][x];
          mushroom[mushroomIndex][y] = centipede[i][y];
          mushroom[mushroomIndex][exists] = 1;
          mushroomSprite[mushroomIndex].setTextureRect(
              sf::IntRect(0, 32, boxPixelsX, boxPixelsY));
        }
      }

      if (i == 0 && bullet[x] >= centipede[i][x]) {
        for (int j = 0; j < 12; j++) {
          bullet[exists] = false;
          centipede[j][exists] = false;
          score += 20;
        }
      }

      bullet[exists] = false;
      centipede[i][exists] = false;
      score += 10;

      if (centipede[i + 1][exists]) {
        centipedeSprite[i + 1].setTexture(headTexture);
        centipedeSprite[i + 1].setTextureRect(
            sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
      }

      if (centipede[i][exists] == false) {
        for (int j = i; j < 12; j++) {
          dirction[j] = -dirction[j];
        }
      }
    }
  }
}

void centipedeplayercollision(float centipede[][3], float player[]) {
  for (int i = 0; i < 12; i++) {
    if (centipede[i][exists] && player[exists] &&
        player[x] >= centipede[i][x] &&
        player[x] <= centipede[i][x] + boxPixelsX &&
        player[y] >= centipede[i][y] &&
        player[y] <= centipede[i][y] + boxPixelsY) {
      const float deathVolume = 50.0f;
      sf::SoundBuffer deathSoundBuffer;
      sf::Sound deathSound;
      deathSoundBuffer.loadFromFile("Music/death.wav");
      deathSound.setBuffer(deathSoundBuffer);
      deathSound.setVolume(deathVolume);
      deathSound.play();
      sleep(1.5);
      exit(0);
    }
  }
}

void playermushroomcollision(float mushroom[][3], float player[]) {
  for (int i = 0; i < 25; i++) {
    if (mushroom[i][exists] && player[exists] && player[x] >= mushroom[i][x] &&
        player[x] <= mushroom[i][x] + boxPixelsX &&
        player[y] >= mushroom[i][y] &&
        player[y] <= mushroom[i][y] + boxPixelsY) {
      const float deathVolume = 50.0f;
      sf::SoundBuffer deathSoundBuffer;
      sf::Sound deathSound;
      deathSoundBuffer.loadFromFile("Music/death.wav");
      deathSound.setBuffer(deathSoundBuffer);
      deathSound.setVolume(deathVolume);
      deathSound.play();
      sleep(1.5);
      exit(0);
    }
  }
}

void resetCentipede(float centipede[][3], float dirction[]) {
  for (int i = 0; i < 12; i++) {
    centipede[i][x] = ((gameColumns - 12) + i) * boxPixelsX;
    centipede[i][y] = (gameColumns / 32) * boxPixelsY;
    centipede[i][exists] = true;
    dirction[i] = -32;
  }
}

void drawscorpion(sf::RenderWindow &window, float scorpion[][3],
                  sf::Sprite scorpionSprite[]) {
  for (int i = 0; i < 1; i++) {
    if (scorpion[i][exists]) {
      scorpionSprite[i].setPosition(scorpion[i][x], scorpion[i][y]);
      window.draw(scorpionSprite[i]);
    }
  }
}

void movescorpion(float scorpion[][3], sf::Clock &scorpionClock) {
  if (scorpionClock.getElapsedTime().asMilliseconds() < 20)
    return;

  scorpionClock.restart();
  for (int i = 0; i < 1; i++) {
    if (scorpion[i][exists]) {
      scorpion[i][x] -= 2;
      scorpion[i][y] += sin(scorpion[i][x] / 32.0) * 16;

      if (scorpion[i][x] < 0 || scorpion[i][y] < 0 ||
          scorpion[i][y] > resolutionY) {
        scorpion[i][x] = gameColumns * boxPixelsX;
        scorpion[i][y] = rand() % resolutionY;
      }
    }
  }
}

void scorpioncollision(float bullet[], float scorpion[][3], int &score) {
  for (int i = 0; i < 1; i++) {
    if (scorpion[i][exists] && bullet[exists] && bullet[x] >= scorpion[i][x] &&
        bullet[x] <= scorpion[i][x] + boxPixelsX &&
        bullet[y] >= scorpion[i][y] &&
        bullet[y] <= scorpion[i][y] + boxPixelsY) {
      for (int i = 0; i < 1; i++) {
        scorpion[i][exists] = false;
        bullet[exists] = false;
        score += 30;
      }
    }
  }
}

void resetscorpion(float scorpion[][3], sf::Clock scorpionRespawnClock) {
  if (scorpionRespawnClock.getElapsedTime().asSeconds() >= 10.0) {
    for (int i = 0; i < 1; i++) {
      if (!scorpion[i][exists]) {
        scorpion[i][x] -= 2;
        scorpion[i][y] += sin(scorpion[i][x] / 32.0) * 16;
        scorpion[i][exists] = true;
      }
    }
  }
}

void playerscorpioncollision(float scorpion[][3], float player[]) {
  for (int i = 0; i < 1; i++) {
    if (scorpion[i][exists] && player[exists] && player[x] >= scorpion[i][x] &&
        player[x] <= scorpion[i][x] + boxPixelsX &&
        player[y] >= scorpion[i][y] &&
        player[y] <= scorpion[i][y] + boxPixelsY) {
      const float deathVolume = 50.0f;
      sf::SoundBuffer deathSoundBuffer;
      sf::Sound deathSound;
      deathSoundBuffer.loadFromFile("Music/death.wav");
      deathSound.setBuffer(deathSoundBuffer);
      deathSound.setVolume(deathVolume);
      deathSound.play();
      sleep(1.5);
      exit(0);
    }
  }
}