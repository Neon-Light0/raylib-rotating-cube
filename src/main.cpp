#include <fmt/core.h>
#include <fmt/ranges.h>
#include <raylib.h>
#include <vector>
#include <raymath.h>
#include <thread>
#include <array>

const int targetFPS = 30;
int screenWidth = 800;
int screenHeight = 800;
const int fontSize = 10;
int worldWidth = screenWidth / fontSize;
int worldHeight = screenHeight / fontSize;
int worldCenterX = 0.5 * worldWidth;
int worldCenterY = 0.5 * worldHeight;
int cubeWidth = 30;
std::array<int, 3> cameraPos = {0, 0, -50};
std::array<double, 3> cubeRotation = {0.0, 0.0, 0.0};

std::vector<std::vector<char>> pixelBuffer(worldHeight, std::vector<char>(worldWidth, ' '));
std::vector<std::vector<int>> zBuffer(worldHeight, std::vector<int>(worldWidth, INT_MAX));

enum class Surface {
  FRONT,
  BACK,
  LEFT,
  RIGHT,
  TOP,
  BOTTOM
};

struct Cube{
  std::array<int, 3> pos = {worldCenterX, worldCenterY, 0};
  std::array<double, 3> angles = {0.0, 0.0, 0.0};
  int width = 30;

  void rotate(double angleX, double angleY, double angleZ){
    angles[0] = fmod(angles[0] + angleX, 2 * M_PI);
    angles[1] = fmod(angles[1] + angleY, 2 * M_PI);
    angles[2] = fmod(angles[2] + angleZ, 2 * M_PI);
  }

  std::array<int, 3> getVertices(float x, float y, float z){
    std::array<int, 3> vertices;    
    double angleX = angles[0];
    double angleY = angles[1];
    double angleZ = angles[2];

    // Rotation around X axis
    double cosX = cos(angleX);
    double sinX = sin(angleX);
    double y1 = y * cosX - z * sinX;
    double z1 = y * sinX + z * cosX;

    // Rotation around Y axis
    double cosY = cos(angleY);
    double sinY = sin(angleY);
    double x2 = x * cosY + z1 * sinY;
    double z2 = -x * sinY + z1 * cosY;

    // Rotation around Z axis
    double cosZ = cos(angleZ);
    double sinZ = sin(angleZ);
    vertices[0] = static_cast<int>(std::round(x2 * cosZ - y1 * sinZ));
    vertices[1] = static_cast<int>(std::round(x2 * sinZ + y1 * cosZ));
    vertices[2] = static_cast<int>(std::round(z2));
    return vertices;
  }

  void drawSurface(Surface surface){
    switch (surface){
      case Surface::FRONT:
        for (float x = -width/2; x < width/2; x+=0.5){
          for (float y = -width/2; y < width/2; y+=0.5){
            float z = -width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;
          
            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'F';
              }
            }
          }
        }

        break;
      case Surface::BACK:
        for (float x = -width/2; x < width/2; x+=0.5){
          for (float y = -width/2; y < width/2; y+=0.5){
            float z = width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;

            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'B';
              }
            }
          }
        }

        break;
      case Surface::LEFT:
        for (float z = -width/2; z < width/2; z+=0.5){
          for (float y = -width/2; y < width/2; y+=0.5){
            float x = -width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;

            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'L';
              }
            }
          }
        }

        break;
      case Surface::RIGHT:
        for (float z = -width/2; z < width/2; z+=0.5){
          for (float y = -width/2; y < width/2; y+=0.5){
            float x = width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;

            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'R';
              }
            }
          }
        }

        break;
      case Surface::TOP:
        // Similar implementation for TOP surface
        for (float x = -width/2; x < width/2; x+=0.5){
          for (float z = -width/2; z < width/2; z+=0.5){
            float y = width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;

            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'T';
              }
            }
          }
        }

        break;
      case Surface::BOTTOM:
        // Similar implementation for BOTTOM surface
        for (float x = -width/2; x < width/2; x+=0.5){
          for (float z = -width/2; z < width/2; z+=0.5){
            float y = -width/2 - 1;
            auto [rotatedX, rotatedY, rotatedZ] = getVertices(x, y, z);
            int screenX = worldCenterX + rotatedX;
            int screenY = worldCenterY + rotatedY;

            if (screenX >= 0 && screenX < worldWidth && screenY >= 0 && screenY < worldHeight){
              if (zBuffer[screenY][screenX] > rotatedZ){
                zBuffer[screenY][screenX] = rotatedZ;
                pixelBuffer[screenY][screenX] = 'B';
              }
            }
          }
        }

        break;
    }  // Implementation of surface drawing
  }

  void drawCube(){
    drawSurface(Surface::FRONT);
    drawSurface(Surface::BACK);
    drawSurface(Surface::TOP);
    drawSurface(Surface::BOTTOM);
    drawSurface(Surface::LEFT);
    drawSurface(Surface::RIGHT);  // Implementation of cube drawing
  }
};

void clearBuffers() { 
  for (int i = 0; i < worldHeight; i++){
    for (int j = 0; j < worldWidth; j++){
      pixelBuffer[i][j] = ' ';
      zBuffer[i][j] = INT_MAX;
    }
  }
}

void drawBuffer(){
  for (int i = 0; i < worldHeight; i += 1) {
    for (int j = 0; j < worldWidth; j += 1) {
      if (pixelBuffer[i][j] != ' '){
        char letter[2] = {pixelBuffer[i][j], '\0'};
        DrawText(letter, j * fontSize, i * fontSize, fontSize, WHITE);
      }
    }
  }
}

void run() {
  InitWindow(screenWidth, screenHeight, "Rotating Cube");
  SetTargetFPS(targetFPS);
  Cube cube1;

  // Main loop
  while (!WindowShouldClose()){  // Detect window close button or ESC key

    BeginDrawing();

    ClearBackground(ORANGE);
    clearBuffers();

    cube1.rotate(0.01, 0.01, 0.01);
    cube1.drawCube();

    drawBuffer();  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<std::size_t>(1000/60.0)));
    EndDrawing();
  }

  CloseWindow();  // Close window and OpenGL context
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) ==  "--screenWidth"){
      screenWidth = std::stoi(argv[++i]);
    } else if (std::string(argv[i]) == "--screenHeight"){
      screenHeight = std::stoi(argv[++i]);
    } else {
      fmt::print("Unknown argument: {}\n", argv[i]);
      return 1;
    }
  }

  run();
  return 0;
}