# **Description**

This project is a 2D platformer game developed in C++ using SFML for rendering graphics and handling events. The game features running, jumping, and shooting mechanics, along with enemies with unique behaviors inherited from a virtual base class. The maps are stored in .tmj format and are processed using the nlohmann/json library.

# **Features**

Player Abilities: The player can run, jump, and shoot.

Enemy AI: Different enemy types with customizable behavior derived from a shared base class.

Map Support: Maps are defined in .tmj format and parsed seamlessly.

Extendable Design: Easy to add new enemies, objects, or mechanics.

High Performance: Optimized rendering and event handling with SFML.

# **Requirements**

To build and run this project, you need:

Compiler: GCC, Clang, or MSVC supporting C++17.

SFML: Version 2.5 or newer.

nlohmann/json: Integrated as an external library.

# **Installation**

Clone the Repository

git clone https://github.com/LOOK-MOM-I-CAN-FLY/SFML-GAME.git
cd SFML-GAME

Build with CMake

Create a build directory:

mkdir build
cd build

Generate build files and compile:

cmake ..
cmake --build .

Run the Game

After building, you can run the executable:

./SFML-GAME

Enjoy the game!

# **Gameplay Instructions**

Controls:

Move left/right: Arrow Keys

Jump: Space

Shoot: Ctrl

Objective:

Navigate through levels, defeat enemies, and reach the goal.

Tips:

Use the environment to your advantage.

Enemies may have unique patterns; observe before attacking.

# **File Structure**

src/: Contains the source code.

assets/: Stores game assets such as textures, sprites, and sounds.

maps/: Includes .tmj map files.

build/: Build directory (generated during compilation).

# **Contributing**

Contributions are welcome! If you want to contribute:

Fork the repository.

Create a new branch for your feature or bug fix:

git checkout -b feature-name

Commit your changes:

git commit -m "Description of changes"

Push your branch:

git push origin feature-name

Create a Pull Request.

# **Future Plans**

Adding more enemy types and behaviors.

Improving animations and visual effects.

Implementing a save/load system for game progress.

Introducing new power-ups and abilities for the player.

Expanding levels and creating new challenges.

# **License**

This project is licensed under the MIT License. See the LICENSE file for more details.

# **Acknowledgments**

SFML: For providing a robust library for 2D game development.

nlohmann/json: For making JSON parsing in C++ effortless.

Open Source Community: For inspiration and support.

# **Contact**

If you have any questions or suggestions, feel free to reach out:

GitHub Issues: Submit your issues here.

Email: igrik315.nekrasov@yandex.ru

Telegram: @Sindi_hall

--------------------------------------------------------------------------------------------

Enjoy playing and feel free to contribute to the development of this exciting 2D platformer!
